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

/* We maintain a linked list of services that we are monitoring. */
struct service_monitor {
	struct mdns_list_item list_item;
	char fqst[MDNS_MAX_NAME_LEN + 1];
	mdns_query_cb cb;
	void *cbdata;
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

/* add a service monitor to our list of monitored services, or fail
 * appropriately
 */
int add_service(struct service_monitor *smon)
{
	struct mdns_list_item *cursor = NULL;
	struct service_monitor *found = NULL;

	DBG("Adding service ");
	debug_print_name(NULL, smon->fqst);
	DBG(" to monitor list\n");

	/* ensure that we're not already monitoring this service */
	cursor = mdns_list_next(&smons_active, cursor);
	while (cursor != NULL) {
		found = cursor->data;
		if (dname_cmp(NULL, found->fqst, NULL, smon->fqst) == 0) {
			DBG("error: already monitoring this service\n");
			return MDNS_INUSE;
		}
		cursor = mdns_list_next(&smons_active, cursor);
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

	cursor = mdns_list_next(&smons_active, cursor);
	while (cursor != NULL) {
		found = cursor->data;
		if (dname_cmp(NULL, found->fqst, NULL, fqst) == 0) {
			mdns_list_remove(&smons_active, cursor);
			mdns_list_push(&smons_free, cursor);
			return;
		}
		cursor = mdns_list_next(&smons_active, cursor);
	}
	DBG("Warning: service was not being monitored\n");
}

/* Main query thread */
static void do_querier(void *data)
{
	int max_sock;
	struct sockaddr_in from;
	int active_fds;
	fd_set fds;
	int ret = 0;
	struct timeval *timeout = NULL;
	socklen_t in_size;
	uint32_t start_wait, stop_wait;
	int status;

	query_enabled = 1;

	LOG("do_querier() launched\n");
	while (query_enabled) {
		FD_ZERO(&fds);
		FD_SET(ctrl_sock, &fds);
		max_sock = ctrl_sock;

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
		smons[i].list_item.data = &smons[i];
		mdns_list_push(&smons_free, &smons[i].list_item);
	}

	/* create both ends of the control socket */
	ctrl_sock = mdns_socket_loopback(htons(MDNS_CTRL_QUERIER), 1);

	socket(PF_INET, SOCK_DGRAM, 0);
	if (ctrl_sock < 0) {
		LOG("Failed to create query control socket: %d\n", ctrl_sock);
		return -1;
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
