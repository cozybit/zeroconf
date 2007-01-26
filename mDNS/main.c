
#ifdef MDNS_HOST_TEST
#include "host.h"
#else
#include "target.h"
#endif

#include "mdns.h"
#include "mdns_config.h"
#include "debug.h"

int m_socket( void )
{
	int sock;
	int one = 1;
	int i_ttl = 255;
	unsigned char c_ttl = 255;
	struct sockaddr_in in_addr;
	struct ip_mreq mc;

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
	return sock;
}

int send_message( struct mdns_message *m, int sock )
{
	struct sockaddr_in to;
	int size, len;

	/* get message length */
	size = (unsigned int)m->cur - (unsigned int)m->header;

	to.sin_family = AF_INET;
	to.sin_port = htons(5353);
	to.sin_addr.s_addr = inet_addr("224.0.0.251");
	
	len = sendto( sock, (char *)m->header, size, 0, (struct sockaddr *)&to,
		sizeof(struct sockaddr_in) );

	if( len < size ) {
		DB_PRINT( "error: failed to send message\n" );
		return 0;
	}
	return 1;
}

int main( void )
{
	int mc_sock;
	int active_fds;
	fd_set fds;
	int len = 0;
	struct timeval timeout;
	char rx_buffer[1000];
	char tx_buffer[1000];
	struct mdns_message tx_message;
	struct mdns_message rx_message;
	struct sockaddr_in from;
	socklen_t in_size = sizeof(struct sockaddr_in);
	char target[] = "\xC""hello, world";

	UINT8 status = 0;
	UINT32 service_ip = htonl(0xC0A8011C); /* 192.168.1.28 */
	struct rr_srv service_srv;

	service_srv.priority = 0;
	service_srv.weight = 0;
	service_srv.port = htons(80);
	service_srv.target = target;

	mc_sock = m_socket();
	if( mc_sock < 0 ) {
		DB_PRINT( "error: unable to open multicast socket\n" );
		return 1;
	}

	/* set up probe to claim name */
	mdns_transmit_init( &tx_message, tx_buffer );
	mdns_mark_question( &tx_message );
	mdns_add_question( &tx_message, SERVICE_NAME SERVICE_TYPE, T_ANY, 1 );

	while( 1 ) {
		timeout.tv_sec = 0;
		timeout.tv_usec = 250000;
		FD_ZERO(&fds);
		FD_SET( mc_sock, &fds);
		active_fds = select( mc_sock+1, &fds, NULL, NULL, &timeout);

		if( active_fds < 0 ) {
			DB_PRINT( "error: select() failed\n" );
			return 1;
		}
	
		if( FD_ISSET( mc_sock, &fds ) ) {
			while( ( len = recvfrom( mc_sock, rx_buffer, 1000, 0, 
							(struct sockaddr*)&from, &in_size ) ) > 0 ) {
				if( mdns_parse_message( &rx_message, rx_buffer ) )
					debug_print_message( &rx_message );
			}
		}
		else if( status < STARTED ) {
			if( status == ANNOUNCE ) {
				mdns_transmit_init( &tx_message, tx_buffer );
				mdns_mark_response( &tx_message );
				mdns_add_answer( &tx_message, SERVICE_NAME SERVICE_TYPE, 
					T_A, 1, 225, sizeof(UINT32), &service_ip );
				mdns_add_answer( &tx_message, SERVICE_NAME SERVICE_TYPE, 
					T_SRV, 1, 225, sizeof(struct rr_srv)+strlen(target), 
					&service_srv );
			}
			send_message( &tx_message, mc_sock );
			status++;
		}
	}

	return 0;
}

