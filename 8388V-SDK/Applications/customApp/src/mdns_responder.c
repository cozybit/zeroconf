/*! \file mdns_responder.c
 *  \brief mDNS Responder implementation
 ******************************************************************************/

#include "system.h"
#include "mdns_responder.h"
#include "mdns.h"
#include "mdns_config.h" /* service name, etc */

/* Debugging and logging */
#ifdef MDNS_DEBUG
#include "log.h"
#define LOG(...) log(__VA_ARGS__) /**< built with logging suppoprt */
#else
#define LOG(...) {} /**< built without logging support */
#endif

static sys_thread mdns_main_thread;
static unsigned char mdns_stack[2048];
int mc_sock = -1; /**< multicast socket used for mDNS communication */

/** mDNS responder state */
enum mdns_status_t {
	INITIALIZED,    /**< initial state: no probes sent yet */
	FIRST_PROBE,	/**< wait a random amount of time (0-250ms) and probe */
	SECOND_PROBE,   /**< wait 250ms and probe */
	THIRD_PROBE,	/**< wait 250ms and probe */
	ANNOUNCE,	   	/**< send announcement message to claim name */
	STARTED			/**< we have claimed our name */
} mdns_status;

/** create and bind the multicast socket used for mDNS communication
 *
 * \retval int multicast socket */
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
		LOG( "failed to bind multicast socket: %d\r\n", 
			tfGetSocketError(sock) );
		sys_socket_close( sock );
		return -1;
	}

	/* join multicast group */
	mc.imr_multiaddr.s_addr = inet_addr("224.0.0.251"); /* FIXME use macro */
	mc.imr_interface.s_addr = sys_get_ip();
	if( setsockopt( sock, IP_PROTOIP, IPO_ADD_MEMBERSHIP, (void *)&mc, 
		sizeof(mc) ) < 0 ) {
		LOG( "failed to add multicast membership: %d\r\n", 
			tfGetSocketError( sock ) );
		sys_socket_close( sock );
		return -1;
	}

	if( setsockopt( sock, IP_PROTOIP, IPO_MULTICAST_TTL, (void*)&ttl, sizeof(ttl)) < 0 ) {
		LOG( "failed to set multicast TTL: %d\r\n", tfGetSocketError(sock) );
		sys_socket_close( sock );
		return -1;
	}

	sys_socket_blocking_off( sock );
	return sock;
}

/** send an mDNS message to announce our service
 *
 *  This function adds all resource records to an mDNS message which, when
 *  sent, will announce our device on the network.
 *
 *  TODO: this function should be rewritten to use the 'service' representation
 *  of resource records from the developer-friendly API.  This will simplify
 *  the interface and make it more general as well.
 *
 *  \param[in] m The mDNS message to which the records will be added.
 *  \param[in] buffer the transmit buffer where the records will be copied
 *  \param[in] sock multicast socket on which to send the message. */
void mdns_announce( struct mdns_message *m, char *buffer, int sock,
					struct mdns_rr *a, struct mdns_rr *srv, 
					struct mdns_rr *txt, struct mdns_rr *ptr )
{
	LOG( "mDNS: announcing...\r\n" );
	mdns_transmit_init( m, buffer );
	mdns_mark_response( m );
	/* A */
	mdns_add_answer( m, SERVICE_TARGET, T_A, C_FLUSH, 225, a );
	/* SRV */
	mdns_add_answer( m, SERVICE_NAME SERVICE_TYPE SERVICE_DOMAIN,
		T_SRV, C_FLUSH, 225, srv );
	/* TXT */
	mdns_add_answer( m, SERVICE_NAME SERVICE_TYPE SERVICE_DOMAIN,
		T_TXT, C_FLUSH, 225, txt );
	/* PTR */
	mdns_add_answer( m, SERVICE_TYPE SERVICE_DOMAIN, 
		T_PTR, C_IN, 255, ptr );
	if( !mdns_send_message( m, sock ) )
		LOG( "mDNS: error, failed to send message\r\n" );
}

/** carry out the mDNS start sequence and then act as a responder
 *
 *  This is the main thread.  It first claims our name and then announces our
 *  service on the network.  After that it acts as an mDNS responder, answering
 *  questions that are relevant to our name and service. 
 *
 *  TODO: modify to work with the new API, namely 'names' and 'services' and
 *  remove all of the hard-coded junk and RR representations that are here for
 *  testing.
 *
 *  TODO: check incomming responses during start sequence, see if they are a
 *  name collision, deal with them.
 *
 *  TODO: respond to messages based on their questions (when relevant), as part
 *  of implementing the spec correctly and moving to the final API */
sys_thread_return mdns_main( sys_thread_data data )
{
	int active_fds;
	fd_set fds;
	int len = 0;
	struct timeval timeout;
	UINT32 start_time, t;
	static char rx_buffer[MDNS_BUFFER_LEN];
	static char tx_buffer[MDNS_BUFFER_LEN];
	struct mdns_message rx_message, tx_message;
	struct sockaddr_in from;
	socklen_t in_size = sizeof(struct sockaddr_in);
	UINT32 ip = sys_get_ip(); /* XXX will not be needed with the final API*/

	/* XXX BEGIN CONFIGURATUION DATA */

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

	/* XXX END CONFIGURATION DATA */

	mc_sock = m_socket();
	if( mc_sock < 0 ) {
		DEBUG( "mDNS: unable to open multicast socket\r\n" );
		return;
	}

	LOG( "mDNS: responder is ready\n\r" );

	/* set up probe to claim name */
	mdns_transmit_init( &tx_message, tx_buffer );
	mdns_mark_question( &tx_message );
	mdns_add_question( &tx_message, SERVICE_NAME SERVICE_TYPE SERVICE_DOMAIN,
		T_ANY, C_IN );

	start_time = get_time();
	timeout.tv_sec = 0;
	timeout.tv_usec = sys_random( 0, 250 )*1000;
	while( 1 ) {
		FD_ZERO(&fds);
		FD_SET( mc_sock, &fds);
		active_fds = select( mc_sock+1, &fds, NULL, NULL, &timeout);
		
		if( active_fds < 0 ) {
			DEBUG( "mDNS: select() error\r\n" );
			return;
		}
	
		if( FD_ISSET( mc_sock, &fds ) ) {
			LOG( "mDNS: received message\r\n" );
			while( ( len = recvfrom( mc_sock, rx_buffer, MDNS_BUFFER_LEN, 0,
					 (struct sockaddr*)&from, (int *)&in_size ) ) > 0 ) {
				if( mdns_parse_message( &rx_message, rx_buffer ) &&
					from.sin_addr.s_addr != ip &&
					MDNS_IS_QUERY( rx_message ) ) {
				
					if( mdns_status < STARTED && 
						MDNS_IS_RESPONSE(rx_message) ) {
						/* TODO: check if conflict */
						LOG( "mDNS: investigating possible conflict...\r\n" );
						/* no conflict */
					}
					else if( mdns_status == STARTED && 
							 MDNS_IS_QUERY( rx_message ) ) {
						/* XXX: just using announce-generated message now 
						   XXX: just respond to any querry for now */
						LOG( "mDNS: responding to query...\r\n" );
						if( !mdns_send_message( &tx_message, mc_sock ) )
							DEBUG( "mDNS: error, failed to send message\r\n" );
					}
				}
				else
					LOG( "mDNS: ignoring this message\r\n" );
			}
		}

		if( mdns_status < STARTED ) {
			if( FD_ISSET( mc_sock, &fds ) ) { /* wait remaining time */
				t = get_time();
				t = t > start_time ? t - start_time : start_time - t;
				timeout.tv_usec = t <= 250000 ? (250-t)*1000 : 250000;
			}
			else { /* send message, advance to next state */
				mdns_status++;
				if( mdns_status == ANNOUNCE )
					mdns_announce( &tx_message, tx_buffer, mc_sock,
								&my_a, &my_srv, &my_txt, &my_ptr );
				else /* probe */
					mdns_send_message( &tx_message, mc_sock );
				timeout.tv_usec = 250000;
			}
			start_time = get_time();
		}
		else { /* set default timeout */
			timeout.tv_usec = 250000;
			start_time = get_time();
		}
	}
}

/** initialize mdns responder state and launch mdns main thread */
sys_status mdns_responder_init( void )
{
	mdns_status = INITIALIZED;
	return sys_thread_create( &mdns_main_thread, mdns_main, 0, 
			(void *)&mdns_stack[0], sizeof(mdns_stack) );
}

/** stop mdns responder thread */
sys_status mdns_responder_shutdown( void )
{
	if( mc_sock >= 0 )
		sys_socket_close(mc_sock);
	sys_thread_halt( &mdns_main_thread );
	sys_thread_delete( &mdns_main_thread );
	return SYS_SUCCESS;
}
