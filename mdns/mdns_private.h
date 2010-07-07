#include "mdns_message.h"

typedef enum { QUERY, RESPONSE } msg_type;

enum mdns_status_t {
	FIRST_PROBE,	/* wait a random amount of time and probe */
	SECOND_PROBE,	/* wait 250ms and probe */
	THIRD_PROBE,	/* wait 250ms and probe */
	ANNOUNCE,		/* send announcement message to claim name */
	STARTED			/* we have claimed our name */
};

/* RR transfer functions */
void rr_transfer_a(struct mdns_message *m, union rr_p r);
void rr_transfer_cname(struct mdns_message *m, union rr_p r);
void rr_transfer_txt(struct mdns_message *m, union rr_p r);
void rr_transfer_ns(struct mdns_message *m, union rr_p r);
void rr_transfer_srv(struct mdns_message *m, union rr_p r);
void rr_transfer_ptr(struct mdns_message *m, union rr_p r);

/* RR length functions */
uint16_t rr_length_a(union rr_p r);
uint16_t rr_length_cname(union rr_p r);
uint16_t rr_length_txt(union rr_p r);
uint16_t rr_length_ns(union rr_p r);
uint16_t rr_length_srv(union rr_p r);
uint16_t rr_length_ptr(union rr_p r);

void mdns_transmit_init(struct mdns_message *m, char *b, msg_type mtype);
int mdns_parse_message(struct mdns_message *m, char *b);
void mdns_add_question(struct mdns_message *m, char* qname, 
	uint16_t qtype, uint16_t qclass);
void mdns_add_answer(struct mdns_message *m, char *name, uint16_t type,
	uint16_t class, uint32_t ttl, struct mdns_rr *rr);

#define DB_PRINT(...) do { \
	printf(__VA_ARGS__); \
    fflush(stdout); \
} while (0)

void debug_print_message(struct mdns_message *m);
