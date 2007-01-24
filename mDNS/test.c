#include "host.h"
#include "mdns.h"

int main( void )
{
	char buffer[1000];
	struct mdns_message tx_message;
	struct mdns_message rx_message;
	union mdns_rr rr;


	mdns_transmit_init( &tx_message, buffer );
	mdns_add_question( &tx_message, "\6andrey\4local", 255, 1 );
	mdns_add_question( &tx_message, "\4test\4local", 255, 1 );
	rr.a.ip = 0xC0A8011C;
	mdns_add_answer( &tx_message, "\4hello\5world", T_A, 1, 255, 
		sizeof(rr.a), &rr );

	mdns_parse_message( &rx_message, buffer );

	debug_print_message( &rx_message );

	return 0;
}
