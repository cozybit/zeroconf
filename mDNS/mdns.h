#ifndef MDNS_H
#define MDNS_H

#include "mdns_message.h"

enum mdns_status_t {
	FIRST_PROBE,	/* wait a random amount of time and probe */
	SECOND_PROBE,	/* wait 250ms and probe */
	THIRD_PROVE,	/* wait 250ms and probe */
	ANNOUNCE,		/* send announcement message to claim name */
	STARTED			/* we have claimed our name */
};

UINT16 mdns_read_n16( struct mdns_message *m );
UINT32 mdns_read_n32( struct mdns_message *m );
void mdns_write_n16( struct mdns_message *m, UINT16 n );
void mdns_write_n32( struct mdns_message *m, UINT32 n );

void rr_transfer_a( struct mdns_message *m, union rr_p r );
void rr_transfer_cname( struct mdns_message *m, union rr_p r );
void rr_transfer_txt( struct mdns_message *m, union rr_p r );
void rr_transfer_ns( struct mdns_message *m, union rr_p r );
void rr_transfer_srv( struct mdns_message *m, union rr_p r );
void rr_transfer_ptr( struct mdns_message *m, union rr_p r );

UINT16 rr_length_a( union rr_p r );
UINT16 rr_length_cname( union rr_p r );
UINT16 rr_length_txt( union rr_p r );
UINT16 rr_length_ns( union rr_p r );
UINT16 rr_length_srv( union rr_p r );
UINT16 rr_length_ptr( union rr_p r );

UINT16 mdns_name_length( char *name );
void mdns_mark_response( struct mdns_message *m );
void mdns_mark_question( struct mdns_message *m );
int mdns_parse_message( struct mdns_message *m, char *b );
void debug_print_message( struct mdns_message *m );
void mdns_transmit_init( struct mdns_message *m, char *b );
void mdns_add_question( struct mdns_message *m, char* qname, 
	UINT16 qtype, UINT16 qclass );
void mdns_add_answer( struct mdns_message *m, char *name, UINT16 type,
	UINT16 class, UINT32 ttl, struct mdns_rr *rr );
#endif
