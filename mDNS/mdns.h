#ifndef MDNS_H
#define MDNS_H

#include "mdns_message.h"

UINT16 mdns_read_n16( struct mdns_message *m );
UINT32 mdns_read_n32( struct mdns_message *m );
void mdns_write_n16( struct mdns_message *m, UINT16 n );
void mdns_write_n32( struct mdns_message *m, UINT32 n );

void mdns_mark_response( struct mdns_message *m );
int mdns_parse_message( struct mdns_message *m, char *b );
void debug_print_message( struct mdns_message *m );
void mdns_transmit_init( struct mdns_message *m, char *b );
void mdns_add_question( struct mdns_message *m, const char* qname, 
	UINT16 qtype, UINT16 qclass );
void mdns_add_answer( struct mdns_message *m, const char *name, UINT16 type,
	UINT16 class, UINT32 ttl, UINT16 length, void *data );
#endif
