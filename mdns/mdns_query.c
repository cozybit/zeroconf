/*
 * Copyright (C) cozybit, Inc. 2007-2010. All Rights Reserved.
 *
 * Use and redistribution subject to licensing terms.
 */
#include "mdns.h"
#include "mdns_private.h"
#include "mdns_port.h"
#include "queue.h"

#ifdef MDNS_QUERY_API

/* OVERVIEW
 *
 * The querier monitors any service instances on the network with the service
 * types specified by calls to mdns_query_monitor.  Each service instance has
 * at least 3 records of interest: a PTR which points to a SRV which points to
 * an A which conains an ipaddr.  The SRV may have an associated TXT record
 * that also needs to be tracked.  Information about the PTR/SRV/TXT records
 * are stored as a service_instance, and the service_instance contains a
 * reference to an A record.  We have this indirection because many services
 * may have the same A record.  Accordingly, each A record maintains a
 * cross-reference list of service_instances that refer to it.  This enables
 * updates to the A record to be passed on to the user without endless amounts
 * of searching.
 *
 * When we receive a message, we sort the answer records by type (see
 * mdns_parse_message).  This allows us to create and populate any new service
 * instances, update existing service instances, create new a records, update
 * existing A records, and notify the user without having to traverse the
 * entire list of services that we are monitoring over and over.  See
 * update_service_cache for details on the algorithm.
 */

/* global mdns state */
static void *query_thread;
static int ctrl_sock;
static int query_enabled;
static int mc_sock;
static struct mdns_message tx_msg;
static struct mdns_message rx_msg;

enum arec_state {
	AREC_STATE_INIT = 0,
	AREC_STATE_QUERYING,
	AREC_STATE_RESOLVED,
};

enum arec_event {
	AREC_EVENT_RX_REC = 0,
	AREC_EVENT_ADD_QUESTIONS,
};

struct service_instance;
SLIST_HEAD(sinst_list, service_instance);

struct arec {
	SLIST_ENTRY(arec) list_item;
	struct sinst_list sinsts;
	uint32_t ipaddr;
	uint32_t ttl;
	uint8_t fqdn[MDNS_MAX_NAME_LEN + 1];
	enum arec_state state;
	uint32_t next_refresh;
	int ttl_percent;
};

SLIST_HEAD(arecs_list, arec);
static struct arecs_list arecs_active;
static struct arecs_list arecs_free;
static struct arec arecs[MDNS_SERVICE_CACHE_SIZE];

enum sinst_state {
	SINST_STATE_INIT = 0,
	SINST_STATE_CLEAN,
	SINST_STATE_UPDATING,
};

enum sinst_event {
	SINST_EVENT_GOT_TXT = 0,
	SINST_EVENT_GOT_SRV,
	SINST_EVENT_GOT_AREC,
	SINST_EVENT_LOST_AREC,
	SINST_EVENT_ADD_QUESTIONS,
};

struct service_monitor;

/* Each service instance that we detect is stored in a service instance */
struct service_instance {
	SLIST_ENTRY(service_instance) list_item;
	struct mdns_service service;
	char sname[MDNS_MAX_LABEL_LEN + 1];
	char stype[MDNS_MAX_LABEL_LEN + 1];
	char domain[MDNS_MAX_LABEL_LEN + 1];
	char keyvals[MDNS_MAX_KEYVAL_LEN + 1];
	char rawkeyvals[MDNS_MAX_KEYVAL_LEN + 1];
	int rawkvlen;
	SLIST_ENTRY(service_instance) alist_item;
	struct arec *arec;
	/* we keep ttls for each record.  Note that these ttls are in milliseconds,
	 * not seconds.  If a received ttl is too big, we simply make it as big as
	 * possible.  Note that we have to regularly refresh the srv and txt
	 * records, so we keep enough details around to refresh it properly.  In
	 * practice, the txt and srv will probably have the same ttl, so we just
	 * refresh with T_ANY when the srv expires.
	 */
	uint32_t ptr_ttl;
	uint32_t srv_ttl;
	uint32_t srv_ttl0;
	uint32_t next_refresh;
	int ttl_percent;
	enum sinst_state state;
	struct service_monitor *smon;
};

/* perform callback for service instance s and status code c */
#define DO_CALLBACK(s, c) do { \
	if ((s)->smon->cb((s)->smon->cbdata, &(s)->service, (c)) != MDNS_SUCCESS) \
		LOG("Warning: callback returned unexpected value.\n"); \
	} while (0)

static struct sinst_list sinsts_free;
static struct service_instance sinsts[MDNS_SERVICE_CACHE_SIZE];

/* We maintain a linked list of services that we are monitoring. */
struct service_monitor {
	SLIST_ENTRY(service_monitor) list_item;
	struct sinst_list sinsts;

	uint8_t fqst[MDNS_MAX_NAME_LEN + 1];
	/* save memory by adding a dname pointer to the fqst and fqsn instead of
	 * copying them over and over.  These offsets can be used while
	 * constructing packets, but aren't expected to be relevant in any other
	 * context.
	 */
	uint16_t fqst_offset;
	uint16_t fqsn_offset;
	mdns_query_cb cb;
	void *cbdata;

	/* this is the time in milliseconds between service refreshes.  It starts
	 * at 1s and doubles after each refresh until it reaches 60s, where it
	 * stays.
	 */
	uint32_t refresh_period;

	/* this is the time until the next refresh in ms.  It gets updated every
	 * time we wake up to do anything.  If it drops to 0, we refresh the
	 * service.
	 */
	uint32_t next_refresh;
};

SLIST_HEAD(smons_list, service_monitor);
static struct smons_list smons_active;
static struct smons_list smons_free;
static struct service_monitor smons[MDNS_MAX_SERVICE_MONITORS];

/* Commands sent over the control socket must at least have a command (e.g.,
 * MDNS_CTRL_HALT, etc.) and a length.  Commands that send data (e.g.,
 * MDNS_CTRL_MONITOR, etc.) will also have a data member.  Note that the length
 * is the total length of the command, not the length of the data.
 */
union query_ctrl_data {
	char raw[0];
	struct service_monitor smon;
	uint8_t fqst[MDNS_MAX_NAME_LEN + 1];
};

struct query_ctrl_msg {
	int command;
	int length;
	union query_ctrl_data data;
};

#define CTRL_MSG_MIN (sizeof(struct query_ctrl_msg) - \
					  sizeof(union query_ctrl_data))

static struct query_ctrl_msg ctrl_msg;

#define SUBTRACT(a, b) ((a) > (b) ? (a) - (b) : 0)
#define ADD(a, b) ((a) > (UINT32_MAX - (b)) ? UINT32_MAX : (a) + (b))
#define CONVERT_TTL(ttl) ((ttl) > UINT32_MAX/1000 ? UINT32_MAX : (ttl) * 1000)
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* Send a control message msg to the server listening at localhost:port.
 * Expect an int response code from the server.
 */
int query_send_ctrl_msg(struct query_ctrl_msg *msg, uint16_t port)
{
	int ret;
	struct sockaddr_in to;
	int s;
	fd_set fds;
	struct timeval t;
	int status;

	s = mdns_socket_loopback(htons(port), 0);
	if (s < 0) {
		LOG("error: failed to create loopback socket\n");
		return -1;
	}

	memset((char *)&to, 0, sizeof(to));
	to.sin_family = PF_INET;
	to.sin_port = htons(port);
	to.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = sendto(s, msg, msg->length, 0, (struct sockaddr *)&to, sizeof(to));
	if (ret == -1)
	{
		LOG("error: failed to send control message: %d\n", errno);
		ret = MDNS_NORESP;
		goto done;
	}

	FD_ZERO(&fds);
	FD_SET(s, &fds);
	SET_TIMEOUT(&t, 2000);
	ret = select(s + 1, &fds, NULL, NULL, &t);
	if (ret == -1)
	{
		LOG("error: select failed for control socket: %d\n", errno);
		ret = MDNS_NORESP;
		goto done;
	}

	if (!FD_ISSET(s, &fds)) {
		LOG("error: no control response received.\n");
		ret = MDNS_NORESP;
		goto done;
	}

	ret = recvfrom(s, &status, sizeof(status), 0, NULL, 0);
	if (ret == -1)
	{
		LOG("error: failed to recv response to control message: %d\n", errno);
		ret = MDNS_NORESP;
		goto done;
	}
	ret = status;

done:
	mdns_socket_close(s);
	return ret;
}

/* find the service monitor for the specified fqst, or NULL if we're not
 * monitoring that service.  Note that the message m can be NULL if the fqst is
 * known to not contain pointers
 */
static struct service_monitor *find_service_monitor(struct mdns_message *m,
													uint8_t *fqst)
{
	struct service_monitor *found = NULL;

	SLIST_FOREACH(found, &smons_active, list_item) {
		if (dname_cmp(NULL, found->fqst, m->data, fqst) == 0)
			return found;
	}
	return NULL;
}

/* add a service monitor to our list of monitored services, or fail
 * appropriately
 */
static int add_service(struct service_monitor *smon)
{
	struct service_monitor *found = NULL;

	DBG("Adding service ");
	debug_print_name(NULL, smon->fqst);
	DBG(" to monitor list\n");

	/* ensure that we're not already monitoring this service */
	found = find_service_monitor(NULL, smon->fqst);
	if (found != NULL) {
		DBG("error: already monitoring this service\n");
		return MDNS_INUSE;
	}

	found = SLIST_FIRST(&smons_free);
	if (found == NULL) {
		DBG("error: no more service monitor slots available.\n");
		return MDNS_NOMEM;
	}
	SLIST_REMOVE_HEAD(&smons_free, list_item);

	memcpy(found, smon, sizeof(struct service_monitor));
	SLIST_INIT(&found->sinsts);
	found->refresh_period = 1000; /* start the refresh period at 1s. */
	found->next_refresh = 0;
	SLIST_INSERT_HEAD(&smons_active, found, list_item);

	return MDNS_SUCCESS;
}

/* cleanup the sinst and put it back on the free list.  Caller is responsible
 * for notifying user if necessary.  Note that this function fixes up the arec
 * list too.
 */
static void cleanup_sinst(struct service_instance *sinst)
{
	SLIST_REMOVE(&sinst->smon->sinsts, sinst, service_instance, list_item);
	if (sinst->arec) {
		SLIST_REMOVE(&sinst->arec->sinsts, sinst, service_instance,
					 alist_item);
		if (SLIST_EMPTY(&sinst->arec->sinsts)) {
			SLIST_REMOVE(&arecs_active, sinst->arec, arec, list_item);
			SLIST_INSERT_HEAD(&arecs_free, sinst->arec, list_item);
		}
	}
	SLIST_INSERT_HEAD(&sinsts_free, sinst, list_item);
	/* just in case any of our callers wants to know when to refresh this dead
	 * record, make the refresh time never.
	 */
	sinst->next_refresh = UINT32_MAX;
}

/* cleanup a service.  Caller is responsible for removing it from the active
 * list and moving it to the free list
 */
static void cleanup_service(struct service_monitor *smon)
{
	struct service_instance *sinst, *stmp;

	SLIST_FOREACH_SAFE(sinst, &smon->sinsts, list_item, stmp)
		cleanup_sinst(sinst);
}

/* remove the service fqst from the monitor list.  If fqst is an empty string,
 * we unmonitor all services.
 */
static void remove_service(uint8_t *fqst)
{
	struct service_monitor *found;

	if (fqst[0] == 0) {
		DBG("Removing all services from monitor list:\n");
		while (!SLIST_EMPTY(&smons_active)) {
			found = SLIST_FIRST(&smons_active);
			SLIST_REMOVE_HEAD(&smons_active, list_item);
			DBG("\t");
			debug_print_name(NULL, found->fqst);
			DBG("\n");
			cleanup_service(found);
			SLIST_INSERT_HEAD(&smons_free, found, list_item);
		}
		return;
	}

	DBG("Removing service ");
	debug_print_name(NULL, fqst);
	DBG(" from monitor list\n");

	/* find the service of interest */
	found = find_service_monitor(NULL, fqst);
	if (found == NULL) {
		DBG("Warning: service was not being monitored\n");
		return;
	}
	SLIST_REMOVE(&smons_active, found, service_monitor, list_item);
	cleanup_service(found);
	SLIST_INSERT_HEAD(&smons_free, found, list_item);
}

/* find the service instance with the specified fqsn in the list of a
 * particular service monitor.  Return the service instance, or NULL if it was
 * not found.
 */
static struct service_instance *find_service_instance(struct service_monitor *smon,
													  struct mdns_message *m,
													  uint8_t *fqsn)
{
	struct service_instance *found = NULL;

	SLIST_FOREACH(found, &smon->sinsts, list_item) {
		if (dname_cmp(NULL, found->service.fqsn, m->data, fqsn) == 0)
			return found;
	}
	return NULL;
}

static void reset_service_instance(struct service_instance *sinst)
{
	memset(sinst, 0, sizeof(struct service_instance));
	sinst->service.servname = sinst->sname;
	sinst->service.servtype = sinst->stype;
	sinst->service.domain = sinst->domain;

	/* start the ttl off at 1s.  If we actully have a suitable SRV in the
	 * current message, we'll update it.  Otherwise, we'll be all set up for
	 * an immediate refresh attempt.
	 */
	sinst->srv_ttl0 = 1000;
	sinst->srv_ttl = 1000;
	sinst->next_refresh = 0;
	sinst->ttl_percent = 20;
}

/* populate the service s with the bits and pieces in the fqsn from message
 * m.  Return 0 on success or -1 on failure.
 */
static int copy_servinfo(struct mdns_service *s, struct mdns_message *m,
						 uint8_t *fqsn)
{
	int ret;

	ret = dname_copy(s->fqsn, m->data, fqsn);
	if (ret == -1)
		return ret;

	fqsn = dname_label_to_c(s->servname, m->data, fqsn, 1);
	if (fqsn == NULL)
		return -1;

	fqsn = dname_label_to_c(s->servtype, m->data, fqsn, 0);
	if (fqsn == NULL)
		return -1;

	if (dname_label_cmp(NULL, (uint8_t *)"\4_tcp", m->data, fqsn) == 0)
		s->proto = MDNS_PROTO_TCP;
	else if (dname_label_cmp(NULL, (uint8_t *)"\4_udp", m->data, fqsn) == 0)
		s->proto = MDNS_PROTO_UDP;
	else {
		LOG("Failed to parse protocol.\n");
		return -1;
	}
	fqsn = dname_label_next(m->data, fqsn);
	if (fqsn == NULL)
		return -1;

	/* NOTE: this means that our domain must be a single label like .local! */
	fqsn = dname_label_to_c(s->domain, m->data, fqsn, 0);
	if (fqsn == NULL)
		return -1;

	return 0;
}

/* the TXT record r goes with the service instance sinst.  Update sinst if
 * necessary and set the dirty bit.
 */
static int update_txt(struct service_instance *sinst, struct mdns_resource *r)
{
	int len, ret = 0, rlen;

	len = r->rdlength < sinst->rawkvlen ? r->rdlength : sinst->rawkvlen;
	rlen = r->rdlength;

	/* is this an update? */
	if (sinst->service.keyvals == NULL ||
		(rlen != sinst->rawkvlen && rlen < MDNS_MAX_KEYVAL_LEN + 1) ||
		memcmp(r->rdata, sinst->rawkeyvals, len) != 0) {
		len = r->rdlength < MDNS_MAX_KEYVAL_LEN + 1 ?
			r->rdlength : MDNS_MAX_KEYVAL_LEN + 1;
		memcpy(sinst->rawkeyvals, r->rdata, len);
		sinst->rawkvlen = len;
		sinst->service.keyvals = sinst->keyvals;
		txt_to_c_ncpy(sinst->keyvals, sizeof(sinst->keyvals),
					  r->rdata, r->rdlength);
		ret = 1;
	}

	return ret;
}

/* find the a record with the specified fqdn in the list of a active a records.
 * Return the arec, or NULL if it was not found.
 */
static struct arec *find_arec(struct mdns_message *m, uint8_t *fqdn)
{
	struct arec *found = NULL;

	SLIST_FOREACH(found, &arecs_active, list_item) {
		if (dname_cmp(NULL, found->fqdn, m->data, fqdn) == 0)
			return found;
	}
	return NULL;
}

/* set the sinst's arec to arec, and deal with any list coherency.  Return 0 if
 * we didn't have to update anything, or 1 otherwise.
 */
static int set_arec(struct service_instance *sinst, struct arec *arec)
{
	int ret = 0;

	if (sinst->arec != NULL) {
		/* We have an existing arec.  Take us off it's list. */
		SLIST_REMOVE(&sinst->arec->sinsts, sinst, service_instance,
					 alist_item);
		if (sinst->arec != arec)
			ret = 1;
	}

	if (arec->state == AREC_STATE_RESOLVED) {
		if (sinst->service.ipaddr != arec->ipaddr) {
			sinst->service.ipaddr = arec->ipaddr;
			ret = 1;
		}
		sinst->service.flags |= SERVICE_HAS_A_FLAG;
	} else {
		sinst->service.flags &= ~SERVICE_HAS_A_FLAG;
	}

	/* We have an arec, and we're not on any other arec's list */
	SLIST_INSERT_HEAD(&arec->sinsts, sinst, alist_item);
	sinst->arec = arec;
	return ret;
}

/* the SRV record r from the mdns_message m goes with the service instance
 * sinst.  Update sinst if necessary.  Also, start an A record if there isn't
 * already one.  Return 1 if we updated the sinst.
 */
static int update_srv(struct service_instance *sinst, struct mdns_message *m,
					  struct mdns_resource *r, uint32_t elapsed)
{
	struct rr_srv *srv = (struct rr_srv *)r->rdata;
	struct mdns_service *s = &sinst->service;
	struct arec *arec;
	int ret = 0;

	if (r->ttl == 0) {
		cleanup_sinst(sinst);
		return -1;
	}

	/* we found a service record for this sinst. */
	if (s->port != srv->port) {
		s->port = srv->port;
		ret = 1;
	}

	/* Now we have to make sure our service instance has an a record */
	arec = find_arec(m, srv->target);
	if (arec == NULL) {
		/* there's no a record for this service.  Get one. */
		arec = SLIST_FIRST(&arecs_free);
		if (arec == NULL) {
			/* the arec cache is full.  Let the user deal with it */
			sinst->arec = NULL;
			goto done;
		}

		SLIST_REMOVE_HEAD(&arecs_free, list_item);
		memset(arec, 0, sizeof(struct arec));
		SLIST_INSERT_HEAD(&arecs_active, arec, list_item);
		dname_copy(arec->fqdn, m->data, srv->target);
	}
	ret += set_arec(sinst, arec);

done:
	/* finally, update the ttl and flags.  We augment the ttl by the elapsed
	 * time so we can do a wholesale inspection and update later.
	 */
	sinst->srv_ttl = ADD(CONVERT_TTL(r->ttl), elapsed);
	sinst->srv_ttl0 = CONVERT_TTL(r->ttl);
	sinst->ttl_percent = 20;
	sinst->next_refresh = ADD(sinst->srv_ttl0 * 80 / 100, elapsed);
	sinst->service.flags |= SERVICE_HAS_SRV_FLAG;
	return ret > 0 ? 1 : 0;
}

/* apply the elapsed time to the service instance.  If it's time to refresh,
 * add a suitable question to the message m and return 1.  If it's time to give
 * up on this record, return -1.  If there's nothing to do at this time, return
 * 0.
 */
static int apply_elapsed(struct service_instance *sinst, uint32_t elapsed,
						 struct mdns_message *m)
{
	/* start by updating all of the ttls */
	sinst->ptr_ttl = SUBTRACT(sinst->ptr_ttl, elapsed);
	sinst->srv_ttl = SUBTRACT(sinst->srv_ttl, elapsed);
	sinst->next_refresh = SUBTRACT(sinst->next_refresh, elapsed);

	if (sinst->next_refresh == 0) {
		/* time to refresh */
		if (sinst->ttl_percent < 5) {
			DBG("Timed out resolving SRV record for ");
			debug_print_name(NULL, sinst->service.fqsn);
			DBG(".  Evicting.\n");
			cleanup_sinst(sinst);
			return -1;
		}

		if (sinst->ttl_percent == 20) {
			/* this is the first refresh attempt.  Send it, and schedule the
			 * next one for 80% of the ttl.
			 */
			sinst->next_refresh = sinst->srv_ttl0 * 80 / 100;
		} else {
			sinst->next_refresh = sinst->srv_ttl0 * sinst->ttl_percent / 100;
		}
		DBG("Refreshing SRV record for ");
		debug_print_name(NULL, sinst->service.fqsn);
		DBG(".\n");
		sinst->ttl_percent >>= 1;
		if (mdns_add_question(m, sinst->service.fqsn, T_ANY, C_IN) != 0) {
			LOG("Warning: failed to add query for SRV record.\n");
			return 0;
		} else {
			return 1;
		}
	}
	return 0;
}

/* march the sinst through its state machine subject to the event.  The sinst
 * must at least be initialized with a fqsn.  The arguments are variously
 * required depending on the event.
 */
static int update_sinst(struct service_instance *sinst, enum sinst_event e,
						struct mdns_message *m, struct arec *arec,
						struct mdns_resource *srvrr,
						struct mdns_resource *txt, uint32_t elapsed)
{
	struct mdns_service *s = &sinst->service;
	int changes = 0, ret;

	/* apply state-independent updates */
	if (e == SINST_EVENT_GOT_SRV)
		changes = update_srv(sinst, m, srvrr, elapsed);
	else if (e == SINST_EVENT_GOT_TXT)
		changes = update_txt(sinst, txt);
	else if (e == SINST_EVENT_GOT_AREC)
		changes = set_arec(sinst, arec);

	/* now decide if we change state */
	switch (sinst->state) {
	case SINST_STATE_INIT:
		if (e == SINST_EVENT_LOST_AREC) {
			/* if we lose a record, we just abandon ship.  We haven't alerted
			 * the user yet, so no need to send a DISAPPEAR message.
			 */
			cleanup_sinst(sinst);
			break;
		}

		if (e == SINST_EVENT_ADD_QUESTIONS) {
			ret = apply_elapsed(sinst, elapsed, m);
			changes = ret == -1 ? 0 : ret;
			break;
		}

		if (changes <= 0)
			break;

		if (SERVICE_IS_READY(s)) {
			DO_CALLBACK(sinst, MDNS_DISCOVERED);
			sinst->state = SINST_STATE_CLEAN;
		}
		break;

	case SINST_STATE_CLEAN:
		if (e == SINST_EVENT_LOST_AREC) {
			cleanup_sinst(sinst);
			DO_CALLBACK(sinst, MDNS_DISAPPEARED);
			break;
		}

		if (e == SINST_EVENT_ADD_QUESTIONS) {
			ret = apply_elapsed(sinst, elapsed, m);
			changes = 0;
			if (ret == -1)
				DO_CALLBACK(sinst, MDNS_DISAPPEARED);
			else if (ret == 1) {
				s->flags &= ~SERVICE_HAS_SRV_FLAG;
				sinst->state = SINST_STATE_UPDATING;
				changes = 1;
			}
			break;
		}

		if (changes == 0)
			break;

		if (changes == -1) {
			DO_CALLBACK(sinst, MDNS_DISAPPEARED);
			break;
		}

		if (SERVICE_IS_READY(s)) {
			DO_CALLBACK(sinst, MDNS_UPDATED);
		} else {
			sinst->state = SINST_STATE_UPDATING;
		}
		break;

	case SINST_STATE_UPDATING:
		if (e == SINST_EVENT_LOST_AREC) {
			cleanup_sinst(sinst);
			DO_CALLBACK(sinst, MDNS_DISAPPEARED);
			break;
		}

		if (e == SINST_EVENT_ADD_QUESTIONS) {
			ret = apply_elapsed(sinst, elapsed, m);
			changes = ret == -1 ? 0 : ret;
			if (ret == -1)
				DO_CALLBACK(sinst, MDNS_DISAPPEARED);
			break;
		}

		if (changes == 0)
			break;

		if (changes == -1) {
			DO_CALLBACK(sinst, MDNS_DISAPPEARED);
			break;
		}

		if (SERVICE_IS_READY(s)) {
			DO_CALLBACK(sinst, MDNS_UPDATED);
			sinst->state = SINST_STATE_CLEAN;
		}
		break;
	}
	return changes > 0 ? 1 : 0;
}

/* set the ip address of the a record and alert associated sinsts if
 * necessary.
 */
static void set_ip(struct arec *arec, struct mdns_resource *a)
{
	uint32_t ipaddr = get_uint32(a->rdata);
	struct service_instance *sinst;

	if (arec->ipaddr == ipaddr)
		goto done;

	arec->ipaddr = ipaddr;
	SLIST_FOREACH(sinst, &arec->sinsts, alist_item)
		update_sinst(sinst, SINST_EVENT_GOT_AREC, NULL, arec, NULL, NULL, 0);

done:
	arec->ttl = CONVERT_TTL(a->ttl);
	/* next refresh is when 20% of the ttl remains */
	arec->next_refresh = arec->ttl * 80 / 100;
	arec->ttl_percent = 10;
}

/* we've tried and tried to get the A record but we can't.  Evict it from the
 * cache along with any sinsts associated with it.
 */
static void evict_arec(struct arec *arec)
{
	struct service_instance *sinst;

	SLIST_FOREACH(sinst, &arec->sinsts, alist_item)
		update_sinst(sinst, SINST_EVENT_LOST_AREC, NULL, NULL, NULL, NULL, 0);

	/* update_slist should remove the sinst from the arec's list.  And, when
	 * the sinst list is empty, it should move the arec to the free list.
	 * Accordingly, the arec's sinsts list should be empty.
	 */
	ASSERT(SLIST_EMPTY(&arec->sinsts));

	/* just in case any of our callers wants to know when to refresh this dead
	 * record, make the refresh time never.
	 */
	arec->next_refresh = UINT32_MAX;
}

/* This is the arec state machine.  Update the A record considering the event
 * that just happened.  The other arguments may or may not be valid depending
 * on the event.  Alert any associated sinsts if necessary.  Return 1 if we
 * need to send a query, otherwise return 0.
 */
static int update_arec(struct arec *arec, enum arec_event e,
					   struct mdns_message *m, struct mdns_resource *a,
					   uint32_t elapsed)
{
	int ret = 0;

	if (e == AREC_EVENT_RX_REC && a->ttl == 0) {
		evict_arec(arec);
		return 0;
	}

	switch (arec->state) {
	case AREC_STATE_INIT:
		if (e == AREC_EVENT_RX_REC) {
			DBG("Immediately resolved A record for ");
			debug_print_name(NULL, arec->fqdn);
			DBG("\n");
			arec->state = AREC_STATE_RESOLVED;
			set_ip(arec, a);

		} else if (e == AREC_EVENT_ADD_QUESTIONS) {
			/* time to send the first query for this arec */
			DBG("Launching A record query for ");
			debug_print_name(NULL, arec->fqdn);
			DBG("\n");
			if (mdns_add_question(m, arec->fqdn, T_A, C_IN) != 0) {
				LOG("Warning: failed to add query for A record.\n");
			}
			ret = 1;
			arec->state = AREC_STATE_QUERYING;
			/* try to get the A record approximately once per second for 3
			 * seconds until we get it, or fail.  Use the same algorithm as the
			 * steady-state refresh period to simplify the code.  That is,
			 * refresh when 20%, 10%, and 5% of the ttl remains.
			 */
			arec->ttl = 1000;
			arec->next_refresh = arec->ttl * 80 / 100;
			arec->ttl_percent = 10;
		}
		break;

	case AREC_STATE_QUERYING:
		if (e == AREC_EVENT_RX_REC) {
			DBG("Resolved A record for ");
			debug_print_name(NULL, arec->fqdn);
			DBG("\n");
			arec->state = AREC_STATE_RESOLVED;
			set_ip(arec, a);
			DBG("Next update in %d ms.\n", arec->next_refresh);

		} else if (e == AREC_EVENT_ADD_QUESTIONS) {
			/* still no response.  It's been "elapsed" ms since our last
			 * invocation.  Either try again or give up
			 */
			arec->next_refresh = SUBTRACT(arec->next_refresh, elapsed);
			if (arec->next_refresh == 0 && arec->ttl_percent < 5) {
				/* we tried to refresh but failed.  So give up. */
				DBG("Failed to resolve A record for ");
				debug_print_name(NULL, arec->fqdn);
				DBG(".  Evicting.\n");
				evict_arec(arec);
				break;
			}

			if (arec->next_refresh == 0) {
				DBG("Still trying to refresh A record for ");
				debug_print_name(NULL, arec->fqdn);
				DBG("\n");
				if (mdns_add_question(m, arec->fqdn, T_A, C_IN) != 0)
					LOG("Warning: failed to add query for A record.\n");
				ret = 1;
				arec->ttl_percent >>= 1;
				arec->next_refresh = arec->ttl * arec->ttl_percent / 100;
			}
		}
		break;

	case AREC_STATE_RESOLVED:
		if (e == AREC_EVENT_RX_REC) {
			set_ip(arec, a);

		} else if (e == AREC_EVENT_ADD_QUESTIONS) {
			/* We're supposed to attempt to refresh when 20%, 10%, and 5% of
			 * the lifetime of the record remains.
			 */
			arec->next_refresh = SUBTRACT(arec->next_refresh, elapsed);
			if (arec->next_refresh == 0) {
				DBG("Refreshing A record for ");
				debug_print_name(NULL, arec->fqdn);
				DBG("\n");
				if (mdns_add_question(m, arec->fqdn, T_A, C_IN) != 0)
					LOG("Warning: failed to add query for A record.\n");
				ret = 1;
				arec->ttl_percent = 10;
				arec->next_refresh = arec->ttl * arec->ttl_percent / 100;
				arec->state = AREC_STATE_QUERYING;
			}
		}
		break;
	}
	return ret;
}

static struct service_instance overflow_sinst;
static struct arec overflow_arec;

/* we received a packet.  If it matches a service that we're monitoring, update
 * the cache and (if necessary) alert the user.
 */
static int update_service_cache(struct mdns_message *m, uint32_t elapsed)
{
	struct service_monitor *smon = NULL;
	struct mdns_resource *ptr, *r, *rtmp, *a;
	struct service_instance *sinst = NULL;
	struct rr_srv *srv;
	struct arec *arec;

	if (m->header->flags.fields.qr != RESPONSE)
		return 0;

	SLIST_FOREACH(ptr, &m->ptrs, list_item) {
		smon = find_service_monitor(m, ptr->name);
		if (smon == NULL)
			continue;

		/* This PTR record is interesting.  Analyze it. */
		sinst = find_service_instance(smon, m, ptr->rdata);
		if (sinst != NULL) {
			/* sneak a bit of state machine logic in here */
			if (ptr->ttl == 0) {
				if (sinst->state == SINST_STATE_CLEAN ||
					sinst->state == SINST_STATE_UPDATING)
					DO_CALLBACK(sinst, MDNS_DISAPPEARED);
				cleanup_sinst(sinst);
			} else {
				sinst->ptr_ttl = ADD(CONVERT_TTL(ptr->ttl), elapsed);
			}
			continue;
		}

		/* This is a new service instance. */
		sinst = SLIST_FIRST(&sinsts_free);
		if (sinst != NULL) {
			SLIST_REMOVE_HEAD(&sinsts_free, list_item);
			reset_service_instance(sinst);
			SLIST_INSERT_HEAD(&smon->sinsts, sinst, list_item);
			sinst->smon = smon;
			if (copy_servinfo(&sinst->service, m, ptr->rdata) == -1) {
				LOG("Warning: failed to copy service info.\n");
				SLIST_REMOVE_HEAD(&smon->sinsts, list_item);
				SLIST_INSERT_HEAD(&sinsts_free, sinst, list_item);
				continue;
			}
			sinst->ptr_ttl = ADD(CONVERT_TTL(ptr->ttl), elapsed);
			continue;
		}

		/* If we get here, it means that we don't have any room in our cache.
		 * So make a best effort here using the overflow data structures by
		 * collecting all of the relevant records, and alerting the user if
		 * this is an overflow item.
		 */
		sinst = &overflow_sinst;
		reset_service_instance(sinst);
		sinst->smon = smon;
		if (copy_servinfo(&sinst->service, m, ptr->rdata) == -1) {
			LOG("Warning: failed to copy service info.\n");
			continue;
		}

		SLIST_FOREACH_SAFE(r, &m->txts, list_item, rtmp) {
			if (dname_cmp(m->data, r->name, NULL, sinst->service.fqsn) == 0) {
				update_txt(sinst, r);
				SLIST_REMOVE(&m->txts, r, mdns_resource, list_item);
			}
		}

		SLIST_FOREACH_SAFE(r, &m->srvs, list_item, rtmp) {
			if (dname_cmp(m->data, r->name, NULL, sinst->service.fqsn) != 0)
				continue;

			srv = (struct rr_srv *)r->rdata;
			sinst->service.port = srv->port;
			memset(&overflow_arec, 0, sizeof(struct arec));
			sinst->arec = &overflow_arec;
			dname_copy(overflow_arec.fqdn, m->data, srv->target);
			SLIST_REMOVE(&m->srvs, r, mdns_resource, list_item);
		}

		if (sinst->arec) {
			SLIST_FOREACH(a, &m->as, list_item) {
				if (dname_cmp(m->data, a->name, NULL, sinst->arec->fqdn) != 0)
					continue;
				sinst->service.ipaddr = get_uint32(a->rdata);
				break;
			}
		}

		DO_CALLBACK(sinst, MDNS_CACHE_FULL);
	}

	/* Any SRV, TXT, or A records that remain in the message are either not of
	 * interest, or are related to existing service instances.  These appear
	 * for a few reasons.  Like if we previously got a message with a PTR of
	 * interest but no SRV/TXT, or we got an SRV of interest but no A.  Do the
	 * TXT and SRV records first.  This will ensure that if there is an A
	 * record in the list that goes with the SRV, it will be resolved right
	 * away.
	 */
	SLIST_FOREACH(r, &m->txts, list_item) {
		smon = find_service_monitor(m, dname_label_next(m->data, r->name));
		if (smon == NULL)
			continue;
		sinst = find_service_instance(smon, m, r->name);
		if (sinst == NULL)
			continue;
		update_sinst(sinst, SINST_EVENT_GOT_TXT, m, NULL, NULL, r, elapsed);
	}

	SLIST_FOREACH(r, &m->srvs, list_item) {
		smon = find_service_monitor(m, dname_label_next(m->data, r->name));
		if (smon == NULL)
			continue;
		sinst = find_service_instance(smon, m, r->name);
		if (sinst == NULL)
			continue;
		update_sinst(sinst, SINST_EVENT_GOT_SRV, m, NULL, r, NULL, elapsed);
	}

	SLIST_FOREACH(a, &m->as, list_item) {
		arec = find_arec(m, a->name);
		if (arec == NULL)
			continue;
		update_arec(arec, AREC_EVENT_RX_REC, m, a, elapsed);
	}

	return 0;
}

/* calculate the size of the answer that contains the ptr record of a service
 * instance.
 */
static int ptr_size(struct service_instance *sinst)
{
	/* our ptr contains an answer, an offset to the fqst, a service label, the
	 * length of the service label, and another offset to the fqst, and the len
	 * of the ptr data.
	 */
	uint16_t len = 2*sizeof(uint16_t) + sizeof(uint32_t); /* answer */
	len += 2*sizeof(uint16_t); /* offsets to fqst */
	len += strlen(sinst->service.servname) + 1; /* service label, its len */
	len += sizeof(uint16_t); /* len of ptr data */
	return len;
}

/* add the known answers from smon to the message m.  Return 0 if everything
 * went fine, or -1 if the answers didn't all fit.  Also take this opportunity
 * to update the ttls of the records.
 */
static int add_known_answers(struct mdns_message *m,
							 struct service_monitor *smon, uint32_t elapsed)
{

	struct service_instance *sinst;
	int len;

	SLIST_FOREACH(sinst, &smon->sinsts, list_item) {

		/* first add the PTR record for the service. */
		if (sinst->ptr_ttl == 0)
			continue;

		len = ptr_size(sinst);
		if (len > TAILROOM(m))
			goto fail;

		mdns_add_answer_o(m, smon->fqst_offset, T_PTR, C_IN,
						  sinst->ptr_ttl/1000);
		smon->fqsn_offset = m->cur - m->data;
		mdns_add_name_lo(m, (uint8_t *)sinst->service.servname,
						 smon->fqst_offset);
	}
	return 0;

fail:
	LOG("Warning: all known answers didn't fit in packet\n");
	return -1;
}

/* given the elapsed time in ms since the last activity, prepare a query in the
 * message m.  Return -1 for error, 0 for no packet to send, and 1 to send the
 * packet.  Also update the next_event pointer with milliseconds until the next
 * event, or to UINT32_MAX if no scheduling is necessary.
 */
static int prepare_query(struct mdns_message *m, uint32_t elapsed,
						 uint32_t *next_event)
{
	struct service_monitor *smon;
	struct service_instance *sinst, *stmp;

	int ret = 0;
	struct arec *arec, *atmp;

	*next_event = UINT32_MAX;

	if (mdns_query_init(m) != 0)
		return -1;

	/* add questions for any unresolved A records. */
	SLIST_FOREACH_SAFE(arec, &arecs_active, list_item, atmp) {
		ret += update_arec(arec, AREC_EVENT_ADD_QUESTIONS, m, NULL, elapsed);
		*next_event = MIN(*next_event, arec->next_refresh);
	}

	/* We make two passes.  One to update all of the refresh times and to
	 * populate the questions section, and another to populated the known
	 * answers section
	 */

	SLIST_FOREACH(smon, &smons_active, list_item) {

		if (smon->next_refresh <= elapsed) {
			/* double the refresh period, topping out at 60s, and add a
			 * suitable question
			 */
			smon->next_refresh = 0;
			smon->refresh_period = smon->refresh_period > 30000 ?
				60000 : smon->refresh_period * 2;
			smon->fqst_offset = m->cur - m->data;
			if (mdns_add_question(m, smon->fqst, T_ANY, C_IN) != 0) {
				LOG("ERROR: failed to populate questions!\n");
				return -1;
			}
			DBG("Added query for service ");
			debug_print_name(NULL, smon->fqst);
			DBG("\n");
			ret += 1;
		} else {
			smon->next_refresh = SUBTRACT(smon->next_refresh, elapsed);
			*next_event = MIN(*next_event, smon->next_refresh);
		}

		SLIST_FOREACH_SAFE(sinst, &smon->sinsts, list_item, stmp) {
			ret += update_sinst(sinst, SINST_EVENT_ADD_QUESTIONS, m, NULL,
								NULL, NULL, elapsed);
			*next_event = MIN(*next_event, sinst->next_refresh);
		}
	}

	if (ret == 0)
		return ret;

	/* Okay.  We've added all of the questions that we want to ask.  Now we
	 * populate the known-answers section.  We know which service instances to
	 * add to the known answers because their service monitors have a
	 * next_refresh time of 0.
	 *
	 * NOTE: we don't support the TC bit.  So we only put as many answers as
	 * will fit into a single packet.  This may generate unnecessary network
	 * traffic by inducing responses with answers that actually are known.
	 */
	SLIST_FOREACH(smon, &smons_active, list_item) {
		if (smon->next_refresh == 0) {
			add_known_answers(m, smon, elapsed);
			smon->next_refresh = smon->refresh_period;
			*next_event = MIN(*next_event, smon->next_refresh);
		}
	}
	return ret > 0 ? 1 : 0;
}

/* Main query thread */
static void do_querier(void *data)
{
	int max_sock;
	struct sockaddr_in from;
	int active_fds;
	fd_set fds;
	int ret = 0;
	struct timeval *timeout = NULL, tv;
	socklen_t in_size;
	uint32_t start_wait, stop_wait, sleep_time, next_event;
	int status, len;

	query_enabled = 1;

	LOG("do_querier() launched\n");
	while (query_enabled) {
		FD_ZERO(&fds);
		FD_SET(ctrl_sock, &fds);
		FD_SET(mc_sock, &fds);
		max_sock = ctrl_sock > mc_sock ? ctrl_sock : mc_sock;

		start_wait = mdns_time_ms();
		active_fds = select(max_sock + 1, &fds, NULL, NULL, timeout);
		stop_wait = mdns_time_ms();
		/* round up to prevent ttls of 1ms from never expiring */
		sleep_time = ADD(interval(start_wait, stop_wait), 1);

		if (active_fds < 0 && errno != EINTR)
			LOG("error: select() failed: %d\n", errno);

		/* handlde control events
		 */
		if (FD_ISSET(ctrl_sock, &fds)) {
			DBG("Querier got control message.\n");
			in_size = sizeof(struct sockaddr_in);
			ret = recvfrom(ctrl_sock, &ctrl_msg, sizeof(ctrl_msg), MSG_DONTWAIT,
						   (struct sockaddr *)&from, &in_size);
			/* we at least need a command and length */
			if (ret == -1 || ret < CTRL_MSG_MIN || ret != ctrl_msg.length) {
				LOG("Warning: querier failed to get control message\n");
				status = MDNS_INVAL;
			} else {
				if (ctrl_msg.command == MDNS_CTRL_HALT) {
					LOG("Querier done.\n");
					query_enabled = 0;
					status = MDNS_SUCCESS;
				} else if (ctrl_msg.command == MDNS_CTRL_MONITOR) {
					status = add_service(&ctrl_msg.data.smon);
				} else if (ctrl_msg.command == MDNS_CTRL_UNMONITOR) {
					remove_service(ctrl_msg.data.fqst);
					status = MDNS_SUCCESS;
				} else {
					LOG("Unkown control message %d\n", ctrl_msg.command);
					status = MDNS_NOIMPL;
				}
			}
			/* send status back */
			ret = sendto(ctrl_sock, &status, sizeof(status), 0,
						 (struct sockaddr *)&from, sizeof(from));
			if (ret == -1)
				LOG("error: failed to send control status: %d\n", errno);
		}

		if (FD_ISSET(mc_sock, &fds)) {
			LOG("querier got message\n");
			in_size = sizeof(struct sockaddr_in);
			len = recvfrom(mc_sock, rx_msg.data, sizeof(rx_msg.data),
						   MSG_DONTWAIT, (struct sockaddr*)&from, &in_size);
			if (len < 0) {
				LOG("querier failed to recv packet: %d\n", errno);
				continue;
			}
			ret = mdns_parse_message(&rx_msg, len);
			if (ret == 0)
				update_service_cache(&rx_msg, sleep_time);
		}
		DBG("Preparing next query\n");
		ret = prepare_query(&tx_msg, sleep_time, &next_event);
		if (ret == 1)
			mdns_send_msg(&tx_msg, mc_sock, htons(5353));

		if (next_event == UINT32_MAX) {
			DBG("No event scheduled\n");
			timeout = NULL;
		} else {
			DBG("Next event in %d ms\n", next_event);
			SET_TIMEOUT(&tv, next_event);
			timeout = &tv;
		}
	}

	/* some targets don't like it if we just bail on a thread without
	 * yeilding
	 */
	mdns_thread_yield();
}

/* Launch the query thread */
int query_launch(void)
{
	int i;

	/* Initially, all service monitors are on the free list */
	SLIST_INIT(&smons_active);
	SLIST_INIT(&smons_free);

	for (i = 0; i < MDNS_MAX_SERVICE_MONITORS; i++) {
		SLIST_INIT(&smons[i].sinsts);
		SLIST_INSERT_HEAD(&smons_free, &smons[i], list_item);
	}

	/* Initially, all service instances are in the free list */
	SLIST_INIT(&sinsts_free);
	for (i = 0; i < MDNS_SERVICE_CACHE_SIZE; i++) {
		reset_service_instance(&sinsts[i]);
		SLIST_INSERT_HEAD(&sinsts_free, &sinsts[i], list_item);
	}

	/* Initially, all arec instances are in the free list */
	SLIST_INIT(&arecs_active);
	SLIST_INIT(&arecs_free);
	for (i = 0; i < MDNS_SERVICE_CACHE_SIZE; i++) {
		memset(&arecs[i], 0, sizeof(struct arec));
		SLIST_INSERT_HEAD(&arecs_free, &arecs[i], list_item);
	}

	/* create both ends of the control socket */
	ctrl_sock = mdns_socket_loopback(htons(MDNS_CTRL_QUERIER), 1);
	if (ctrl_sock < 0) {
		LOG("Failed to create query control socket: %d\n", ctrl_sock);
		return -1;
	}

	mc_sock = mdns_socket_mcast(inet_addr("224.0.0.251"), htons(5353));
	if (mc_sock < 0) {
		LOG("error: unable to open multicast socket in querier\n");
		return mc_sock;
	}

	query_thread = mdns_thread_create(do_querier, NULL);
	if (query_thread == NULL)
		return -1;

	return 0;
}

/* Send the halt command to the query thread */
void query_halt(void)
{
	int ret;
	/* no need to waste stack space that we don't need */
	char buf[CTRL_MSG_MIN];
	struct query_ctrl_msg *ctrl_msg = (struct query_ctrl_msg *)buf;

	ctrl_msg->command = MDNS_CTRL_HALT;
	ctrl_msg->length = sizeof(buf);

	ret = query_send_ctrl_msg(ctrl_msg, MDNS_CTRL_QUERIER);
	if (ret != 0) {
		LOG("Warning: failed to send HALT message to querier: %d\n", ret);
	} else {
		mdns_thread_yield();
	}
	if (query_enabled != 0)
		LOG("Warning: failed to halt querier.  Forcing.\n");

	mdns_thread_delete(query_thread);
	mdns_socket_close(ctrl_sock);
	mdns_socket_close(mc_sock);
	query_enabled = 0;
}

int mdns_query_monitor(char *fqst, mdns_query_cb cb, void *data)
{
	struct query_ctrl_msg ctrl_msg;
	struct service_monitor *m = &ctrl_msg.data.smon;
	int ret;

	if (cb == NULL) {
		LOG("mdns query callback must not be NULL\n");
		return MDNS_INVAL;
	}
	m->cb = cb;
	m->cbdata = data;

	ret = strlen(fqst);
	if (ret > MDNS_MAX_NAME_LEN)
		return MDNS_INVAL;

	ret = dnameify(fqst, '.', m->fqst);
	if (ret == -1) {
		LOG("Failed to parse fully-qualified service type\n");
		return MDNS_INVAL;
	}
	/* dnameify doesn't null-terminate by design.  But we want to use
	 * dname_cmp, etc. on the fqst.  So we null terminate here.
	 */
	m->fqst[ret] = 0;

	/* Why don't we just populate a free smon instead of copying one all over
	 * the place and sending a control message?  The reason is that the mdns
	 * thread owns the smons, and we don't want to have to use mutexes or other
	 * locks at runtime
	 */
	ctrl_msg.command = MDNS_CTRL_MONITOR;
	ctrl_msg.length = sizeof(struct query_ctrl_msg);
	return query_send_ctrl_msg(&ctrl_msg, MDNS_CTRL_QUERIER);
}

void mdns_query_unmonitor(char *fqst)
{
	struct query_ctrl_msg ctrl_msg;
	int ret;

	if (fqst == NULL)
		ret = 0;

	else {
		ret = strlen(fqst);
		if (ret > MDNS_MAX_NAME_LEN) {
			LOG("error: invalid service type\n");
			return;
		}
		ret = dnameify(fqst, '.', ctrl_msg.data.fqst);
		if (ret == -1) {
			LOG("Failed to parse fully-qualified service type\n");
			return;
		}
	}
	ctrl_msg.data.fqst[ret] = 0;

	ctrl_msg.command = MDNS_CTRL_UNMONITOR;
	ctrl_msg.length = sizeof(struct query_ctrl_msg);
	query_send_ctrl_msg(&ctrl_msg, MDNS_CTRL_QUERIER);
	return;
}

#else

int query_launch(void)
{
	/* not implmented, but we transparently drop through here so the
	 * querier can be "started" by mdns_launch
	 */
	return 0;
}

void query_halt(void)
{
	/* not implmented, but we transparently drop through here so the
	 * querier can be halted by mdns_halt
	 */
}

int mdns_query_monitor(char *fqst, mdns_query_cb cb, void *data)
{
	return MDNS_NOIMPL;
}

void mdns_query_unmonitor(char *fqst)
{
	return;
}

#endif
