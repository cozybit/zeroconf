#ifndef MDNS_MESSAGE_H
#define MDNS_MESSAGE_H

#include <stdint.h>
/* settings */
#define MDNS_MAX_NAME_LEN	255	/* defined by the standard */
#define MDNS_MAX_LABEL_LEN	63	/* defined by the standard */
#define MDNS_MAX_QUESTIONS	32
#define MDNS_MAX_ANSWERS	10

/* class */
#define C_IN	0x0001	/* Internet */
#define C_FLUSH	0x8001	/* FLUSH */

/* types (that are used in mDNS) */
#define T_A		1	/* host address */
#define T_NS	2	/* authoritative name server */
#define T_CNAME	5	/* canonical name for an alias */
#define T_PTR	12	/* domain name pointer */
#define T_TXT	16	/* text strings */
#define T_SRV	33	/* service */
#define T_ANY	255	/* all records */

/* mDNS message header */
struct mdns_header {
	uint16_t id;
	union {
		struct { uint16_t
			rcode:4,	/* response code */
			cd:1,		/* checking disabled  RFC-2535*/
			ad:1,		/* authentic data RFC-2535 */
			z:1,		/* zero */
			ra:1,		/* recursion available */
			rd:1,		/* recursion desired */
			tc:1,		/* truncated */
			aa:1,		/* authoritative answer */
			opcode:4,	/* should be 0 for mDNS messages */
			qr:1;		/* query/response */
		} fields;
		uint16_t num;
	} flags;
	uint16_t qdcount; /* number of entries in questions section */
	uint16_t ancount; /* number of resource records in answer section */
	uint16_t nscount;
	uint16_t arcount;
};

/* Resource Record (RR) representations */
struct rr_a { uint32_t ip; };
struct rr_cname { char *name; };
struct rr_txt { char *data; };
struct rr_ns { char *name; };
struct rr_srv { uint16_t priority; uint16_t weight; uint16_t port; char *target; };
struct rr_ptr { char *name; };

/* Resource Record (RR) pointer */
union rr_p {
	struct rr_a *a;
	struct rr_cname *cname;
	struct rr_txt *txt;
	struct rr_ns *ns;
	struct rr_srv *srv;
	struct rr_ptr *ptr;
};

/* mDNS question (query) representation */
struct mdns_question {
	char *qname;
	uint16_t qtype; /* question type */
	uint16_t qclass; /* question class */
};

/* mDNS resource record (RR) format */
struct mdns_resource {
	char *name;
	uint16_t type; /* resource type for the RDATA field */
	uint16_t class; /* resource class for the RDATA field */
	uint32_t ttl; /* how long the record may be cached */
	uint16_t rdlength; /* length of RDATA field */
	void *rdata;
};

/* mDNS message representation */
struct mdns_message {
	struct mdns_header *header;
	char *cur;
	/* pointers to (and representations of) questions and answers */
	struct mdns_question questions[MDNS_MAX_QUESTIONS];
	struct mdns_resource answers[MDNS_MAX_ANSWERS];
	uint16_t num_questions, num_answers; /* for convenience */
};

struct mdns_rr {
	void(*transfer)( struct mdns_message *m, union rr_p r );
	uint16_t(*length)( union rr_p r );
	union rr_p data;
};

#define MDNS_IS_QUERY(msg)		(msg.header->flags.fields.qr == 0)
#define MDNS_IS_RESPONSE(msg)	(msg.header->flags.fields.qr == 1)

#endif
