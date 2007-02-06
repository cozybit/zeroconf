/******************************************************************************
 * mdns_responder.c
 *
 * 
 ******************************************************************************/

#include "system.h"
#include "mdns_responder.h"
#include "mdns.h"
#include "mdns_config.h" /* service name, etc */

static sys_thread mdns_main_thread;
static unsigned char mdns_stack[2048];
int mc_sock;

enum mdns_status_t {
    FIRST_PROBE,    /* wait a random amount of time (0-250ms) and probe */
    SECOND_PROBE,   /* wait 250ms and probe */
    THIRD_PROBE,    /* wait 250ms and probe */
    ANNOUNCE,       /* send announcement message to claim name */
    STARTED         /* we have claimed our name */
} mdns_status;

int m_socket( void )
{
    int sock;
    unsigned char yes = 1;
    unsigned char ttl = 255;
    struct sockaddr_in in_addr;
    struct ip_mreq mc;

	/* set up bind address */
    memset( &in_addr, 0x00, sizeof(in_addr) );
    in_addr.sin_family = AF_INET;
    in_addr.sin_port = htons(5353);
    in_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    sock = socket( AF_INET, SOCK_DGRAM, 0 );
    if( sock < 0 )
        return sock;

#ifdef SO_REUSEPORT
    setsockopt( sock, SOL_SOCKET, SO_REUSEPORT, (void *)&yes, sizeof(yes) );
#endif
    setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, (void *)&yes, sizeof(yes) );

    if( bind( sock, (struct sockaddr*)&in_addr, sizeof(in_addr) ) ) {
        DEBUG( "failed to bind multicast socket: %d\r\n", 
			tfGetSocketError(sock) );
		socket_close( sock );
        return -1;
    }

    /* join multicast group */
    mc.imr_multiaddr.s_addr = inet_addr("224.0.0.251");
    mc.imr_interface.s_addr = sys_get_ip(); //htonl(INADDR_ANY);
    if( setsockopt( sock, IP_PROTOIP, IPO_ADD_MEMBERSHIP, (void *)&mc, 
		sizeof(mc) ) < 0 ) {
		DEBUG( "failed to add multicast membership: %d\r\n", 
			tfGetSocketError( sock ) );
		socket_close( sock );
		return -1;
	}

    if( setsockopt( sock, IP_PROTOIP, IPO_MULTICAST_TTL, (void*)&ttl, sizeof(ttl)) < 0 ) {
		DEBUG( "failed to set multicast TTL: %d\r\n", tfGetSocketError(sock) );
		socket_close( sock );
		return -1;
	}

    socket_blocking_off( sock );
    return sock;
}

sys_thread_return mdns_main( sys_thread_data data )
{
	int active_fds;
	fd_set fds;
	int len = 0;
	struct timeval timeout;
	static char rx_buffer[MDNS_BUFFER_LEN];
	static char tx_buffer[MDNS_BUFFER_LEN];
	struct mdns_message rx_message, tx_message;
	struct sockaddr_in from;
	socklen_t in_size = sizeof(struct sockaddr_in);
	UINT32 ip = sys_get_ip();

	/* BEGIN CONFIGURATUION DATA */

	/* device settings */
    struct mdns_rr my_a, my_srv, my_txt, my_ptr;
    struct rr_a service_a;
    struct rr_srv service_srv;
    struct rr_ptr service_ptr;
    struct rr_txt service_txt;

	service_a.ip = ntohl(ip);

	service_srv.priority = 0;
    service_srv.weight = 0;
    service_srv.port = 80;
    service_srv.target = SERVICE_TARGET;

	service_ptr.name = SERVICE_NAME SERVICE_TYPE SERVICE_DOMAIN;

    service_txt.data = "\xF""path=index.html";

    my_a.data.a = &service_a;
    my_a.length = rr_length_a;
    my_a.transfer = rr_transfer_a;

    my_srv.data.srv = &service_srv;
    my_srv.length = rr_length_srv;
    my_srv.transfer = rr_transfer_srv;

    my_ptr.data.ptr = &service_ptr;
    my_ptr.length = rr_length_ptr;
    my_ptr.transfer = rr_transfer_ptr;

    my_txt.data.txt = &service_txt;
    my_txt.length = rr_length_txt;
    my_txt.transfer = rr_transfer_txt;

	/* END CONFIGURATION DATA */

    mc_sock = m_socket();
    if( mc_sock < 0 ) {
        DEBUG( "mDNS: unable to open multicast socket\r\n" );
        return;
    }

	DEBUG( "mDNS: responder is ready\n\r" );

    /* set up probe to claim name */
    mdns_transmit_init( &tx_message, tx_buffer );
    mdns_mark_question( &tx_message );
    mdns_add_question( &tx_message, SERVICE_NAME SERVICE_TYPE SERVICE_DOMAIN,
        T_ANY, C_IN );

	while( 1 ) {
		timeout.tv_sec = 0;
        timeout.tv_usec = 250000;
        FD_ZERO(&fds);
        FD_SET( mc_sock, &fds);

        active_fds = select( mc_sock+1, &fds, NULL, NULL, &timeout);
		
		if( active_fds < 0 ) {
			DEBUG( "mDNS: select() error\r\n" );
			return;
		}
	
		if( FD_ISSET( mc_sock, &fds ) ) {
			DEBUG( "mDNS: received message\r\n" );
            while( ( len = recvfrom( mc_sock, rx_buffer, MDNS_BUFFER_LEN, 0,
                     (struct sockaddr*)&from, (int *)&in_size ) ) > 0 ) {
                if( mdns_parse_message( &rx_message, rx_buffer ) &&
					mdns_status == STARTED &&
                    from.sin_addr.s_addr != ip &&
                    MDNS_IS_QUERY( rx_message ) ) {
					/* TODO: if status < STARTED, check message for probe
							 response, see if we have a conflict */
                    /* XXX just respond to anyone that isn't myself */
                    DEBUG( "mDNS: responding to query...\r\n" );
                    if( !mdns_send_message( &tx_message, mc_sock ) )
						DEBUG( "mDNS: error, failed to send message\r\n" );
                }
				else
					DEBUG( "mDNS: ignoring this message\r\n" );
            }
        }
		else if( mdns_status < STARTED ) {
            if( mdns_status == ANNOUNCE ) {
				DEBUG( "mDNS: announcing!\r\n" );
                mdns_transmit_init( &tx_message, tx_buffer );
                mdns_mark_response( &tx_message );
                /* A */
                mdns_add_answer( &tx_message, SERVICE_TARGET,
                    T_A, C_FLUSH, 225, &my_a );
                /* SRV */
                mdns_add_answer( &tx_message,
                    SERVICE_NAME SERVICE_TYPE SERVICE_DOMAIN,
                    T_SRV, C_FLUSH, 225, &my_srv );
                /* TXT */
                mdns_add_answer( &tx_message,
                    SERVICE_NAME SERVICE_TYPE SERVICE_DOMAIN,
                    T_TXT, C_FLUSH, 225, &my_txt );
                /* PTR */
                mdns_add_answer( &tx_message,
                    SERVICE_TYPE SERVICE_DOMAIN,
                    T_PTR, C_IN, 255, &my_ptr );
            }
            DEBUG( "mDNS: sending message...\r\n" ); 
            if( mdns_send_message( &tx_message, mc_sock ) )
            	mdns_status++;
			else
				DEBUG( "mDNS: error, failed to send message\r\n" );
		}
	}
}

/* initialize mdns responder state and launch mdns main thread */
sys_status mdns_responder_init( void )
{
	mdns_status = FIRST_PROBE;
	return sys_thread_create( &mdns_main_thread, mdns_main, 0, 
			(void *)&mdns_stack[0], sizeof(mdns_stack) );
}

sys_status mdns_responder_shutdown( void )
{
	socket_close(mc_sock);
	sys_thread_halt( &mdns_main_thread );
	sys_thread_delete( &mdns_main_thread );
	return SYS_SUCCESS;
}
