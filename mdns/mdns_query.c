/*
 * Copyright (C) cozybit, Inc. 2007-2010. All Rights Reserved.
 *
 * Use and redistribution subject to licensing terms.
 */
#include "mdns.h"
#include "mdns_private.h"
#include "mdns_port.h"

#ifdef MDNS_QUERY_API

/* global mdns state */
static void *query_thread;
static int ctrl_sock;
static int query_enabled;
static int mc_sock;
static struct mdns_message tx_msg;
static struct mdns_message rx_msg;

/* Each service instance that we detect is stored in a service instance */
struct service_instance {
	struct mdns_list_item list_item;
	struct mdns_service service;
	char sname[MDNS_MAX_LABEL_LEN + 1];
	char stype[MDNS_MAX_LABEL_LEN + 1];
	char domain[MDNS_MAX_LABEL_LEN + 1];
	char keyvals[MDNS_MAX_KEYVAL_LEN + 1];
	char fqdn[MDNS_MAX_NAME_LEN + 1];
	/* we keep ttls for each record.  Note that these ttls are in milliseconds,
	 * not seconds.  If a received ttl is too big, we simply make it as big as
	 * possible.
	 */
	uint32_t ptr_ttl;
	uint32_t srv_ttl;
	uint32_t txt_ttl;
};

static struct mdns_list sinsts_free;
static struct service_instance sinsts[MDNS_SERVICE_CACHE_SIZE];

/* We maintain a linked list of services that we are monitoring. */
struct service_monitor {
	struct mdns_list_item list_item;
	struct mdns_list sinsts;
	char fqst[MDNS_MAX_NAME_LEN + 1];
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

static struct mdns_list smons_active;
static struct mdns_list smons_free;
static struct service_monitor smons[MDNS_MAX_SERVICE_MONITORS];

/* Commands sent over the control socket must at least have a command (e.g.,
 * MDNS_CTRL_HALT, etc.) and a length.  Commands that send data (e.g.,
 * MDNS_CTRL_MONITOR, etc.) will also have a data member.  Note that the length
 * is the total length of the command, not the length of the data.
 */
union query_ctrl_data {
	char raw[0];
	struct service_monitor smon;
	char fqst[MDNS_MAX_NAME_LEN + 1];
};

struct query_ctrl_msg {
	int command;
	int length;
	union query_ctrl_data data;
};

#define CTRL_MSG_MIN (sizeof(struct query_ctrl_msg) - \
					  sizeof(union query_ctrl_data))

static struct query_ctrl_msg ctrl_msg;

#define UPDATE_TTL(ttl, elapsed) ((ttl) > (elapsed) ? (ttl) - (elapsed) : 0)
#define CONVERT_TTL(ttl) ((ttl) > UINT32_MAX/1000 ? UINT32_MAX : (ttl) * 1000)

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
													char *fqst)
{
	struct mdns_list_item *cursor = NULL;
	struct service_monitor *found = NULL;

	cursor = mdns_list_next(&smons_active, cursor);
	while (cursor != NULL) {
		found = cursor->data;
		if (dname_cmp(NULL, found->fqst, m->data, fqst) == 0)
			return found;
		cursor = mdns_list_next(&smons_active, cursor);
	}
	return NULL;
}

/* add a service monitor to our list of monitored services, or fail
 * appropriately
 */
static int add_service(struct service_monitor *smon)
{
	struct mdns_list_item *cursor = NULL;
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

	cursor = mdns_list_pop(&smons_free);
	if (cursor == NULL) {
		DBG("error: no more service monitor slots available.\n");
		return MDNS_NOMEM;
	}

	found = cursor->data;
	memcpy(cursor->data, smon, sizeof(struct service_monitor));
	/* preserve the data pointer, which points to self in our case */
	cursor->data = found;
	LIST_INIT(&found->sinsts);
	found->refresh_period = 1000;
	found->next_refresh = 0;
	mdns_list_push(&smons_active, cursor);

	return MDNS_SUCCESS;
}

/* remove the service fqst from the monitor list.  If fqst is an empty string,
 * we unmonitor all services.
 */
void remove_service(char *fqst)
{
	struct mdns_list_item *cursor = NULL;
	struct service_monitor *found = NULL;

	if (fqst[0] == 0) {
		DBG("Removing all services from monitor list:\n");
		cursor = mdns_list_pop(&smons_active);
		while (cursor != NULL) {
			found = cursor->data;
			DBG("\t");
			debug_print_name(NULL, found->fqst);
			DBG("\n");
			mdns_list_push(&smons_free, cursor);
			cursor = mdns_list_pop(&smons_active);
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

	mdns_list_remove(&smons_active, &found->list_item);
	mdns_list_push(&smons_free, &found->list_item);
}

/* update the service instance sinst with any SRV, PTR, A, or TXT information
 * in the message m.  Expect the fqsn of the sinst to be initialized.
 */
void update_service_instance(struct service_instance *sinst,
							 struct mdns_message *m)
{
	int i;
	struct mdns_resource *r;
	struct rr_srv *srv;
	struct mdns_service *s = &sinst->service;

	for(i = 0; i < m->num_answers; i++) {
		r = &m->answers[i];
		if (r->type == T_SRV &&
			dname_cmp(m->data, r->name, NULL, s->fqsn) == 0) {
			srv = (struct rr_srv *)r->rdata;
			if (s->port != srv->port ||
				dname_cmp(m->data, srv->target, NULL, sinst->fqdn) != 0) {
				s->port = srv->port;
				dname_copy(sinst->fqdn, m->data, srv->target);
				sinst->srv_ttl = CONVERT_TTL(r->ttl);
				sinst->service.flags |= SERVICE_IS_DIRTY_FLAG;
				sinst->service.flags |= SERVICE_HAS_SRV_FLAG;
			}
		} else if (r->type == T_TXT &&
				   dname_cmp(m->data, r->name, NULL, s->fqsn) == 0) {
			/* TODO: inspect TXT record, copy it, and update it */
		}
	}

	/* Now there may be an A record in there that we need.  But we must have
	 * the SRV record first.  So we search again.
	 */
	if (!(sinst->service.flags & SERVICE_HAS_SRV_FLAG))
		return;

	for(i = 0; i < m->num_answers; i++) {
		r = &m->answers[i];
		if (r->type == T_A &&
			dname_cmp(m->data, r->name, NULL, sinst->fqdn) == 0 &&
			get_uint32(r->rdata) != s->ipaddr) {
			s->ipaddr = get_uint32(r->rdata);
			sinst->service.flags |= SERVICE_HAS_A_FLAG;
			sinst->service.flags |= SERVICE_IS_DIRTY_FLAG;
		}
	}
}

/* find the service instance with the specified fqsn in the list of a
 * particular service monitor.  Return the service instance, or NULL if it was
 * not found.
 */
static struct service_instance *find_service_instance(struct service_monitor *smon,
													  struct mdns_message *m,
													  char *fqsn)
{
	struct mdns_list_item *cursor = NULL;
	struct service_instance *found = NULL;

	cursor = mdns_list_next(&smon->sinsts, cursor);
	while (cursor != NULL) {
		found = cursor->data;
		if (dname_cmp(NULL, found->service.fqsn, m->data, fqsn) == 0)
			return found;
		cursor = mdns_list_next(&smon->sinsts, cursor);
	}
	return NULL;
}

static void reset_service_instance(struct service_instance *sinst)
{
	sinst->sname[0] = 0;
	sinst->stype[0] = 0;
	sinst->domain[0] = 0;
	sinst->fqdn[0] = 0;
	sinst->service.port = 0;
	sinst->service.keyvals = NULL;
	sinst->service.ipaddr = 0;
	sinst->service.servname = sinst->sname;
	sinst->service.servtype = sinst->stype;
	sinst->service.domain = sinst->domain;
	sinst->service.flags = 0;
}

/* populate the service s with the bits and pieces in the fqsn from message
 * m.  Return 0 on success or -1 on failure.
 */
int copy_servinfo(struct mdns_service *s, struct mdns_message *m, char *fqsn)
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

	if (dname_label_cmp(NULL, "\4_tcp", m->data, fqsn) == 0)
		s->proto = MDNS_PROTO_TCP;
	else if (dname_label_cmp(NULL, "\4_udp", m->data, fqsn) == 0)
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

static struct service_instance overflow_sinst;

/* we received a packet.  If it matches a service that we're monitoring, update
 * the cache and (if necessary) alert the user.
 */
static int update_service_cache(struct mdns_message *m)
{
	struct service_monitor *smon = NULL;
	struct mdns_resource *ptr;
	struct mdns_list_item *cursor;
	struct service_instance *sinst = NULL;
	int status, i, ret;

	if (m->header->flags.fields.qr != RESPONSE)
		return 0;

	/* wouldn't it be nice if we could just walk through the entire list of
	 * answers, generate a list of services, than analyze the list to decide if
	 * we hold on to those services or not?  The problem with this is that we
	 * don't know how many services will arrive in a message, and we don't have
	 * dynamic memory allocation.  Also, we don't know what order the PTR, SRV,
	 * and TXT records will arrive anyway.  So we check all of the PTR records,
	 * then the SRV records and TXT records if necessary.  This is O(n^2) where
	 * n is the number of answers.  Yuck.  Fortunately, we don't expect more
	 * than a few answers per packet.
	 */
	for(i = 0; i < m->num_answers; i++) {
		ptr = &m->answers[i];
		if (ptr->type != T_PTR)
			continue;

		smon = find_service_monitor(m, ptr->name);
		if (smon == NULL)
			continue;

		sinst = find_service_instance(smon, m, ptr->rdata);
		status = MDNS_UPDATED;
		if (sinst == NULL) {
			/* This is a new service instance. */
			status = MDNS_DISCOVERED;
			cursor = mdns_list_pop(&sinsts_free);
			sinst = cursor == NULL ? NULL : cursor->data;
			if (sinst == NULL) {
				/* no more space in cache.  Use the overflow instance. */
				status = MDNS_CACHE_FULL;
				sinst = &overflow_sinst;
			}
			reset_service_instance(sinst);
			sinst->service.flags = SERVICE_IS_DIRTY_FLAG;
			if (copy_servinfo(&sinst->service, m, ptr->rdata) == -1)
				LOG("Warning: failed to copy service info\n");
			sinst->ptr_ttl = CONVERT_TTL(ptr->ttl);
		}
		/* populate the service instance and update its state */
		update_service_instance(sinst, m);
		if (status == MDNS_DISCOVERED)
			mdns_list_push(&smon->sinsts, &sinst->list_item);
		if ((sinst->service.flags & SERVICE_IS_READY) == SERVICE_IS_READY &&
			sinst->service.flags & SERVICE_IS_DIRTY_FLAG) {
			ret = smon->cb(smon->cbdata, &sinst->service, status);
			if (ret != MDNS_SUCCESS)
				LOG("Warning: callback returned unexpected value: %d\n", ret);
			sinst->service.flags &= ~SERVICE_IS_DIRTY_FLAG;
		}
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

	struct mdns_list_item *cursor = NULL;
	struct service_instance *sinst;
	int len;

	cursor = mdns_list_next(&smon->sinsts, cursor);
	while (cursor != NULL) {
		sinst = cursor->data;

		/* first add the PTR record for the service. */
		sinst->ptr_ttl = UPDATE_TTL(sinst->ptr_ttl, elapsed);
		len = ptr_size(sinst);
		if (len > TAILROOM(m))
			goto fail;

		mdns_add_answer_o(m, smon->fqst_offset, T_PTR, C_IN,
						  sinst->ptr_ttl/1000);
		smon->fqsn_offset = m->cur - m->data;
		mdns_add_name_lo(m, sinst->service.servname, smon->fqst_offset);
		cursor = mdns_list_next(&smons->sinsts, cursor);
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
	struct mdns_list_item *cursor = NULL;
	int ret = 0;

	*next_event = UINT32_MAX;

	/* We make two passes.  One to update all of the refresh times and to
	 * populate the questions section, and another to populated the known
	 * answers section
	 */
	if (mdns_query_init(m) != 0)
		return -1;

	cursor = mdns_list_next(&smons_active, cursor);
	while (cursor != NULL) {
		smon = cursor->data;
		DBG("Considering ");
		debug_print_name(NULL, smon->fqst);
		DBG("\n");
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
			ret = 1;
		} else {
			smon->next_refresh -= elapsed;
			if (smon->next_refresh < *next_event)
				*next_event = smon->next_refresh;
		}
		cursor = mdns_list_next(&smons_active, cursor);
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
	cursor = mdns_list_next(&smons_active, cursor);
	while (cursor != NULL) {
		smon = cursor->data;
		if (smon->next_refresh == 0) {
			add_known_answers(m, smon, elapsed);
			smon->next_refresh = smon->refresh_period;
			if (smon->next_refresh < *next_event)
				*next_event = smon->next_refresh;
		}
		cursor = mdns_list_next(&smons_active, cursor);
	}
	return 1;
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
				update_service_cache(&rx_msg);
		}
		sleep_time = interval(start_wait, stop_wait);
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
}

/* Launch the query thread */
int query_launch(void)
{
	int i;

	/* Initially, all service monitors are on the free list */
	LIST_INIT(&smons_active);
	LIST_INIT(&smons_free);
	for (i = 0; i < MDNS_MAX_SERVICE_MONITORS; i++) {
		LIST_INIT(&smons[i].sinsts);
		smons[i].list_item.data = &smons[i];
		mdns_list_push(&smons_free, &smons[i].list_item);
	}

	/* Initially, all service instances are in the free list */
	LIST_INIT(&sinsts_free);
	for (i = 0; i < MDNS_SERVICE_CACHE_SIZE; i++) {
		sinsts[i].list_item.data = &sinsts[i];
		sinsts[i].service.servname = sinsts[i].sname;
		sinsts[i].service.servtype = sinsts[i].stype;
		sinsts[i].service.domain = sinsts[i].domain;
		sinsts[i].service.keyvals = NULL;
		mdns_list_push(&sinsts_free, &sinsts[i].list_item);
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
