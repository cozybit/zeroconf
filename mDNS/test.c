
#ifdef MDNS_HOST_TEST
#include "host.h"
#else
#include "target.h"
#endif

#include "mdns.h"

int main( void )
{
	int len = 0, size;
	char tx_buffer[1000];
	struct mdns_message tx_message;
    int sock;
    int one = 1;
    int i_ttl = 255;
    unsigned char c_ttl = 255;
    struct sockaddr_in in_addr;
    struct ip_mreq mc;
	struct sockaddr_in to;

    memset( &in_addr, 0x00, sizeof(in_addr) );
    in_addr.sin_family = AF_INET;
    in_addr.sin_port = htons(5353);
    in_addr.sin_addr.s_addr = 0;

    sock = socket( AF_INET, SOCK_DGRAM, 0 );
    if( sock < 0 )
        return sock;

	#ifdef SO_REUSEPORT
    setsockopt( sock, SOL_SOCKET, SO_REUSEPORT, (char*)&one, sizeof(one) );
    #endif
	setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(one) );

    if( bind( sock, (struct sockaddr*)&in_addr, sizeof(in_addr) ) ) {
        socket_close( sock );
        return 0;
    }

    /* set up multicast address */
    mc.imr_multiaddr.s_addr = inet_addr("224.0.0.251"); /* TODO: set directly*/
    mc.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt( sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mc, sizeof(mc) );
    setsockopt( sock, IPPROTO_IP, IP_MULTICAST_TTL, &c_ttl, sizeof(c_ttl) );
    setsockopt( sock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&i_ttl,
                sizeof(i_ttl) );

	socket_blocking_off( sock );
	if( sock < 0 ) {
		DB_PRINT( "error: unable to open multicast socket\n" );
		return 1;
	}

	mdns_transmit_init( &tx_message, tx_buffer );
	mdns_add_question( &tx_message, "\5""aloha""\xC""_workstation""\4""_tcp"
		"\5""local", T_ANY, 1 );
	mdns_add_question( &tx_message, "\5""hello""\x5""_http""\4""_tcp"
		"\5""local", T_ANY, 1 );
	
	printf( "sending...\n" );
	size = (unsigned int)tx_message.cur - (unsigned int)tx_message.header;
	to.sin_family = AF_INET;
	to.sin_port = htons(5353);
	to.sin_addr.s_addr = inet_addr("224.0.0.251");
	len = sendto( sock, tx_buffer, size, 0, (struct sockaddr *)&to,
		sizeof(struct sockaddr_in) );

	if( len < size )
		printf( "error: %d (%d)\n", len, size );

	close( sock );
	return 0;
}

