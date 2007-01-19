#include "os_if.h"
#include "sysinfo.h"
#include "tx_api.h"

#include <trsocket.h>
#include "tcpip_socki.h"

#include "dbg_Facility.h"

extern int tcp_ready;

static char     	MDNS_Stack[4096];
static TX_THREAD	MDNS_Thread;

static void MDNSTask( os_Addrword_t );

/* create multicast socket on 224.0.0.251:5353 */
int m_socket( void )
{
	int sock;
	int one = 1;
	int i_ttl = 255;
	char c_ttl = 255;
	struct sockaddr_in in_addr;
	struct ip_mreq mc;

	memset( &in_addr, 0x00, sizeof(in_addr) );
	in_addr.sin_family = AF_INET;
    in_addr.sin_port = htons(5353);
    in_addr.sin_addr.s_addr = 0;

	sock = socket( AF_INET, SOCK_DGRAM, 0 );
	if( sock < 0 )
		return sock;

	setsockopt( sock, SOL_SOCKET, SO_REUSEPORT, (char*)&one, sizeof(one) );
	setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(one) );

	if( bind( sock, (struct sockaddr*)&in_addr, sizeof(in_addr) ) ) { 
		tfClose( sock ); 
		return 0; 
	}

	/* set up multicast address */
	mc.imr_multiaddr.s_addr = inet_addr("224.0.0.251");
	mc.imr_interface.s_addr = htonl(INADDR_ANY);
	setsockopt( sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mc, sizeof(mc) );
    setsockopt( sock, IPPROTO_IP, IP_MULTICAST_TTL, &c_ttl, sizeof(c_ttl) );
    setsockopt( sock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&i_ttl, 
				sizeof(i_ttl) );

	tfBlockingState( sock, TM_BLOCKING_OFF); /* set to non-blocking */

	return sock;
}

/* create mDNS responder thread */
Status_e mdnsInit( void )
{
	memset( MDNS_Stack, 0xFF, sizeof(MDNS_Stack) ); /* initialize stack */
	if( tx_thread_create( &MDNS_Thread, 
						"MDNSTask", 
						MDNSTask,
						(UINT32)NULL,
						(VOID *)&MDNS_Stack[0],
						sizeof(MDNS_Stack),
						20,
                		20,
                		2,
                		TX_AUTO_START) != TX_SUCCESS) {
		DBG_P( ( DBG_L0 "error: failed to create mDNS thread\r\n" ) );
		return FAIL;
	}
	DBG_P( ( DBG_L0 "mDNS thread initialized\r\n" ) );
	return SUCCESS;
}

/* mDNS responder main task */
void MDNSTask( os_Addrword_t w )
{
	int mc_sock;
	int active_fds;
	fd_set fds;
	struct timeval timeout;

	DBG_P( ( DBG_L0 "mDNS task started\r\n" ) );

	/* wait until TCP setup is complete TODO: actually wait on IPV4LL */
	while( !tcp_ready ) os_TaskDelay( 1 );

	/* open multicast listening socket */
	mc_sock = m_socket();
	if( mc_sock < 0 ) {
		DBG_P( ( DBG_L0 "error: failed to create multicast socket\r\n" ) );
		return;
	}
	
	DBG_P( ( DBG_L0 "mDNS multicast socket created\r\n" ) );

	timeout.tv_sec = 1;
    timeout.tv_usec = 0;

	while( 1 ) {
        FD_ZERO(&fds);
        FD_SET( mc_sock, &fds);
        active_fds = select( mc_sock+1, &fds, NULL, NULL, &timeout);

		if( FD_ISSET( mc_sock, &fds ) ) {
			DBG_P( ( DBG_L0 "<----\r\n" ) );
		}

		os_TaskDelay( 1 );
	}
}
