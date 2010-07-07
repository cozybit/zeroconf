#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "mdns.h"
#include "mdns_private.h"
#include "mdns_os.h"

/* Our fully qualified domain name is something like 'node.local.' */
static uint8_t fqdn[2*(MDNS_MAX_LABEL_LEN + 1)];

uint16_t mdns_name_length(char *name);
uint16_t mdns_read_n16(struct mdns_message *m);
uint32_t mdns_read_n32(struct mdns_message *m);
void mdns_write_n16(struct mdns_message *m, uint16_t n);
void mdns_write_n32(struct mdns_message *m, uint32_t n);

uint16_t mdns_read_n16(struct mdns_message *m)
{
	uint16_t n = ntohs(*((uint16_t*)m->cur));
	m->cur += sizeof(uint16_t);
	return n;
}

uint32_t mdns_read_n32(struct mdns_message *m)
{
	uint32_t n = ntohl(*((uint32_t*)m->cur));
	m->cur += sizeof(uint32_t);
	return n;
}

void mdns_write_n16(struct mdns_message *m, uint16_t n)
{
	*((uint16_t*)m->cur) = htons(n);
	m->cur += sizeof(uint16_t);
}

void mdns_write_n32(struct mdns_message *m, uint32_t n)
{
	*((uint32_t*)m->cur) = htonl(n);
	m->cur += sizeof(uint32_t);
}

void mdns_write_name(struct mdns_message *m, char *name)
{
	uint16_t len = mdns_name_length(name);
	memcpy(m->cur, name, len);
	m->cur += len;
}

/* a name may be one of:
 *	- a series of labels terminated by a NULL byte
 *	- a series of labels terminated by a pointer
 *	- a pointer */
int mdns_traverse_name(struct mdns_message *m)
{
	while (*(m->cur) != 0x00) {
		if ((*(m->cur) & 0xC0)) { /* pointer */
			m->cur++;
			break;
		}
		else if (*(m->cur) <= MDNS_MAX_LABEL_LEN) /* valid label */
			m->cur += *(m->cur)+1; /* move to next label */
		else /* invalid label */
			return 0;
	}
	m->cur++; /* move past terminating byte */
	return 1;
}

uint16_t mdns_name_length(char *name)
{
	char *start = name;

	while (*name != 0) {
		if (*name & 0xC0) {
			name++;
			break;
		}
		name += *name+1;
	}

	return (uint32_t)name - (uint32_t)start + 1;
}

int mdns_parse_message(struct mdns_message *m, char *b)
{
	uint16_t i, t;

	m->header = (struct mdns_header *)b;
	m->num_questions = ntohs(m->header->qdcount);
	m->num_answers = ntohs(m->header->ancount);

	m->header->flags.num = ntohs(m->header->flags.num);

	if (m->header->flags.fields.opcode != 0) {
		DB_PRINT("dropping message with opcode != 0\n");
		return 0;
	}

#if 0
	if (m->header->flags.fields.rcode != 0) {
		DB_PRINT("dropping message with rcode != 0\n");
		return 0;
	}
#endif

	m->cur = (char *)m->header + sizeof(struct mdns_header);

	for(i = 0; i < m->num_questions && i < MDNS_MAX_QUESTIONS; i++) {
		/* get qname */
		m->questions[i].qname = m->cur;
		if (!mdns_traverse_name(m)) { /* move past qname */
			DB_PRINT("dropping message: invalid label in question %d\n",i);
			return 0;
		}
		/* get qtype */
		t = mdns_read_n16(m);
		if (t <= T_ANY)
			m->questions[i].qtype = t;
		else {
			DB_PRINT("dropping message: invalid type %u\n", t);
			return 0;
		}
		/* get qclass */
		t = mdns_read_n16(m);
		if ((t & ~0x8000) == 1)
			m->questions[i].qclass = t;
		else {
			DB_PRINT("dropping message: invalid class %u\n", t);
			return 0;
		}
	}

	for(i = 0; i < m->num_answers && i < MDNS_MAX_ANSWERS; i++) {
		m->answers[i].name = m->cur;
		if (!mdns_traverse_name(m)) {
			DB_PRINT("dropping message: invalid label in answer %d\n", i);
			return 0;
		}
		m->answers[i].type = mdns_read_n16(m);
		m->answers[i].class = mdns_read_n16(m);
		m->answers[i].ttl = mdns_read_n32(m);
		m->answers[i].rdlength = mdns_read_n16(m);
		m->answers[i].rdata = m->cur;
		m->cur += m->answers[i].rdlength;
	}
	return 1;
}

/* transmit message creation */

void rr_transfer_a(struct mdns_message *m, union rr_p r)
{
	mdns_write_n32(m, r.a->ip);
}

void rr_transfer_cname(struct mdns_message *m, union rr_p r)
{
	mdns_write_name(m, r.cname->name);
}

void rr_transfer_txt(struct mdns_message *m, union rr_p r)
{
	mdns_write_name(m, r.txt->data);
}

void rr_transfer_ns(struct mdns_message *m, union rr_p r)
{
	mdns_write_name(m, r.ns->name);
}

void rr_transfer_srv(struct mdns_message *m, union rr_p r)
{
	mdns_write_n16(m, r.srv->priority);
	mdns_write_n16(m, r.srv->weight);
	mdns_write_n16(m, r.srv->port);
	mdns_write_name(m, r.srv->target);
}

void rr_transfer_ptr(struct mdns_message *m, union rr_p r)
{
	mdns_write_name(m, r.ptr->name);
}

uint16_t rr_length_a(union rr_p r)
{
	return sizeof(uint32_t);
}

uint16_t rr_length_cname(union rr_p r)
{
	return mdns_name_length(r.cname->name);
}

uint16_t rr_length_txt(union rr_p r)
{
	return mdns_name_length(r.txt->data);
}

uint16_t rr_length_ns(union rr_p r)
{
	return mdns_name_length(r.ns->name);
}

uint16_t rr_length_ptr(union rr_p r)
{
	return mdns_name_length(r.ptr->name);
}

uint16_t rr_length_srv(union rr_p r)
{
	return 3*sizeof(uint16_t)+mdns_name_length(r.srv->target);
}

void mdns_transmit_init(struct mdns_message *m, char *b, msg_type mtype)
{
	m->header = (struct mdns_header *)b;
	m->cur = b + sizeof(struct mdns_header);
	memset(m->header, 0x00, sizeof(struct mdns_header));

	if (mtype == RESPONSE) {
		m->header->flags.fields.qr = 1; /* response */
		m->header->flags.fields.aa = 1; /* authoritative */
		m->header->flags.fields.rcode = 0;
		m->header->flags.num = htons(m->header->flags.num);
	}
}

void mdns_add_question(struct mdns_message *m, char* qname,
						uint16_t qtype, uint16_t qclass)
{
	uint16_t len = mdns_name_length(qname);

	memcpy(m->cur, qname, len);
	m->cur += len;
	mdns_write_n16(m, qtype);
	mdns_write_n16(m, qclass);
	m->header->qdcount += htons(1);
}

void mdns_add_answer(struct mdns_message *m, char *name, uint16_t type,
					  uint16_t class, uint32_t ttl, struct mdns_rr *rr)
{
	uint16_t d_len = mdns_name_length(name);

	memcpy(m->cur, name, d_len);
	m->cur += d_len;
	mdns_write_n16(m, type);
	mdns_write_n16(m, class);
	mdns_write_n32(m, ttl);
	mdns_write_n16(m, rr->length(rr->data));
	rr->transfer(m, rr->data);

	m->header->ancount += htons(1);
}

int m_socket(void)
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
		DB_PRINT("error: could not open multicast socket\n");
		return sock;
	}

#ifdef SO_REUSEPORT
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (char*)&yes,
					sizeof(yes)) < 0) {
		DB_PRINT("error: failed to set SO_REUSEPORT option\n");
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
		DB_PRINT("error: failed to join multicast group\n");
		return -1;
	}
	/* set other IP-level options */
	if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (unsigned char *)&ttl,
					sizeof(ttl)) < 0) {
		DB_PRINT("error: failed to set multicast TTL\n");
		return -1;
	}
#if defined(IP_MULTICAST_LOOP) && !defined(LOOPBACK)
	if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&no,
					sizeof(no)) < 0) {
		DB_PRINT("error: failed to unset IP_MULTICAST_LOOP option\n");
		return -1;
	}
#endif

	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0) {
		DB_PRINT("error: failed to put socket in non-blocking mode\n");
		return -1;
	}
	return sock;
}

int send_message(struct mdns_message *m, int sock, short port)
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
		DB_PRINT("error: failed to send message\n");
		return 0;
	}

	DB_PRINT("sent %u-byte message\n", size);
	return 1;
}

/* mdns state */
static void *mdns_thread;
static int mc_sock;
static int ctrl_sock;
static int mdns_enabled;

/* control message types */
#define MDNS_CTRL_HALT 0

static int active_fds;
static fd_set fds;
static int len = 0;
static struct timeval timeout;
static char rx_buffer[1000];
static char tx_buffer[1000];
static struct mdns_message tx_message;
static struct mdns_message rx_message;
static struct sockaddr_in from;
static socklen_t in_size = sizeof(struct sockaddr_in);
static uint8_t status = FIRST_PROBE;

/* device settings */
static struct mdns_rr my_a;
static struct rr_a service_a;

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

/* This is the mdns thread function */
static void do_mdns(void *data)
{
	int max_sock;
	int msg, ret, i;

	mdns_enabled = 1;

	while (mdns_enabled) {
		timeout.tv_sec = 0;
		timeout.tv_usec = 250000;
		FD_ZERO(&fds);
		FD_SET(mc_sock, &fds);
		FD_SET(ctrl_sock, &fds);
		max_sock = ctrl_sock > mc_sock ? ctrl_sock : mc_sock;

		active_fds = select(max_sock + 1, &fds, NULL, NULL, &timeout);

		if (active_fds < 0 && errno != EINTR)
			DB_PRINT("error: select() failed: %d\n", errno);

		if (FD_ISSET(ctrl_sock, &fds)) {
			DB_PRINT("Got control message.\n");
			ret = recvfrom(ctrl_sock, &msg, sizeof(msg), 0,
						   (struct sockaddr *)0, 0);
			if (ret == -1) {
				DB_PRINT("Warning: failed to get control message\n");
			} else {
				if (msg == MDNS_CTRL_HALT) {
					DB_PRINT("mdns done.\n");
					mdns_enabled = 0;
					break;
				}
			}
		}

		if (FD_ISSET(mc_sock, &fds)) {
			len = recvfrom(mc_sock, rx_buffer, 1000, 0,
						   (struct sockaddr*)&from, &in_size);
			if (len < 0) {
				DB_PRINT("failed to recv packet\n");
				continue;
			}
			if (mdns_parse_message(&rx_message, rx_buffer)) {
				debug_print_message(&rx_message);
				/* TODO: parse, decide if/how to respond */
				if (status == STARTED &&
					from.sin_addr.s_addr != htonl(service_a.ip) &&
					MDNS_IS_QUERY(rx_message)) {

					for(i = 0; i < rx_message.num_questions; i++) {
						if (rx_message.questions[i].qtype == T_A &&
							strcmp(rx_message.questions[i].qname, fqdn) == 0)
						{
							DB_PRINT("responding to query...\n");
							mdns_add_answer(&rx_message, fqdn, T_A, C_IN, 225,
											&my_a);
							rx_message.header->flags.fields.qr = 1;
							rx_message.header->flags.fields.aa = 1;
							rx_message.header->flags.fields.rcode = 0;
							rx_message.header->flags.num =
								htons(rx_message.header->flags.num);
							send_message(&rx_message, mc_sock, from.sin_port);
						}
					}
				}
			}
		}
		else if (status < STARTED) {
			if (status == ANNOUNCE) {
				mdns_transmit_init(&tx_message, tx_buffer, RESPONSE);
				/* A */
				mdns_add_answer(&tx_message, fqdn, T_A, C_FLUSH, 225, &my_a);
			}
			send_message(&tx_message, mc_sock, 5353);
			status++;
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

int mdns_launch(uint32_t ipaddr, char *domain, char *hostname)
{
	int one = 1, ret;
	struct sockaddr_in ctrl_listen;
	int addr_len;
	uint8_t *p, len;

	/* We accept the IP arg in network order, but internally (and for no good
	 * reason) we store it in host order.
	 */
	service_a.ip = htonl(ipaddr);

	/* populate the fqdn */
	if (!valid_label(hostname) || !valid_label(domain)) {
		DB_PRINT("Invalid hostname: %s\n", hostname);
		return MDNS_INVAL;
	}
	/* names are preceded by their len */
	p = fqdn;
	len = (uint8_t)strlen(hostname);
	*p++ = len;
	strcpy(p, hostname);
	p += len;
	len = (uint8_t)strlen(domain);
	*p++ = len;
	strcpy(p, domain);

	my_a.data.a = &service_a;
	my_a.length = rr_length_a;
	my_a.transfer = rr_transfer_a;

	mc_sock = m_socket();
	if (mc_sock < 0) {
		DB_PRINT("error: unable to open multicast socket\n");
		return 1;
	}

	/* set up probe to claim name */
	mdns_transmit_init(&tx_message, tx_buffer, QUERY);
	mdns_add_question(&tx_message, "\6andrey\5_http\4_tcp\5local",
					   T_ANY, C_IN);

	/* create control socket */
	ctrl_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (ctrl_sock < 0) {
		DB_PRINT("Failed to create control socket.\n");
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
		DB_PRINT("Failed to bind control socket\n");
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
	int count = 0;

	DB_PRINT("Halting mdns.\n");
	ret = send_ctrl_msg(MDNS_CTRL_HALT);
	if (ret != 0)
		DB_PRINT("Warning: failed to send HALT message to mdns: %d\n", errno);
	else
		mdns_thread_yield();

	if (mdns_enabled != 0)
		DB_PRINT("Warning: failed to halt mdns.  Forcing.\n");

	mdns_thread_delete(mdns_thread);
	mdns_enabled = 0;
}
