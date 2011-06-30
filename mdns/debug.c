/*
 * Copyright (C) 2007-2011 cozybit Inc.
 *
 * This file is part of libmdns.
 *
 * libmdns is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * libmdns is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libmdns. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "mdns_private.h"

#ifdef MDNS_DBG
char *statenames[] = {
	"INIT",
	"FIRST_PROBE_SENT",
	"SECOND_PROBE_SENT",
	"THIRD_PROBE_SENT",
	"READY_TO_RESPOND",
	"READY_TO_SEND",
};

char *eventnames[] = {
	"EVENT_RX",
	"EVENT_CTRL",
	"EVENT_TIMEOUT",
};

void debug_print_ip(uint32_t ip)
{
	DBG("%u.%u.%u.%u",
		ip>>24,
		((ip & 0x00FF0000)>>16),
		((ip & 0x0000FF00)>>8),
		ip & 0x000000FF
		);
}

void debug_print_txt(char *txt, uint16_t len)
{
	uint16_t i;
	for(i = 0; i < len; i++)
		if (txt[i] < ' ')
			DBG("\\%d", txt[i]);
		else
			DBG("%c", txt[i]);
}

/* print a RFC-1035 format domain name */
void debug_print_name(struct mdns_message *m, uint8_t *name)
{
	uint8_t *s = name;
	int first = 1;

	while(*s) {
		if(IS_POINTER(*s)) { /* pointer */
			if(m == NULL)
				break;
			/* go print at start of message+offset */
			s = m->data + POINTER(s);
			continue;
		}
		else { /* label */
			if (!first) {
				DBG(".");
			}
			first = 0;
			debug_print_txt((char *)s + 1, *s);
			s += *s;
		}
		s++;
	}
}

/* print question (query) data */
void debug_print_question(struct mdns_message *m, struct mdns_question *q)
{
	debug_print_name(m, q->qname);
	DBG(" (type %u, class %u)\n", q->qtype, q->qclass);
}

/* print resource (answer) and associated RR */
void debug_print_resource(struct mdns_message *m, struct mdns_resource *r)
{
	struct rr_srv *srv;

	debug_print_name(m, r->name);
	DBG(" (class %u%s, ttl=%u, len=%u) ",
		r->class & 0x8000 ? r->class & ~(0x8000) : r->class,
		r->class & 0x8000 ? " FLUSH" : "", r->ttl, r->rdlength);
	switch(r->type) {
	case T_A:
		DBG("A ");
		debug_print_ip(ntohl(*((uint32_t*)r->rdata)));
		break;
	case T_NS:
		DBG("NS ");
		debug_print_name(m, (uint8_t *)r->rdata);
		break;
	case T_CNAME:
		DBG("CNAME ");
		debug_print_name(m, (uint8_t *)r->rdata);
		break;
	case T_SRV:
		DBG("SRV ");
		srv = (struct rr_srv *)r->rdata;
		DBG("prior: %u, weight: %u, port: %u, target: \"",
			ntohs(srv->priority), ntohs(srv->weight), ntohs(srv->port));
		debug_print_name(m, srv->target);
		DBG("\"\n");
		break;
	case T_PTR:
		DBG("PTR ");
		debug_print_name(m, (uint8_t *)r->rdata);
		break;
	case T_TXT:
		DBG("TXT \"");
		debug_print_txt((char *)r->rdata, r->rdlength);
		DBG("\"\n");
		break;
	default:
		DBG("\tunknown\n");
		break;
	}
	DBG("\n");
}

void debug_print_header(struct mdns_header *h)
{
	DBG("ID=%u, QR=%u, AA=%d OPCODE=%u\n"
		"QDCOUNT=%u, ANCOUNT=%u, NSCOUNT=%u, ARCOUNT=%u\n",
		ntohs(h->id), h->flags.fields.qr,
		h->flags.fields.aa, h->flags.fields.opcode,
		ntohs(h->qdcount), ntohs(h->ancount),
		ntohs(h->nscount), ntohs(h->arcount));
}

/* print information about a message: header, questions, answers */
void debug_print_message(struct mdns_message *m)
{
	int i;

	DBG("########################################################\n");
	DBG("HEADER;\n");
	debug_print_header(m->header);

	DBG("QUESTION;\n");
	for(i = 0; i < m->num_questions; i++)
		debug_print_question(m, &m->questions[i]);

	DBG("ANSWER;\n");
	for(i = 0; i < m->num_answers; i++)
		debug_print_resource(m, &m->answers[i]);

	DBG("AUTHORITY;\n");
	for(i = 0; i < m->num_authorities; i++)
		debug_print_resource(m, &m->authorities[i]);
	DBG("########################################################\n");
}

#endif /* MDNS_DBG */
