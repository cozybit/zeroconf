#include "mdns_private.h"

#ifdef MDNS_DBG
char *statenames[] = {
	"INIT"
	"FIRST_PROBE_SENT",
	"SECOND_PROBE_SENT",
	"THIRD_PROBE_SENT",
	"IDLE",
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
		DBG("%c", txt[i]);
}

/* print a RFC-1035 format domain name */
void debug_print_name(struct mdns_message *m, char *name)
{
	char *s = name;
	uint8_t ptr = 0;

	while(*s) {
		if(*s & 0xC0) { /* pointer */
			if(ptr != 0 || m == NULL)
				break;
			/* go print at start of message+offset */
			s = (char *)m->header+((uint8_t)(((*s & ~(0xC0))<<8) | *(s+1)));
			ptr = 1;
			continue;
		}
		else { /* label */
			if (s != name)
				DBG(".");
			debug_print_txt(s+1, *s); /* print label text */
			s += *s;
		}
		s++;
	}
}

/* print question (query) data */
void debug_print_question(struct mdns_message *m, struct mdns_question *q)
{
	DBG("--------------------------------------------------------\n"
		"question: \"");
	debug_print_name(m, q->qname);
	DBG("\"\n(type %u, class %u)\n", q->qtype, q->qclass);
}

/* print resource (answer) and associated RR */
void debug_print_resource(struct mdns_message *m, struct mdns_resource *r)
{
	struct rr_srv *srv;

	DBG("--------------------------------------------------------\n"
		"resource \"");
	debug_print_name(m, r->name);
	DBG("\" (type %u, class %u%s)\n\tttl=%u, rdlength=%u\n",
		r->type,
		r->class & 0x8000 ? r->class & ~(0x8000) : r->class,
		r->class & 0x8000 ? " FLUSH" : "", r->ttl, r->rdlength);
	switch(r->type) {
	case T_A:
		DBG("\tA type, IP=");
		debug_print_ip(ntohl(*((uint32_t*)r->rdata)));
		DBG("\n");
		break;
	case T_NS:
		DBG("\tNS type, name=\"");
		debug_print_name(m, (char *)r->rdata);
		DBG("\"\n");
		break;
	case T_CNAME:
		DBG("\tCNAME type, name=\"");
		debug_print_name(m, (char *)r->rdata);
		DBG("\"\n");
		break;
	case T_SRV:
		DBG("\tSRV type, ");
		srv = (struct rr_srv *)r->rdata;
		DBG("priority: %u, weight: %u, port: %u, target: \"",
			ntohs(srv->priority), ntohs(srv->weight), ntohs(srv->port));
		debug_print_name(m, srv->target);
		DBG("\"\n");
		break;
	case T_PTR:
		DBG("\tPTR type, name=\"");
		debug_print_name(m, (char *)r->rdata);
		DBG("\"\n");
		break;
	case T_TXT:
		DBG("\tTXT type, data=\"");
		debug_print_txt((char *)r->rdata, r->rdlength);
		DBG("\"\n");
		break;
	default:
		DBG("\tunknown RR type\n");
		break;
	}
}

void debug_print_header(struct mdns_header *h)
{
	DBG("--------------------------------------------------------\n"
		"header:\nID=%u, QR=%u, AA=%d OPCODE=%u\n"
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
	debug_print_header(m->header);

	for(i = 0; i < m->num_questions; i++)
		debug_print_question(m, &m->questions[i]);

	for(i = 0; i < m->num_answers; i++)
		debug_print_resource(m, &m->answers[i]);
}
#endif /* MDNS_DBG */
