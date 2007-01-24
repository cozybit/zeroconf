#ifndef MDNS_MESSAGE_H
#define MDNS_MESSAGE_H

/* settings */
#define MDNS_MAX_NAME_LEN	255
#define MDNS_MAX_QUESTIONS	8
#define MDNS_MAX_ANSWERS	8

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
    UINT16 id; 
    struct { UINT16
        qr:1,       /* query/response */
        opcode:4,
        aa:1,       /* authoritative answer */
        tc:1,       /* truncated */
        rd:1,       /* recursion desired */
        ra:1,       /* recursion available */
        z:1,        /* zero */
        ad:1,       /* authentic data RFC-2535 */
        cd:1,       /* checking disabled  RFC-2535*/
        rcode:4;    /* response code */
    } flags;
    UINT16 qdcount; /* number of entries in questions section */
    UINT16 ancount; /* number of resource records in answer section */
    UINT16 nscount;
    UINT16 arcount;
};

/* mDNS question (query) representation */
struct mdns_question {
	char *qname;
	UINT16 qtype; /* question type */
	UINT16 qclass; /* question class */
};

/* mDNS resource data (RDATA) */
union mdns_rr {
	struct { UINT32 ip; } a;
	struct { char *name; } ns;
	struct { char *name; } cname;
	struct { char *name; } ptr;
	struct { char *data; } txt;
	struct { char *name; } srv;
};

/* mDNS resource record (RR) format */
struct mdns_resource {
	char *name;
	UINT16 type; /* resource type for the RDATA field */
	UINT16 class; /* resource class for the RDATA field */
	UINT32 ttl; /* how long the record may be cached */
	UINT16 rdlength; /* length of RDATA field */
	union mdns_rr *rdata;
};

/* mDNS message representation */
struct mdns_message {
	struct mdns_header *header;
	char *cur;
	char *end;
	/* pointers to (and representations of) questions and answers */
	struct mdns_question questions[MDNS_MAX_QUESTIONS];
	struct mdns_resource answers[MDNS_MAX_ANSWERS];
	UINT16 num_questions, num_answers; /* for convenience */
};

#endif
