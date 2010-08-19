/*
 * Copyright (C) cozybit, Inc. 2007-2010. All Rights Reserved.
 *
 * Use and redistribution subject to licensing terms.
 */
#include "mdns.h"
#include "mdns_private.h"
#include "mdns_port.h"

/* parse a resource record from the current pointer and put it in the resource
 * structure r.  Return 0 for success, -1 for failure.
 */
static int parse_resource(struct mdns_message *m, struct mdns_resource *r)
{
	int len;

	r->name = m->cur;
	len = dname_size(m->cur);
	if (len == -1) {
		DBG("Warning: invalid label in resource\n");
		return -1;
	}
	CHECK_TAILROOM(m, len);
	m->cur += len;
	CHECK_TAILROOM(m, 3*sizeof(uint16_t) + sizeof(uint32_t));
	r->type = get_uint16(m->cur);
	m->cur += sizeof(uint16_t);
	r->class = get_uint16(m->cur);
	m->cur += sizeof(uint16_t);
	r->ttl = get_uint32(m->cur);
	m->cur += sizeof(uint32_t);
	r->rdlength = get_uint16(m->cur);
	m->cur += sizeof(uint16_t);
	CHECK_TAILROOM(m, r->rdlength);
	r->rdata = m->cur;
	m->cur += r->rdlength;
	return 0;
}

/* Parse an incoming dns message of length blen into the struct mdns_message m.
 * Return 0 for success, -1 for failure.
 */
int mdns_parse_message(struct mdns_message *m, int mlen)
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
	m->num_authorities = ntohs(m->header->nscount);
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
		len = dname_size(m->cur);
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
		len = parse_resource(m, &m->answers[i]);
		if (len == -1) {
			DBG("Failed to parse answer %d\n", i);
			return -1;
		}
	}

	if (m->num_authorities > MDNS_MAX_AUTHORITIES) {
		LOG("Warning: Only parsing first %d authorities of %d\n",
			MDNS_MAX_ANSWERS, m->num_answers);
		m->num_answers = MDNS_MAX_AUTHORITIES;
	}
	for(i = 0; i < m->num_authorities; i++) {
		len = parse_resource(m, &m->authorities[i]);
		if (len == -1) {
			DBG("Failed to parse authority %d.\n", i);
			return -1;
		}
	}

	if (ntohs(m->header->arcount) != 0)
		LOG("Warning: Ignroing additional RRs\n");

	return 0;
}

/* prepare an empty query message m.  Return 0 for success and -1 for
 * failure.
 */
int mdns_query_init(struct mdns_message *m)
{
	m->len = sizeof(m->data);
	m->header = (struct mdns_header *)m->data;
	m->cur = m->data + sizeof(struct mdns_header);
	m->end = m->data + sizeof(m->data) - 1;
	memset(m->header, 0x00, sizeof(struct mdns_header));
	m->num_questions = 0;
	m->num_answers = 0;
	m->num_authorities = 0;
	return 0;
}

/* prepare an empty response message m.  Return 0 for success and -1 for
 * failure.
 */
int mdns_response_init(struct mdns_message *m)
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
	uint16_t len = (uint16_t)dname_size(name);
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
int mdns_add_question(struct mdns_message *m, char *qname, uint16_t qtype,
					  uint16_t qclass)
{
	if (__mdns_add_tuple(m, qname, qtype, qclass, (uint32_t)-1) == -1)
		return -1;
	m->header->qdcount = htons(htons(m->header->qdcount) + 1);
	m->num_questions += 1;
	return 0;
}

/* add an answer to name to the message m.  Return 0 for success, -1 for
 * failure.  Note that this function does not add the resource record data.  It
 * just populates the common header.
 */
int mdns_add_answer(struct mdns_message *m, char *name, uint16_t type,
					uint16_t class, uint32_t ttl)
{
	if (__mdns_add_tuple(m, name, type, class, ttl))
		return -1;
	m->header->ancount = htons(htons(m->header->ancount) + 1);
	m->num_answers += 1;
	return 0;
}

/* add a proposed answer for name to the authority section of the message m.
 * Return 0 for success, -1 for failure.  Note that this function does not add
 * the resource record data.  It just populates the common header.
 */
int mdns_add_authority(struct mdns_message *m, char *name, uint16_t type,
					   uint16_t class, uint32_t ttl)
{
	if (__mdns_add_tuple(m, name, type, class, ttl))
		return -1;
	m->header->nscount = htons(htons(m->header->nscount) + 1);
	m->num_authorities += 1;
	return 0;
}

/* add a 4-byte record containing i to the message m.  This is used for A
 * records.
 */
int mdns_add_uint32(struct mdns_message *m, uint32_t i)
{
	CHECK_TAILROOM(m, sizeof(uint16_t) + sizeof(uint32_t));
	set_uint16(m->cur, sizeof(uint32_t));
	m->cur += sizeof(uint16_t);
	set_uint32(m->cur, i);
	m->cur += sizeof(uint32_t);
	return 0;
}

/* add a dns name containing name to the message m.  This is used for cname,
 * ns, and ptr.
 */
int mdns_add_name(struct mdns_message *m, char *name)
{
	int len = dname_size(name);
	CHECK_TAILROOM(m, len + sizeof(uint16_t));
	set_uint16(m->cur, len);
	m->cur += sizeof(uint16_t);
	memcpy(m->cur, name, len);
	m->cur += len;
	return 0;
}

/* add a txt string containing txt to the message m.  This is nearly identical
 * to mdns_add_name, but it doesn't have the trailing 0.  That is implied by the
 * length of the record.
 */
static int mdns_add_txt(struct mdns_message *m, char *txt, uint16_t len)
{
	CHECK_TAILROOM(m, len + sizeof(uint16_t));
	set_uint16(m->cur, len);
	m->cur += sizeof(uint16_t);
	memcpy(m->cur, txt, len);
	m->cur += len;
	return 0;
}

/* add a dns SRV record to message m with given priority, weight, port, and
 * target name.
 */
static int mdns_add_srv(struct mdns_message *m, uint16_t priority,
						uint16_t weight, uint16_t port, char *target)
{
	int len = dname_size(target);
	CHECK_TAILROOM(m, len + 4*sizeof(uint16_t));
	set_uint16(m->cur, len + 3*sizeof(uint16_t));
	m->cur += sizeof(uint16_t);
	set_uint16(m->cur, priority);
	m->cur += sizeof(uint16_t);
	set_uint16(m->cur, weight);
	m->cur += sizeof(uint16_t);
	set_uint16(m->cur, port);
	m->cur += sizeof(uint16_t);
	memcpy(m->cur, target, len);
	m->cur += len;
	return 0;
}

/* add all of the required RRs that represent the service s to the specified
 * section of the message m.  Return 0 for success or -1 for error.
 */
int mdns_add_srv_ptr_txt(struct mdns_message *m, struct mdns_service *s,
						 char *fqdn, int section, uint32_t ttl)
{

	if (section == MDNS_SECTION_ANSWERS) {
		/* If we're populating the answer section, we put all of the data */
		if (mdns_add_answer(m, s->fqsn, T_SRV, C_FLUSH, ttl) != 0 ||
			mdns_add_srv(m, 0, 0, s->port, fqdn) != 0)
			return -1;
		if (mdns_add_answer(m, s->ptrname, T_PTR, C_FLUSH, ttl) != 0 ||
			mdns_add_name(m, s->fqsn) != 0)
			return -1;
		if (s->keyvals) {
			if (mdns_add_answer(m, s->fqsn, T_TXT, C_FLUSH, ttl) != 0 ||
				mdns_add_txt(m, s->keyvals, s->kvlen) != 0)
				return -1;
		}

	} else if (section == MDNS_SECTION_AUTHORITIES) {
		/* For the authority section , we only need srv and txt */
		if (mdns_add_authority(m, s->fqsn, T_SRV, C_FLUSH, ttl) != 0 ||
			mdns_add_srv(m, 0, 0, s->port, fqdn) != 0)
			return -1;
		if (s->keyvals) {
			if (mdns_add_authority(m, s->fqsn, T_TXT, C_FLUSH, ttl) != 0 ||
				mdns_add_txt(m, s->keyvals, s->kvlen) != 0)
				return -1;
		}

	} else if (section == MDNS_SECTION_QUESTIONS) {
		/* we only add SRV records to the question section when we are probing.
		 * And in this case we just add the fqsn.
		 */
		return mdns_add_question(m, s->fqsn, T_ANY, C_IN);
	} else {
		return -1;
	}

	return 0;
}

int mdns_send_msg(struct mdns_message *m, int sock, short port)
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

int mdns_send_ctrl_msg(int msg, uint16_t port)
{
	int ret;
	struct sockaddr_in to;
	int s;

	s = mdns_socket_loopback(htons(port), 0);
	if (s < 0) {
		LOG("error: failed to create loopback socket\n");
		return -1;
	}

	memset((char *)&to, 0, sizeof(to));
	to.sin_family = PF_INET;
	to.sin_port = htons(port);
	to.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = sendto(s, &msg, sizeof(msg), 0, (struct sockaddr *)&to, sizeof(to));
	if (ret != -1)
		ret = 0;

	mdns_socket_close(s);
	return ret;
}

/* calculate the interval between the start and stop timestamps accounting for
 * wrap.
 */
uint32_t interval(uint32_t start, uint32_t stop)
{
	if (stop >= start)
		return stop - start;
	return UINT32_MAX - start + stop;
}

/* We wanted to timeout after "target" ms, but we got interrupted.  Calculate
 * the new timeout "t" given that we started our timeout at "start" and were
 * interupted at "stop".
 */
void recalc_timeout(struct timeval *t, uint32_t start, uint32_t stop,
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

int mdns_launch(uint32_t ipaddr, char *domain, char *hostname,
				struct mdns_service **services)
{
	int ret;

	if (hostname) {
		ret = responder_launch(ipaddr, domain, hostname, services);
		if (ret != 0)
			return ret;
	}
	return 0;
}

void mdns_halt(void)
{
	LOG("Halting mdns.\n");
	responder_halt();
}

#ifdef MDNS_TESTS
void mdns_tests(void)
{
	dname_tests();
	responder_tests();
}
#else
void mdns_tests(void)
{
	return;
}
#endif
