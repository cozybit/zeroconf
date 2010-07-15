#include "mdns.h"
#include "mdns_private.h"
#include "mdns_port.h"

static char *hname; /* this is a cstring, not a DNS string */
static char *dname; /* this is a cstring, not a DNS string */
/* Our fully qualified domain name is something like 'node.local.' */
static char fqdn[MDNS_MAX_NAME_LEN + 1];
/* Our reverse name for the in-addr.arpa PTR record */
static char in_addr_arpa[MDNS_INADDRARPA_LEN];
static uint32_t my_ipaddr;

/* global mdns state */
static void *mdns_thread;
static int mc_sock;
static int ctrl_sock;
static int mdns_enabled;

/* control message types */
#define MDNS_CTRL_HALT 0

static struct mdns_message tx_message;
static struct mdns_message rx_message;

/* Find the length of a DNS name.  A DNS name may be one of:
 *	- a series of labels terminated by a NULL byte
 *	- a series of labels terminated by a pointer
 *	- a pointer
 *
 * return -1 if the name is invalid, or the length of the name.
 */
int mdns_name_length(char *name)
{
	char *start = name;
	while (*name != 0x00) {
		if (*name & 0xC0) { /* pointer */
			name++;
			break;
		}

		if (*name  > MDNS_MAX_LABEL_LEN)
			return -1;

		/* we've found a valid label length */
		name += *name + 1;
	}
	return name - start + 1;
}

/* write the normal c string "name" to "dst" with appropriate dns length and
 * null termination.
 */
static char *put_name(char *dst, char *name)
{
	int len;
	char *p = dst;

	len = (uint8_t)strlen(name);
	*p++ = len;
	strcpy(p, name);
	return p + len;
}

/* reset the fqdn to hostname.domain.
 */
static void reset_fqdn(void)
{
	char *p;
	p = put_name(fqdn, hname);
	put_name(p, dname);
}

#define CHECK_TAILROOM(m, l) \
	do { \
		if (m->cur + l - 1 > m->end) { \
			DBG("Warning: truncated mdns message (%d).\n", __LINE__); \
			return -1; \
		} \
	} while (0)

/* Parse an incoming dns message of length blen into the struct mdns_message m.
 * Return 0 for success, -1 for failure.
 */
static int mdns_parse_message(struct mdns_message *m, int mlen)
{
	uint16_t t;
	int len, i;

	if (mlen < sizeof(struct mdns_header)) {
		LOG("Warning: DNS message too short.\n");
		return -1;
	}
	m->header = (struct mdns_header *)m->data;
	m->len = mlen;
	m->end = m->data + mlen - 1;
	m->cur = (char *)m->header + sizeof(struct mdns_header);
	m->num_questions = ntohs(m->header->qdcount);
	m->num_answers = ntohs(m->header->ancount);
	m->header->flags.num = ntohs(m->header->flags.num);

	if (m->header->flags.fields.opcode != DNS_OPCODE_QUERY) {
		DBG("Ignoring message with opcode != QUERY\n");
		return -1;
	}

	if (m->num_questions > MDNS_MAX_QUESTIONS) {
		LOG("Warning: Only parsing first %d questions of %d\n",
			MDNS_MAX_QUESTIONS, m->num_questions);
		m->num_questions = MDNS_MAX_QUESTIONS;
	}
	for(i = 0; i < m->num_questions; i++) {
		/* get qname */
		m->questions[i].qname = m->cur;
		len = mdns_name_length(m->cur);
		if (len == 0) {
			DBG("Warning: invalid name in question %d\n", i);
			return -1;
		}
		CHECK_TAILROOM(m, len);
		m->cur += len;

		/* get qtype and qclass */
		CHECK_TAILROOM(m, 2*sizeof(uint16_t));
		t = get_uint16(m->cur);
		m->cur += sizeof(uint16_t);
		if (t > T_ANY) {
			DBG("Warning: invalid type in question: %u\n", t);
			return -1;
		}
		m->questions[i].qtype = t;

		/* get qclass */
		t = get_uint16(m->cur);
		m->cur += sizeof(uint16_t);
		if (t != C_FLUSH && t != C_IN) {
			DBG("Warning: invalid class in question: %u\n", t);
			return -1;
		}
		m->questions[i].qclass = t;
	}

	if (m->num_answers > MDNS_MAX_ANSWERS) {
		LOG("Warning: Only parsing first %d answers of %d\n",
			MDNS_MAX_ANSWERS, m->num_answers);
		m->num_answers = MDNS_MAX_ANSWERS;
	}
	for(i = 0; i < m->num_answers; i++) {
		m->answers[i].name = m->cur;
		len = mdns_name_length(m->cur);
		if (len == -1) {
			DBG("Warning: invalid label in answer %d\n", i);
			return -1;
		}
		CHECK_TAILROOM(m, len);
		m->cur += len;
		CHECK_TAILROOM(m, 4*sizeof(uint16_t));
		m->answers[i].type = get_uint16(m->cur);
		m->cur += sizeof(uint16_t);
		m->answers[i].class = get_uint16(m->cur);
		m->cur += sizeof(uint16_t);
		m->answers[i].ttl = get_uint16(m->cur);
		m->cur += sizeof(uint16_t);
		m->answers[i].rdlength = get_uint16(m->cur);
		m->cur += sizeof(uint16_t);
		CHECK_TAILROOM(m, m->answers[i].rdlength);
		m->answers[i].rdata = m->cur;
		m->cur += m->answers[i].rdlength;
	}

	if (ntohs(m->header->nscount) != 0)
		LOG("Warning: Ignroing authority RRs\n");

	if (ntohs(m->header->arcount) != 0)
		LOG("Warning: Ignroing additional RRs\n");

	return 0;
}

/* prepare an empty query message m.  Return 0 for success and -1 for
 * failure.
 */
static int mdns_query_init(struct mdns_message *m)
{
	m->len = sizeof(m->data);
	m->header = (struct mdns_header *)m->data;
	m->cur = m->data + sizeof(struct mdns_header);
	m->end = m->data + sizeof(m->data) - 1;
	memset(m->header, 0x00, sizeof(struct mdns_header));
	return 0;
}

/* prepare an empty response message m.  Return 0 for success and -1 for
 * failure.
 */
static int mdns_response_init(struct mdns_message *m)
{
	/* a response is just like a query at first */
	if (mdns_query_init(m) != 0)
		return -1;

	m->header->flags.fields.qr = 1; /* response */
	m->header->flags.fields.aa = 1; /* authoritative */
	m->header->flags.fields.rcode = 0;
	m->header->flags.num = htons(m->header->flags.num);
	return 0;
}

/* add a name, type, class, ttl tuple to the message m.  If the ttl is
 * (uint32_t)-1, then we only add the name, type, and class.  This is common
 * functionality used to add questions, answers, and authorities.  No error
 * checking is performed; the caller is responsible for ensuring that all
 * values will fit.  Return 0 for success or -1 for failure.
 */
static int __mdns_add_tuple(struct mdns_message *m, char *name, uint16_t type,
							uint16_t class, uint32_t ttl)
{
	uint16_t len = (uint16_t)mdns_name_length(name);
	int size = ttl == (uint32_t)-1 ? len + 2*sizeof(uint16_t) :
		len + 2*sizeof(uint16_t) + sizeof(uint32_t);
	CHECK_TAILROOM(m, size);
	memcpy(m->cur, name, len);
	m->cur += len;
	set_uint16(m->cur, type);
	m->cur += sizeof(uint16_t);
	set_uint16(m->cur, class);
	m->cur += sizeof(uint16_t);
	if (ttl != (uint32_t)-1) {
		set_uint32(m->cur, ttl);
		m->cur += sizeof(uint32_t);
	}
	return 0;
}

/* add a question with name qname, type qtype, and class qclass to the message
 * m.  Return 0 for success, -1 for failure.
 */
static int mdns_add_question(struct mdns_message *m, char *qname,
							 uint16_t qtype, uint16_t qclass)
{
	if (__mdns_add_tuple(m, qname, qtype, qclass, (uint32_t)-1) == -1)
		return -1;
	m->header->qdcount = htons(htons(m->header->qdcount) + 1);
	return 0;
}

/* add an answer to name to the message m.  Return 0 for success, -1 for
 * failure.  Note that this function does not add the resource record data.  It
 * just populates the common header.
 */
static int mdns_add_answer(struct mdns_message *m, char *name, uint16_t type,
						   uint16_t class, uint32_t ttl)
{
	if (__mdns_add_tuple(m, name, type, class, ttl))
		return -1;
	m->header->ancount = htons(htons(m->header->ancount) + 1);
	return 0;
}

/* add a proposed answer for name to the authority section of the message m.
 * Return 0 for success, -1 for failure.  Note that this function does not add
 * the resource record data.  It just populates the common header.
 */
static int mdns_add_authority(struct mdns_message *m, char *name,
							  uint16_t type, uint16_t class, uint32_t ttl)
{
	if (__mdns_add_tuple(m, name, type, class, ttl))
		return -1;
	m->header->nscount = htons(htons(m->header->nscount) + 1);
	return 0;
}

/* add a 4-byte record containing i to the message m.  This is used for A
 * records.
 */
static int mdns_add_uint32(struct mdns_message *m, uint32_t i)
{
	CHECK_TAILROOM(m, sizeof(uint16_t) + sizeof(uint32_t));
	set_uint16(m->cur, sizeof(uint32_t));
	m->cur += sizeof(uint16_t);
	set_uint32(m->cur, i);
	m->cur += sizeof(uint32_t);
	return 0;
}

/* add a dns name containing name to the message m.  This is used for cname,
 * txt, ns, and ptr.
 */
static int mdns_add_name(struct mdns_message *m, char *name)
{
	int len = mdns_name_length(name);
	CHECK_TAILROOM(m, len + sizeof(uint16_t));
	set_uint16(m->cur, len);
	m->cur += sizeof(uint16_t);
	memcpy(m->cur, name, len);
	m->cur += len;
	return 0;
}

/* add a dns SRV record to message m with given priority, weight, port, and
 * target name.
 */
static int mdns_add_srv(struct mdns_message *m, uint16_t priority,
						uint16_t weight, uint16_t port, char *target)
{
	int len = mdns_name_length(target);
	CHECK_TAILROOM(m, len + 3*sizeof(uint16_t));
	set_uint16(m->cur, priority);
	m->cur += sizeof(uint16_t);
	set_uint32(m->cur, weight);
	m->cur += sizeof(uint16_t);
	set_uint32(m->cur, port);
	m->cur += sizeof(uint16_t);
	memcpy(m->cur, target, len);
	m->cur += len;
	return 0;
}

static int m_socket(void)
{
	int sock;
	int yes = 1;
	int no = 0;
	unsigned char ttl = 255;
	struct sockaddr_in in_addr;
	struct ip_mreq mc;

	/* set up bind address (any, port 5353) */
	memset(&in_addr, 0, sizeof(in_addr));
	in_addr.sin_family = AF_INET;
	in_addr.sin_port = htons(5353);
	in_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		LOG("error: could not open multicast socket\n");
		return sock;
	}

#ifdef SO_REUSEPORT
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (char*)&yes,
					sizeof(yes)) < 0) {
		LOG("error: failed to set SO_REUSEPORT option\n");
		return -1;
	}
#endif
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes));

	if (bind(sock, (struct sockaddr*)&in_addr, sizeof(in_addr))) {
		close(sock);
		return -1;
	}

	/* join multicast group */
	mc.imr_multiaddr.s_addr = inet_addr("224.0.0.251");
	mc.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mc,
					sizeof(mc)) < 0) {
		LOG("error: failed to join multicast group\n");
		return -1;
	}
	/* set other IP-level options */
	if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (unsigned char *)&ttl,
					sizeof(ttl)) < 0) {
		LOG("error: failed to set multicast TTL\n");
		return -1;
	}
#if defined(IP_MULTICAST_LOOP) && !defined(LOOPBACK)
	if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&no,
					sizeof(no)) < 0) {
		LOG("error: failed to unset IP_MULTICAST_LOOP option\n");
		return -1;
	}
#endif

	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0) {
		LOG("error: failed to put socket in non-blocking mode\n");
		return -1;
	}
	return sock;
}

static int send_message(struct mdns_message *m, int sock, short port)
{
	struct sockaddr_in to;
	int size, len;

	/* get message length */
	size = (unsigned int)m->cur - (unsigned int)m->header;

	to.sin_family = AF_INET;
	to.sin_port = port;
	to.sin_addr.s_addr = inet_addr("224.0.0.251");

	len = sendto(sock, (char *)m->header, size, 0, (struct sockaddr *)&to,
				  sizeof(struct sockaddr_in));

	if (len < size) {
		LOG("error: failed to send message\n");
		return 0;
	}

	DBG("sent %u-byte message\n", size);
	return 1;
}

static int send_ctrl_msg(int msg)
{
	int ret;
	struct sockaddr_in to;
	int s;

	s = socket(PF_INET, SOCK_DGRAM, 0);
	if (s < 0)
		return -1;

	memset((char *)&to, 0, sizeof(to));
	to.sin_family = PF_INET;
	to.sin_port = htons(MDNS_CTRL_PORT);
	to.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = sendto(s, &msg, sizeof(msg), 0, (struct sockaddr *)&to, sizeof(to));
	if (ret != -1)
		ret = 0;

	return ret;
}

/* Make m a probe packet suitable for the initial probe sequence.  It must
 * contain a question for each resource record that we intend on advertising
 * and an authority with our propose answers.
 */
static int mdns_prepare_probe(struct mdns_message *m)
{
	mdns_query_init(m);
	if (mdns_add_question(m, fqdn, T_ANY, C_IN) != 0 ||
		mdns_add_question(m, in_addr_arpa, T_ANY, C_IN) != 0 ||
		mdns_add_authority(m, fqdn, T_A, C_IN, 255) != 0 ||
		mdns_add_uint32(m, htonl(my_ipaddr)) != 0 ||
		mdns_add_authority(m, in_addr_arpa, T_PTR, C_FLUSH, 255) != 0 ||
		mdns_add_name(m, fqdn) != 0) {
		LOG("Resource records don't fit into probe packet.\n");
		return -1;
	}
	return 0;
}

/* if we detect a conflict during probe time, we must grow our host name and
 * service names with -2, or -3, etc.  Report the maximum number of extra bytes
 * that we would need to do this.
 */
static int max_probe_growth(int num_services)
{
	/* We need 2 bytes for each instance of the host name that appears in the
	 * probe (A record q, A record a, inaddrarpa PTR, one for each SRV PTR),
	 * and 2 for each service name.
	 */
	return 2*3 + 2*2*num_services;
}

/* prepare a response to the message rx.  Put it in tx.  Return -1 for error, 0
 * for no need to respond, or 1 for send response.
 */
static int mdns_prepare_response(struct mdns_message *rx,
								 struct mdns_message *tx)
{
	int i, ret = 0;
	struct mdns_question *q;

	if (rx->header->flags.fields.qr != QUERY)
		return 0;

	mdns_response_init(tx);
	tx->header->id = rx->header->id;

	for(i = 0; i < rx->num_questions; i++) {
		q = &rx->questions[i];
		if (q->qtype == T_ANY || q->qtype == T_A) {
			if (strcmp(q->qname, fqdn) == 0) {
				if (mdns_add_answer(tx, fqdn, T_A, C_FLUSH, 225) != 0 ||
					mdns_add_uint32(tx, htonl(my_ipaddr)) != 0)
					return -1;
				ret = 1;
			}
		} else if (q->qtype == T_ANY || q->qtype == T_PTR) {
			if (strcmp(q->qname, in_addr_arpa) == 0) {
				if (mdns_add_answer(tx, in_addr_arpa, T_PTR, C_FLUSH, 225) != 0 ||
					mdns_add_name(tx, fqdn) != 0)
					return -1;
				ret = 1;
			}
		}
	}
	return ret;
}

/* Change the name foo.local to foo-2.local.  If it already says foo-X.local,
 * make it foo-(X+1).local.  If it says foo-9.local, just leave it alone and
 * return -1.  The caller should sleep for a while and re-try foo.local at that
 * point.  name must be a valid dns name.
 */
int mdns_increment_name(char *name)
{
	int len = name[0], newlen;

	if (name[len - 1] == '-' && name[len] >= '2' && name[len] < '9') {
		name[len] += 1;
		return 0;
	}

	if (name[len - 1] == '-' && name[len] == '9')
		return -1;

	newlen = len + 2;
	if (newlen > MDNS_MAX_LABEL_LEN)
		newlen = MDNS_MAX_LABEL_LEN;

	name[0] = newlen;
	memmove(&name[len + 3], &name[len + 1], strlen(name) - len - 1);
	name[len + 1] = '-';
	name[len + 2] = '2';
	DBG("Derived new name: ");
	debug_print_name(NULL, name);
	DBG("\n");
	return 0;
}

/* does the message m conflict with our host name or service names?  If so,
 * alter the service names and return 1.  Otherwise return 0.  If we've seen so
 * many conflicts that we have tried all the possible names, return -1;
 */
static int fix_response_conflicts(struct mdns_message *m)
{
	struct mdns_resource *r;
	int ret = 0, i;

	if (m->header->flags.fields.qr != RESPONSE)
		return 0;

	for(i = 0; i < m->num_answers; i++) {
		r = &m->answers[i];
		if (r->type == T_A && strncmp(r->name, fqdn, sizeof(fqdn)) == 0) {
			DBG("Detected conflict with this packet:\n");
			debug_print_message(m);
			DBG("\n");
			/* try a different name */
			if (mdns_increment_name(fqdn) == -1)
				ret = -1;
			ret = 1;
		}
	}
	return ret;
}

/* dumb macro to set a struct timeval to "ms" milliseconds. */
#define SET_TIMEOUT(t, ms)								\
	do {												\
		(t)->tv_sec = (ms)/1000;						\
		(t)->tv_usec = ((ms) - (ms)/1000) * 1000;		\
	} while (0)

/* calculate the interval between the start and stop timestamps accounting for
 * wrap.
 */
static uint32_t interval(uint32_t start, uint32_t stop)
{
	if (stop >= start)
		return stop - start;
	return UINT32_MAX - start + stop;
}

/* We wanted to timeout after "target" ms, but we got interrupted.  Calculate
 * the new timeout "t" given that we started our timeout at "start" and were
 * interupted at "stop".
 */
static void recalc_timeout(struct timeval *t, uint32_t start, uint32_t stop,
						   uint32_t target)
{
	uint32_t waited, remaining;

	waited = interval(start, stop);
	if (target <= waited) {
		SET_TIMEOUT(t, 0);
		return;
	}
	remaining = target - waited;
	SET_TIMEOUT(t, remaining);
}


/* We're in a probe state and we got a probe response (rx).  Process it and
 * return the next state.  Also, update the timeout with the time until the
 * next event.  state must be one of the probe states!
 */
static int process_probe_resp(struct mdns_message *rx, int state,
							  struct timeval *timeout, uint32_t start_wait,
							  uint32_t stop_wait)
{
	int ret;

	ret = fix_response_conflicts(&rx_message);
	if (ret == 1) {
		/* there were conflicts with some of our names.  The names have been
		 * adapted.  Go back to square one.
		 */
		SET_TIMEOUT(timeout, 0);
		return INIT;

	} else if (ret == -1) {
		/* we've tried lots of names.  Reset to our original name, sleep for
		 * 5s, then try again with our original name.
		 */
		SET_TIMEOUT(timeout, 5000);
		reset_fqdn();
		return INIT;
	} else {
		/* this was an unrelated message.  Remain in the same state.  Assume
		 * the caller has set the timeout properly.
		 */
		recalc_timeout(timeout, start_wait, stop_wait, 250);
		return state;
	}
}

/* This is the mdns thread function */
static void do_mdns(void *data)
{
	int max_sock;
	int msg, ret;
	struct sockaddr_in from;
	int active_fds;
	fd_set fds;
	int len = 0;
	struct timeval *timeout;
	struct timeval probe_wait_time;
	socklen_t in_size = sizeof(struct sockaddr_in);
	int state = INIT;
	int event;
	uint32_t start_wait, stop_wait;

	mdns_enabled = 1;

	/* TODO: wait a random amount of time before first probe per
	 * specification
	 */
	SET_TIMEOUT(&probe_wait_time, 0);
	timeout = &probe_wait_time;

	while (mdns_enabled) {
		FD_ZERO(&fds);
		FD_SET(mc_sock, &fds);
		FD_SET(ctrl_sock, &fds);
		max_sock = ctrl_sock > mc_sock ? ctrl_sock : mc_sock;

		start_wait = mdns_time_ms();
		active_fds = select(max_sock + 1, &fds, NULL, NULL, timeout);
		stop_wait = mdns_time_ms();

		if (active_fds < 0 && errno != EINTR)
			LOG("error: select() failed: %d\n", errno);

		/* handlde control events outside of the state machine. If we ever have
		 * more than just a HALT command, we may have to move it.
		 */
		if (FD_ISSET(ctrl_sock, &fds)) {
			DBG("Got control message.\n");
			ret = recvfrom(ctrl_sock, &msg, sizeof(msg), 0,
						   (struct sockaddr *)0, 0);
			if (ret == -1) {
				LOG("Warning: failed to get control message\n");
			} else {
				if (msg == MDNS_CTRL_HALT) {
					LOG("mdns done.\n");
					mdns_enabled = 0;
					break;
				}
			}
		}

		if (FD_ISSET(mc_sock, &fds)) {
			len = recvfrom(mc_sock, rx_message.data, sizeof(rx_message.data),
						   0, (struct sockaddr*)&from, &in_size);
			if (len < 0) {
				LOG("failed to recv packet\n");
				continue;
			}
			ret = mdns_parse_message(&rx_message, len);
			if (ret != 0 || from.sin_addr.s_addr == my_ipaddr)
				continue;
			event = EVENT_RX;
		} else {
			event = EVENT_TIMEOUT;
		}

		DBG("Got event %s in state %s\n",
			eventnames[event], statenames[state]);
		switch (state) {
		case INIT:
			if (event == EVENT_TIMEOUT) {
				DBG("Sending first probe\n");
				mdns_prepare_probe(&tx_message);
				send_message(&tx_message, mc_sock, 5353);
				SET_TIMEOUT(&probe_wait_time, 250);
				state = FIRST_PROBE_SENT;
			} else if (event == EVENT_RX) {
				recalc_timeout(&probe_wait_time, start_wait, stop_wait, 250);
			}
			timeout = &probe_wait_time;
			break;

		case FIRST_PROBE_SENT:
			if (event == EVENT_TIMEOUT) {
				DBG("Sending second probe\n");
				send_message(&tx_message, mc_sock, 5353);
				SET_TIMEOUT(&probe_wait_time, 250);
				state = SECOND_PROBE_SENT;
			} else if (event == EVENT_RX) {
				state = process_probe_resp(&rx_message, state,
										   &probe_wait_time, start_wait,
										   stop_wait);
			}
			timeout = &probe_wait_time;
			break;

		case SECOND_PROBE_SENT:
			if (event == EVENT_TIMEOUT) {
				DBG("Sending third probe\n");
				send_message(&tx_message, mc_sock, 5353);
				SET_TIMEOUT(&probe_wait_time, 250);
				state = THIRD_PROBE_SENT;
			} else if (event == EVENT_RX) {
				state = process_probe_resp(&rx_message, state,
										   &probe_wait_time, start_wait,
										   stop_wait);
			}
			timeout = &probe_wait_time;
			break;

		case THIRD_PROBE_SENT:
			if (event == EVENT_TIMEOUT) {
				/* Okay.  We now own our name.  Announce it. */
				mdns_response_init(&tx_message);
				if (mdns_add_answer(&tx_message, fqdn, T_A, C_FLUSH, 225) != 0 ||
					mdns_add_uint32(&tx_message, my_ipaddr) != 0) {
					/* This is highly unlikely */
					break;
				}
				send_message(&tx_message, mc_sock, 5353);
				timeout = NULL;
				state = IDLE;

			} else if (event == EVENT_RX) {
				state = process_probe_resp(&rx_message, state,
										   &probe_wait_time, start_wait,
										   stop_wait);
				timeout = &probe_wait_time;
			}
			break;

		case IDLE:
			if (event == EVENT_RX) {
				/* prepare a response if necessary */
				ret = mdns_prepare_response(&rx_message, &tx_message);
				if (ret <= 0)
					break;
				DBG("responding to query:\n");
				debug_print_message(&rx_message);
				send_message(&tx_message, mc_sock, from.sin_port);
			}
			break;
		}
	}
}

/* return 0 for invalid, 1 for valid */
#ifdef MDNS_CHECK_ARGS
static int valid_label(char *name)
{
	if (strlen(name) > MDNS_MAX_LABEL_LEN ||
		strchr(name, '.') != NULL)
		return 0;
	return 1;
}
#else
#define valid_label(name) (1)
#endif

/* create the inaddrarpa domain name DNS string for ipaddr.  Output buffer must
 * be at least MDNS_INADDRARPA_LEN.  ipaddr must be in host order.
 */
static void ipaddr_to_inaddrarpa(uint32_t ipaddr, char *out)
{
	char *ptr = out;
	uint8_t byte;
	int i, len;

	/* This is a DNS name of the form 45.1.168.192.in-addr.arpa */
	for (i = 0; i < 4; i++) {
		byte = ipaddr & 0xff;
		ipaddr >>= 8;
		len = sprintf(ptr + 1, "%d", byte);
		*ptr = len;
		ptr += len + 1;
	}
	*ptr++ = 7;
	sprintf(ptr, "in-addr");
	ptr += 7;
	*ptr++ = 4;
	sprintf(ptr, "arpa");
}

int mdns_launch(uint32_t ipaddr, char *domain, char *hostname,
				struct mdns_service **services)
{
	int one = 1, ret;
	struct sockaddr_in ctrl_listen;
	int addr_len, num_services = 0;

	my_ipaddr = ipaddr;

	/* populate the fqdn */
	if (domain == NULL)
		domain = "local";
	if (!valid_label(hostname) || !valid_label(domain)) {
		LOG("Invalid hostname: %s\n", hostname);
		return MDNS_INVAL;
	}
	/* populate fqdn */
	hname = hostname;
	dname = domain;
	reset_fqdn();

	ipaddr_to_inaddrarpa(htonl(ipaddr), in_addr_arpa);

	mc_sock = m_socket();
	if (mc_sock < 0) {
		LOG("error: unable to open multicast socket\n");
		return 1;
	}

	if (services != NULL)
		LOG("Warning: services not implemented yet.\n");

	/* check to make sure all of our names and services will fit */
	ret = mdns_prepare_probe(&tx_message);
	if (ret == -1)
		return MDNS_TOOBIG;

	if (max_probe_growth(num_services) > tx_message.end - tx_message.cur + 1) {
		LOG("Insufficient space for host name and service names\n");
		return MDNS_TOOBIG;
	}

	/* create control socket */
	ctrl_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (ctrl_sock < 0) {
		LOG("Failed to create control socket.\n");
		return -1;
	}
	setsockopt(ctrl_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one));
	ctrl_listen.sin_family = PF_INET;
	ctrl_listen.sin_port = htons(MDNS_CTRL_PORT);
	ctrl_listen.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr_len = sizeof(struct sockaddr_in);

	/* bind control socket */
	ret = bind(ctrl_sock, (struct sockaddr *)&ctrl_listen, addr_len);
	if (ret < 0) {
		LOG("Failed to bind control socket\n");
		return -1;
	}

	mdns_thread = mdns_thread_create(do_mdns, NULL);
	if (mdns_thread == NULL)
		return -1;
	return 0;
}

void mdns_halt(void)
{
	int ret;

	LOG("Halting mdns.\n");
	ret = send_ctrl_msg(MDNS_CTRL_HALT);
	if (ret != 0) {
		LOG("Warning: failed to send HALT message to mdns: %d\n", errno);
	} else {
		mdns_thread_yield();
	}
	if (mdns_enabled != 0)
		LOG("Warning: failed to halt mdns.  Forcing.\n");

	mdns_thread_delete(mdns_thread);
	close(ctrl_sock);
	close(mc_sock);
	mdns_enabled = 0;
}
