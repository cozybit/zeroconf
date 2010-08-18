/*
 * Copyright (C) cozybit, Inc. 2007-2010. All Rights Reserved.
 *
 * Use and redistribution subject to licensing terms.
 */
#include "mdns.h"
#include "mdns_private.h"
#include "mdns_port.h"

static char *hname; /* this is a cstring, not a DNS string */
static char *domname; /* this is a cstring, not a DNS string */
/* Our fully qualified domain name is something like 'node.local.' */
static char fqdn[MDNS_MAX_NAME_LEN + 1];
/* Our reverse name for the in-addr.arpa PTR record */
static char in_addr_arpa[MDNS_INADDRARPA_LEN];
static uint32_t my_ipaddr;
struct mdns_service **user_services;

/* global mdns state */
static void *mdns_thread;
static int mc_sock;
static int ctrl_sock;
static int mdns_enabled;

/* control message types */
#define MDNS_CTRL_HALT 0

static struct mdns_message tx_message;
static struct mdns_message rx_message;

/* reset the fqdn to hostname.domain.
 */
static void reset_fqdn(void)
{
	char *p;
	p = dname_put_label(fqdn, hname);
	dname_put_label(p, domname);
}

/* reset the internal fully qualified service name of s to the values supplied
 * by the user at launch time.
 */
static void reset_fqsn(struct mdns_service *s)
{
	char *p;

	s->ptrname = dname_put_label(s->fqsn, s->servname);
	/* we have to be a bit tricky when adding the service type, because we have
	 * to append a leading '_' and adjust the length.
	 */
	p = dname_put_label(s->ptrname + 1, s->servtype);
	*s->ptrname = *(s->ptrname + 1) + 1;
	*(s->ptrname + 1) = '_';

	p = dname_put_label(p, s->proto == MDNS_PROTO_TCP ? "_tcp" : "_udp");

	dname_put_label(p, domname);
}

/* return the amount of tail room in the message m */
#define TAILROOM(m) ((m)->end - (m)->cur + 1)

/* ensure that message m has at least l bytes of room left */
#define CHECK_TAILROOM(m, l)										  \
	do {															  \
		if (TAILROOM(m) < l) {										  \
			DBG("Warning: truncated mdns message (%d).\n", __LINE__); \
			return -1;												  \
		}															  \
	} while (0)

/* return the number of valid bytes in message m's buffer */
#define VALID_LENGTH(m) ((m)->cur - (m)->data)

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
static int mdns_query_init(struct mdns_message *m)
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
static int mdns_add_question(struct mdns_message *m, char *qname,
							 uint16_t qtype, uint16_t qclass)
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
static int mdns_add_answer(struct mdns_message *m, char *name, uint16_t type,
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
static int mdns_add_authority(struct mdns_message *m, char *name,
							  uint16_t type, uint16_t class, uint32_t ttl)
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
 * ns, and ptr.
 */
static int mdns_add_name(struct mdns_message *m, char *name)
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
						 int section, uint32_t ttl)
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

/* add all of the services that we have to the specified section of the message
 * m.  Return 0 for success or -1 for error.
 */
int mdns_add_all_services(struct mdns_message *m, int section, uint32_t ttl)
{
	struct mdns_service **s;

	for (s = user_services; s != NULL && *s != NULL; s++)
		if (mdns_add_srv_ptr_txt(m, *s, section, ttl) != 0)
			return -1;
	return 0;
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

	s = mdns_socket_loopback(htons(MDNS_CTRL_RESPONDER), 0);
	if (s < 0) {
		LOG("error: failed to create loopback socket\n");
		return -1;
	}

	memset((char *)&to, 0, sizeof(to));
	to.sin_family = PF_INET;
	to.sin_port = htons(MDNS_CTRL_RESPONDER);
	to.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = sendto(s, &msg, sizeof(msg), 0, (struct sockaddr *)&to, sizeof(to));
	if (ret != -1)
		ret = 0;

	mdns_socket_close(s);
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
		mdns_add_all_services(m, MDNS_SECTION_QUESTIONS, 0) != 0 ||
		mdns_add_authority(m, fqdn, T_A, C_FLUSH, 255) != 0 ||
		mdns_add_uint32(m, htonl(my_ipaddr)) != 0 ||
		mdns_add_authority(m, in_addr_arpa, T_PTR, C_FLUSH, 255) != 0 ||
		mdns_add_name(m, fqdn) != 0 ||
		mdns_add_all_services(m, MDNS_SECTION_AUTHORITIES, 255) != 0)
	{
		LOG("Resource records don't fit into probe packet.\n");
		return -1;
	}
	/* populate the internal data structures of m so we can easily compare this
	 * probe to a probe response later.  Be sure to leave the end pointer at
	 * the end of our buffer so we can grow the packet if necessary.
	 */
	mdns_parse_message(m, VALID_LENGTH(m));
	m->end = m->data + sizeof(m->data) - 1;
	return 0;
}

static int mdns_prepare_announcement(struct mdns_message *m, uint32_t ttl)
{
	mdns_response_init(m);
	if (mdns_add_answer(m, fqdn, T_A, C_FLUSH, ttl) != 0 ||
		mdns_add_uint32(m, htonl(my_ipaddr)) != 0 ||
		mdns_add_answer(m, in_addr_arpa, T_PTR, C_FLUSH, ttl) != 0 ||
		mdns_add_name(m, fqdn) != 0 ||
		mdns_add_all_services(m, MDNS_SECTION_ANSWERS, ttl) != 0) {
		/* This is highly unlikely */
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

/* if we detect a conflict during probe time, we must grow our host name and
 * service names with -2, or -3, etc.  Report the maximum number of extra bytes
 * that we would need to do this.
 */
static int max_response_growth(int num_services)
{
	/* We need 2 bytes for each instance of the host name that appears in the:
	 * Query part:
	 * +2 A record q
	 * +2 * num services
	 *
	 * Answer part:
	 * +2 A record a
	 * +2 inaddrarpa PTR
	 * +2 * num services SRV
	 * +2 * num services SRV PTR
	 * +2 * num services TX
	 */

	return 2*3 + 4*2*num_services;
}

/* prepare a response to the message rx.  Put it in tx.
 * Return RS_ERROR (-1) for error
 *        RS_NO_SEND (0) for nothing to send
 *        or bit 0 (RS_SEND) set if a response needs to be sent
 *        and/or bit 1 (RS_SEND_DELAY) set if the response needs to be sent
 *        with a delay
 */
static int mdns_prepare_response(struct mdns_message *rx,
                                 struct mdns_message *tx)
{
	int i, srv_ret = 0;
	int ret = RS_NO_SEND;
	struct mdns_question *q;
	struct mdns_service **s;

	if (rx->header->flags.fields.qr != QUERY)
		return RS_NO_SEND;

	mdns_response_init(tx);
	tx->header->id = rx->header->id;

	for(i = 0; i < rx->num_questions; i++) {
		q = &rx->questions[i];

		if (q->qtype == T_ANY || q->qtype == T_A) {
			if (dname_cmp(rx->data, q->qname, NULL, fqdn) == 0) {
				if (mdns_add_answer(tx, fqdn, T_A, C_FLUSH, 255) != 0 ||
					mdns_add_uint32(tx, htonl(my_ipaddr)) != 0)
					return RS_ERROR;
				ret |= RS_SEND;
			}
		}

		if (q->qtype == T_ANY || q->qtype == T_PTR) {
			if (dname_cmp(rx->data, q->qname, NULL, in_addr_arpa) == 0) {
				if (mdns_add_answer(tx, in_addr_arpa, T_PTR, C_FLUSH, 255) != 0 ||
					mdns_add_name(tx, fqdn) != 0)
					return RS_ERROR;
				ret |= RS_SEND;
			}
			/* if the querier wants PTRs to services that we offer, add those */
			for (s = user_services; s != NULL && *s != NULL; s++) {
				if (dname_cmp(rx->data, q->qname, NULL, (*s)->ptrname) == 0 ||
					dname_cmp(rx->data, q->qname, NULL, (*s)->fqsn) == 0) {
					srv_ret = mdns_add_srv_ptr_txt(tx, *s, MDNS_SECTION_ANSWERS, 255);
					if (srv_ret != 0)
						return RS_ERROR;
					ret |= RS_SEND_DELAY;
				}
			}
		}
	}
	return ret;
}

/* Build a response packet to check that everything will fit in the packet.
 * First need to build the query and then will simulate the response.
 */
static int mdns_check_max_response(struct mdns_message *rx, struct mdns_message *tx)
{
	mdns_query_init(rx);
	if (mdns_add_question(rx, fqdn, T_ANY, C_IN) != 0 ||
		mdns_add_question(rx, in_addr_arpa, T_ANY, C_IN) != 0 ||
		mdns_add_all_services(rx, MDNS_SECTION_QUESTIONS, 0) != 0 )
	{
		LOG("Resource records don't fit into query of response packet.\n");
		return -1;
	}

	if (mdns_parse_message(rx, VALID_LENGTH(rx)) != 0)	{
		LOG("Failed to parse biggest expected query.\n");
		return -1;
	}

	if (mdns_prepare_response(rx, tx) < RS_SEND) {
		LOG("Resource records don't fit into response packet.\n");
		return -1;
	}

	return 0;
}

/* find all of the authority records in the message m that have the specified
 * name and return up to "size" of them sorted in order of increasing type.
 * This allows us to compare them in lexicographically increasing order as
 * required by the spec.  Return the number of valid records in the list.
 * "name" must be a complete dname without any pointers.
 */

/* in practice, we only expect a subset of records.  So we just make an array
 * of these to simplify the sorting.
 */
enum type_indicies {
	A_INDEX = 0,
	CNAME_INDEX,
	PTR_INDEX,
	TXT_INDEX,
	SRV_INDEX,
	MAX_INDEX,
};

static int find_authorities(struct mdns_message *m, char *name,
							struct mdns_resource *results[MAX_INDEX])
{
	int i, n = 0;

	for (i = 0; i < MAX_INDEX; i++)
		results[i] = 0;

	for(i = 0; i < m->num_authorities; i++) {

		if (dname_cmp(m->data, m->authorities[i].name, NULL, name) != 0)
			continue;

		/* okay.  The authority matches.  Add it to the results */
		n++;
		switch (m->authorities[i].type) {
		case T_A:
			results[A_INDEX] = &m->authorities[i];
			break;
		case T_CNAME:
			results[CNAME_INDEX] = &m->authorities[i];
			break;
		case T_PTR:
			results[PTR_INDEX] = &m->authorities[i];
			break;
		case T_TXT:
			results[TXT_INDEX] = &m->authorities[i];
			break;
		case T_SRV:
			results[SRV_INDEX] = &m->authorities[i];
			break;
		default:
			LOG("Warning: unexpected record of type %d\n",
				m->authorities[i].type);
			n--;
		}
	}
	return n;
}

/* is the resource a from message ma "lexicographically later" than b, earlier
 * than b, or equal to b?  Return 1, 0, or -1 respectively.
 */
static int rr_cmp(struct mdns_message *ma, struct mdns_resource *a,
				  struct mdns_message *mb, struct mdns_resource *b)
{
	int min, i, ret = 0;
	struct rr_srv *sa, *sb;

	/* check the fields in the order specified (class, type, then rdata).
	 * Normally, we would start with the name of the resource, but by the time
	 * we call this function, we know they are equal.
	 */
	if ((a->class & ~0x8000) > (b->class & ~0x8000))
		return 1;

	if ((a->class & ~0x8000) < (b->class & ~0x8000))
		return -1;

	if (a->type > b->type)
		return 1;

	if (a->type < b->type)
		return -1;

	/* Okay.  So far, everything is the same.  Now we must check the rdata.
	 * This part depends on the record type because we have to decompress any
	 * names.
	 */
	switch (a->type) {
	case T_A:
		/* A record always contains a 4-byte IP address */
		ret = memcmp(a->rdata, b->rdata, 4);
		break;

	case T_CNAME:
	case T_PTR:
	case T_TXT:
		/* some records just have a dname */
		return dname_cmp(ma->data, a->rdata, mb->data, b->rdata);
		break;

	case T_SRV:
		/* first check the fixed part of the record */
		ret = memcmp(a->rdata, b->rdata, sizeof(struct rr_srv));
		if (ret != 0)
			break;
		sa = (struct rr_srv *)a->rdata;
		sb = (struct rr_srv *)b->rdata;
		return dname_cmp(ma->data, sa->target, mb->data, sb->target);
		break;

	default:
		LOG("Warning: Unexpected RR type in rr_cmp: %d\n", a->type);
		min = a->rdlength > b->rdlength ? b->rdlength : a->rdlength;
		for (i = 0; i < min; i++) {
			if (((unsigned char *)a->rdata)[i] > ((unsigned char *)b->rdata)[i])
				return 1;
			if (((unsigned char *)a->rdata)[i] > ((unsigned char *)b->rdata)[i])
				return -1;
		}
		/* if we get all the way here, one rdata is a substring of the other.  The
		 * longer one wins.
		 */
		if (a->rdlength > b->rdlength)
			return 1;
		if (a->rdlength < b->rdlength)
			return -1;
	}
	ret = ret > 0 ? 1 : ret;
	ret = ret < 0 ? -1 : ret;
	return ret;
}

/* Is the authority record set in p1 with the specified name greater, equal, or
 * lesser than the equivalent authority record set in p2?  Return 1, 0, or -1
 * respectively.
 */
static int authority_set_cmp(struct mdns_message *p1, struct mdns_message *p2,
							 char *name)
{
	struct mdns_resource *r1[MAX_INDEX], *r2[MAX_INDEX];
	int ret = 0, i1 = 0, i2 = 0, n1, n2;

	/* The spec says we have to check the records in increasing lexicographic
	 * order.  So get a list sorted as such.
	 */
	n1 = find_authorities(p1, name, r1);
	n2 = find_authorities(p2, name, r2);
	if (n1 == 0 && n2 == 0)
		return 0;

	/* now check them in order of increasing type */
	while (1) {
		/* advance to the next valid record */
		while (i1 < MAX_INDEX && r1[i1] == NULL)
			i1++;

		while (i2 < MAX_INDEX && r2[i2] == NULL)
			i2++;

		if (i1 == MAX_INDEX && i2 == MAX_INDEX) {
			/* The record sets are absolutely identical. */
			return 0;
		}

		if (i1 == MAX_INDEX) {
			/* we've gone through all of p1's records and p2 still has records.
			 * This means p2 is greater.
			 */
			return -1;
		}

		if (i2 == MAX_INDEX) {
			/* we've gone through all of p2's records and p1 still has records.
			 * This means p1 is greater.
			 */
			return 1;
		}

		ret = rr_cmp(p1, r1[i1], p2, r2[i2]);
		if (ret != 0)
			return ret;
		i1++;
		i2++;
	}
	return ret;
}

/* does the incoming message m conflict with our probe p?  If so, alter the
 * service/host names as necessary and return 1.  Otherwise return 0.  If we've
 * seen so many conflicts that we have tried all the possible names, return -1;
 */
static int fix_response_conflicts(struct mdns_message *m,
								  struct mdns_message *p)
{
	struct mdns_resource *r;
	struct mdns_question *q;
	struct mdns_service **s;
	int ret = 0, i, len, cmp;

	if (m->header->flags.fields.qr == QUERY && m->num_authorities > 0) {
		/* We only want to check the authorities of service names once because
		 * it is quite costly.  So we use the SERVICE_CHECKED_FLAG to know if
		 * we've already checked a service.  We start with the flag cleared.
		 */
		for (s = user_services; s != NULL && *s != NULL; s++)
			(*s)->flags &= ~(SERVICE_CHECKED_FLAG);

		/* is this a probe from somebody with conflicting records? */
		for(i = 0; i < m->num_questions; i++) {
			q = &m->questions[i];

			/* ignore PTR records.  Other people are allowed to have PTR records
			 * with the same name as ours.
			 */
			if (q->qtype == T_PTR)
				continue;

			if (dname_cmp(m->data, q->qname, NULL, fqdn) == 0) {
				/* somebody wants our hostname. Check the authorities. */
				cmp = authority_set_cmp(m, p, fqdn);
				if (cmp == 1) {
					/* their authoritative RR is greater than ours */
					if (ret == 0)
						ret = 1;
					if (dname_increment(fqdn) == -1) {
						reset_fqdn();
						ret = -1;
					}
				}
			}

			for (s = user_services; s != NULL && *s != NULL; s++) {

				if ((*s)->flags & SERVICE_CHECKED_FLAG)
					continue;

				if (dname_cmp(m->data, q->qname, NULL, (*s)->fqsn) == 0) {
					/* somebody wants one of our service names. */
					cmp = authority_set_cmp(m, p, (*s)->fqsn);
					if (cmp == 1) {
						/* their authoritative RR is greater than ours */
						if (ret == 0)
							ret = 1;
						len = dname_size((*s)->fqsn);
						if (dname_increment((*s)->fqsn) == -1) {
							reset_fqsn(*s);
							ret = -1;
						}
						/* remember that we must increment the pointer to the fully
						 * qualified service type if we added bytes to the service
						 * name.
						 */
						if (dname_size((*s)->fqsn) > len)
							(*s)->ptrname += 2;
					}
					(*s)->flags |= SERVICE_CHECKED_FLAG;
				}
			}
		}
		return ret;
	}

	/* otherwise, this is a response.  Is it a response to our probe? */
	for(i = 0; i < m->num_answers; i++) {
		r = &m->answers[i];
		if (r->type == T_A && dname_cmp(m->data, r->name, NULL, fqdn) == 0) {
			/* try a different name */
			if (ret == 0)
				ret = 1;
			if (dname_increment(fqdn) == -1) {
				reset_fqdn();
				ret = -1;
			}
		}
		/* ensure that none of the service names conflict. */
		if (r->type == T_SRV) {
			for (s = user_services; s != NULL && *s != NULL; s++) {
				if (dname_cmp(m->data, r->name, NULL, (*s)->fqsn) == 0) {
					/* try a different service name */
					if (ret == 0)
						ret = 1;
					len = dname_size((*s)->fqsn);
					if (dname_increment((*s)->fqsn) == -1) {
						reset_fqsn(*s);
						ret = -1;
					}
					/* remember that we must increment the pointer to the fully
					 * qualified service type if we added bytes to the service
					 * name.
					 */
					if (dname_size((*s)->fqsn) > len)
						(*s)->ptrname += 2;
				}
			}
		}
	}

	if (ret == 1) {
		DBG("Detected conflict with this response:\n");
		debug_print_message(m);
		DBG("\n");
	}

	return ret;
}

/* dumb macro to set a struct timeval to "ms" milliseconds. */
#define SET_TIMEOUT(t, ms)								\
	do {												\
		(t)->tv_sec = (ms)/1000;						\
		(t)->tv_usec = ((ms)%1000) * 1000;				\
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


/* We're in a probe state sending the probe tx, and we got a probe response
 * (rx).  Process it and return the next state.  Also, update the timeout with
 * the time until the next event.  state must be one of the probe states!
 */
static int process_probe_resp(struct mdns_message *tx, struct mdns_message *rx,
							  int state, struct timeval *timeout,
							  uint32_t start_wait, uint32_t stop_wait)
{
	int ret;

	ret = fix_response_conflicts(rx, tx);
	if (ret == 1) {
		/* there were conflicts with some of our names.  The names have been
		 * adapted.  Go back to square one.
		 */
		SET_TIMEOUT(timeout, 0);
		return INIT;

	} else if (ret == -1) {
		/* we've tried lots of names.  Where applicable, we've reset original
		 * names.  Sleep for 5s, then try again.
		 */
		DBG("Tried all possible names.  Resetting.\n");
		SET_TIMEOUT(timeout, 5000);
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
	int rand_time = 0;
	mdns_enabled = 1;

	/* Per specification, section 8.1, wait a random ammount of time between
	 * 0 and 250 ms before the first probe.
	 */
	rand_time = mdns_rand_range(250);
	SET_TIMEOUT(&probe_wait_time, rand_time);
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
			ret = recvfrom(ctrl_sock, &msg, sizeof(msg), MSG_DONTWAIT,
						   (struct sockaddr *)0, 0);
			if (ret == -1) {
				LOG("Warning: failed to get control message\n");
			} else {
				if (msg == MDNS_CTRL_HALT) {
					/* Send the goodbye packet.  This is same as announcement,
					 * but with a TTL of 0
					 */
					ret = mdns_prepare_announcement(&tx_message, 0);
					if (ret != 0)
						break;
					send_message(&tx_message, mc_sock, 5353);

					LOG("mdns done.\n");
					mdns_enabled = 0;
					break;
				}
			}
		}

		if (FD_ISSET(mc_sock, &fds)) {
			len = recvfrom(mc_sock, rx_message.data, sizeof(rx_message.data),
						   MSG_DONTWAIT, (struct sockaddr*)&from, &in_size);
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
				state = process_probe_resp(&tx_message, &rx_message, state,
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
				state = process_probe_resp(&tx_message, &rx_message, state,
										   &probe_wait_time, start_wait,
										   stop_wait);
			}
			timeout = &probe_wait_time;
			break;

		case THIRD_PROBE_SENT:
			if (event == EVENT_TIMEOUT) {
				/* Okay.  We now own our name.  Announce it. */
				ret = mdns_prepare_announcement(&tx_message, 255);
				if (ret != 0)
					break;
				send_message(&tx_message, mc_sock, 5353);
				timeout = NULL;
				state = READY_TO_RESPOND;

			} else if (event == EVENT_RX) {
				state = process_probe_resp(&tx_message, &rx_message, state,
										   &probe_wait_time, start_wait,
										   stop_wait);
				timeout = &probe_wait_time;
			}
			break;

		case READY_TO_RESPOND:
			if (event == EVENT_RX) {
				/* prepare a response if necessary */
				ret = mdns_prepare_response(&rx_message, &tx_message);
				if (ret <= RS_NO_SEND)
					break;

				if (ret & RS_SEND_DELAY) {
					/* Implment random delay from 20-120msec */
					DBG("delaying response\n");
					rand_time = mdns_rand_range(100);
					SET_TIMEOUT(&probe_wait_time, (rand_time+20));
					timeout = &probe_wait_time;
					state = READY_TO_SEND;
				} else if (ret & RS_SEND) {
					/* We send immedately */
					DBG("responding to query:\n");
					debug_print_message(&rx_message);
					send_message(&tx_message, mc_sock, from.sin_port);
				}
			}
			break;

		case READY_TO_SEND:
			/* We send, no matter if triggered by timeout or RX */
			DBG("responding to query:\n");
			debug_print_message(&rx_message);
			send_message(&tx_message, mc_sock, from.sin_port);

			if (event == EVENT_TIMEOUT) {
				/* Here due to timeout, so we're done, go back to RTR */
				timeout = NULL;
				state = READY_TO_RESPOND;
			} else if (event == EVENT_RX) {
				/* prepare a response if necessary */
				ret = mdns_prepare_response(&rx_message, &tx_message);

				if (ret <= RS_NO_SEND) {
					/* Error or otherwise no response, go back to RTR */
					timeout = NULL;
					state = READY_TO_RESPOND;
				} else if (ret & RS_SEND_DELAY) {
					/* Implment random delay from 20-120msec */
					DBG("delaying response\n");
					rand_time = mdns_rand_range(100);
					SET_TIMEOUT(&probe_wait_time, (rand_time+20));
					timeout = &probe_wait_time;
					state = READY_TO_SEND;
				} else if (ret & RS_SEND) {
					/* We send immedately */
					DBG("responding to query:\n");
					debug_print_message(&rx_message);
					send_message(&tx_message, mc_sock, from.sin_port);

					/* No longer have a message queued up, so go back to RTR */
					timeout = NULL;
					state = READY_TO_RESPOND;
				}
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

static int validate_service(struct mdns_service *s)
{
	int maxlen;
	char *src, *dest;

	if (!valid_label(s->servname)) {
		LOG("Invalid service name: %s\n", s->servname);
		return MDNS_INVAL;
	}

	if (!valid_label(s->servtype)) {
		LOG("Invalid service type: %s\n", s->servtype);
		return MDNS_INVAL;
	}

	if (s->proto != MDNS_PROTO_TCP && s->proto != MDNS_PROTO_UDP) {
		LOG("Invalid proto: %d\n", s->proto);
		return MDNS_INVAL;
	}

	maxlen = strlen(s->servname) + 1; /* +1 for label len */
	maxlen += strlen(s->servtype) + 2; /* +1 for label len, +1 for leading _ */
	maxlen += 4; /* for _tcp or _udp */
	maxlen += strlen(domname) + 1;
	maxlen += 2; /* may need to add a '-2' to the servname if it's not unique */
	maxlen += 1; /* and we need a terminating 0 */
	if (maxlen > MDNS_MAX_NAME_LEN)
		return MDNS_TOOBIG;

	/* Our TXT basically look like dnames but without the terminating \0.
	 * Instead of depending on this \0 to find the end, we record the length.
	 * This allows us to use that \0 space from the user to accomodate the
	 * leading length.
	 */
	if (s->keyvals != NULL) {

		/* now change all of the colons to lengths starting from the last
		 * char
		 */
		dest = s->keyvals + strlen(s->keyvals);
		src = dest - 1;
		s->kvlen = 0;
		maxlen = 0;

		while (1) {
			if (*src == ':' && maxlen == 0)
				return MDNS_BADSRV;

			if (dest == s->keyvals || *src == ':' ) {
				/* This is the beginning of a key/val.  Update its length and
				 * start looking at the next one.
				 */
				if (maxlen > MDNS_MAX_KEYVAL_LEN) {
					LOG("key/value exceeds MDNS_MAX_KEYVAL_LEN");
					return MDNS_BADSRV;
				}
				*dest = maxlen;

				if (UINT16_MAX - s->kvlen < maxlen)
					return MDNS_TOOBIG;

				s->kvlen += maxlen + 1;
				maxlen = 0;

				if (dest == s->keyvals)
					break;
				dest--;
				src--;
				continue;
			}
			/* move the char down */
			*dest-- = *src--;
			maxlen++;
		}
	}

	return 0;
}

static int responder_launch(uint32_t ipaddr, char *domain, char *hostname,
							struct mdns_service **services)
{
	int ret;
	int num_services = 0;

	my_ipaddr = ipaddr;

	/* populate the fqdn */
	if (domain == NULL)
		domain = "local";
	if (!valid_label(hostname) || !valid_label(domain)) {
		LOG("Invalid hostname: %s.%s\n", hostname, domain);
		return MDNS_INVAL;
	}
	/* populate fqdn */
	hname = hostname;
	domname = domain;
	reset_fqdn();

	ipaddr_to_inaddrarpa(htonl(ipaddr), in_addr_arpa);

	mc_sock = mdns_socket_mcast(inet_addr("224.0.0.251"), htons(5353));
	if (mc_sock < 0) {
		LOG("error: unable to open multicast socket\n");
		return mc_sock;
	}

	user_services = NULL;
	num_services = 0;
	if (services != NULL) {
		user_services = services;
		for (; *services != NULL; services++) {
			ret = validate_service(*services);
			if (ret != 0)
				return ret;
			reset_fqsn(*services);
			num_services++;
		}
	}

	/* check to make sure all of our names and services will fit */
	ret = mdns_prepare_probe(&tx_message);
	if (ret == -1)
		return MDNS_TOOBIG;

	if (max_probe_growth(num_services) > TAILROOM(&tx_message)) {
		LOG("Insufficient space for host name and service names\n");
		return MDNS_TOOBIG;
	}

	ret = mdns_check_max_response(&rx_message, &tx_message);
	if (ret == -1) {
		LOG("Insufficient space for host name and service names in response\n");
		return MDNS_TOOBIG;
	}

	if (max_response_growth(num_services) > TAILROOM(&tx_message)) {
		LOG("Insufficient growth for host name and service names in response\n");
		return MDNS_TOOBIG;
	}

	/* create both ends of the control socket */
	ctrl_sock = mdns_socket_loopback(htons(MDNS_CTRL_RESPONDER), 1);

	socket(PF_INET, SOCK_DGRAM, 0);
	if (ctrl_sock < 0) {
		LOG("Failed to listening create control socket: %d\n", ctrl_sock);
		return -1;
	}

	mdns_thread = mdns_thread_create(do_mdns, NULL);
	if (mdns_thread == NULL)
		return -1;
	return 0;
}

void resolver_halt(void)
{
	int ret;

	ret = send_ctrl_msg(MDNS_CTRL_HALT);
	if (ret != 0) {
		LOG("Warning: failed to send HALT message to mdns: %d\n", errno);
	} else {
		mdns_thread_yield();
	}
	if (mdns_enabled != 0)
		LOG("Warning: failed to halt mdns.  Forcing.\n");

	mdns_thread_delete(mdns_thread);
	mdns_socket_close(ctrl_sock);
	mdns_socket_close(mc_sock);
	mdns_enabled = 0;
}

int mdns_launch(uint32_t ipaddr, char *domain, char *hostname,
				struct mdns_service **services)
{
	int ret;

	hname = NULL;
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
	resolver_halt();
}

#ifdef MDNS_TESTS
/* This is a probe packet with a single query for foo.local and an authority ns
 * with a name pointer to foo.local
 */
char pkt0[] = {0xE4, 0x53, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00,
			   0x00, 0x03, 0x66, 0x6F, 0x6F, 0x05, 0x6C, 0x6F, 0x63, 0x61, 0x6C,
			   0x00, 0x00, 0x01, 0x00, 0x01, 0xC0, 0x0C, 0x00, 0x01, 0x00, 0x01,
			   0x00, 0x00, 0x00, 0xFF, 0x00, 0x04, 0xC0, 0xA8, 0x00, 0x07};

void message_parse_tests(void)
{
	int ret;
	test_title("mdns_parse_message");
	memcpy(rx_message.data, pkt0, sizeof(pkt0));
	ret = mdns_parse_message(&rx_message, sizeof(pkt0));
	FAIL_UNLESS(ret == 0, "Failed to parse probe-like packet");
	FAIL_UNLESS(VALID_LENGTH(&rx_message) == sizeof(pkt0),
				"parsed packet has unexpected length");
	
	/* prepare a probe, decode it, and check the values */
	my_ipaddr = ntohl(0xC0A80109);
	domname = "local";
	hname = "myname";
	reset_fqdn();
	ipaddr_to_inaddrarpa(my_ipaddr, in_addr_arpa);
	ret = mdns_prepare_probe(&rx_message);
	FAIL_UNLESS(ret == 0, "Failed to create probe packet");
	DBG("Created probe:\n");
	debug_print_message(&rx_message);
	FAIL_UNLESS(max_probe_growth(0) <= TAILROOM(&rx_message),
				"Insufficient tail room after creating probe");
	memcpy(tx_message.data, rx_message.data, sizeof(rx_message.data));
	ret = mdns_parse_message(&tx_message, sizeof(tx_message.data));
	FAIL_UNLESS(ret == 0, "Failed to parse probe packet");
	DBG("Parsed probe:\n");
	debug_print_message(&tx_message);
}

void mdns_tests(void)
{
	dname_tests();
	message_parse_tests();
}
#else
void mdns_tests(void)
{
	return;
}
#endif
