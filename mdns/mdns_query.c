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

#define CTRL_MSG_LEN 255

struct query_ctrl_msg {
	int command;
	int length;
	char data[CTRL_MSG_LEN];
};

#define CTRL_MSG_MIN (sizeof(struct query_ctrl_msg) - CTRL_MSG_LEN)

static struct query_ctrl_msg ctrl_msg;

/* Send a control message msg of length len to the server listening at
 * localhost:port.  Expect an int response code from the server.
 */
int query_send_ctrl_msg(struct query_ctrl_msg *msg, int len, uint16_t port)
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

	ret = sendto(s, msg, len, 0, (struct sockaddr *)&to, sizeof(to));
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
			if (ret == -1 || ret < sizeof(ctrl_msg) - CTRL_MSG_LEN) {
				LOG("Warning: querier failed to get control message\n");
				status = MDNS_INVAL;
			} else {
				if (ctrl_msg.command == MDNS_CTRL_HALT) {
					LOG("Querier done.\n");
					query_enabled = 0;
					status = MDNS_SUCCESS;
				} else {
					LOG("Unkown control message %d", ctrl_msg.command);
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
	char buf[sizeof(ctrl_msg) - CTRL_MSG_LEN];
	struct query_ctrl_msg *ctrl_msg = (struct query_ctrl_msg *)buf;

	ctrl_msg->command = MDNS_CTRL_HALT;
	ctrl_msg->length = sizeof(buf);

	ret = query_send_ctrl_msg(ctrl_msg, ctrl_msg->length, MDNS_CTRL_QUERIER);
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
	ctrl_msg.command = MDNS_CTRL_MONITOR;
	ctrl_msg.length = CTRL_MSG_MIN;

	return query_send_ctrl_msg(&ctrl_msg, ctrl_msg.length, MDNS_CTRL_QUERIER);
}

void mdns_query_unmonitor(char *fqst)
{
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
