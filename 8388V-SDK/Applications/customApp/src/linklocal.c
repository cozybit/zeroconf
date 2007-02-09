/*! \file linklocal.c
 *
 *  \brief Link-Local implimentation
 ******************************************************************************/

#include "system.h"
#include "linklocal.h"

/******************************************************************************
 * Link Local Variables and Macros
 ******************************************************************************/

/* Constants called out by RFC 3927.  All time values are in seconds. */
#define PROBE_WAIT           1 /**< initial random delay */
#define PROBE_NUM            3 /**< number of probe packets */
#define PROBE_MIN            1 /**< minimum delay till repeated probe */
#define PROBE_MAX            2 /**< maximum delay till repeated probe */
#define ANNOUNCE_WAIT        2 /**< delay before announcing */
#define ANNOUNCE_NUM         2 /**< number of announcement packets */
#define ANNOUNCE_INTERVAL    2 /**< time between announcement packets */
#define MAX_CONFLICTS       10 /**< max conflicts before rate limiting */
#define RATE_LIMIT_INTERVAL 60 /**< delay between successive attempts */
#define DEFEND_INTERVAL     10 /**< minimum interval between defensive ARPs */

enum {
	LL_NO_ADDRESS = 0,
	LL_PROBE,
	LL_ANNOUNCE,
	LL_RATE_LIMITED_PROBE,
	LL_STABLE,
};

/**< time to wait between attempts to init tcp stack. */
#define LL_SYS_INIT_RETRY 1000

/**< WARNING!  Works only with little endian! */
#define LL_NETMASK 0x0000ffff

static unsigned int ll_state = LL_NO_ADDRESS;
static unsigned int ll_candidate_ip = 0;
static char ll_mac[6];

/**< Task information for the User Interface task*/
static sys_thread ll_main_thread;
static sys_eflags ll_eflags;
#define LL_CONFLICT_EVENT 0x1
static int ll_running = 0;
static unsigned char ll_stack[2048];

/**< ARP packet definitions.  Consider moving to its own file */

#define ARP_TYPE 0x0806

PACKED_STRUCT_BEGIN struct arp_pkt {
	unsigned short hard_type;
	unsigned short prot_type;
	unsigned char hard_size;
	unsigned char prot_size;
	unsigned short op;
	unsigned char sender_mac[6];
	unsigned char sender_ip[4]; /**< network byte order */
	unsigned char target_mac[6];
	unsigned char target_ip[4]; /**< network byte order */
} PACKET_STRUCT_END;

#define ARP_OP_REQUEST 1
#define ARP_OP_REPLY 2
#define RARP_OP_REQUEST 3
#define RARP_OP_REPLY 4

/* Debugging and logging */
#define LL_DEBUG
#ifdef LL_DEBUG
#include "log.h"
#define LOG(...) log(__VA_ARGS__)
#else
#define LOG(...) {}
#endif

/******************************************************************************
 * Private Functions
 ******************************************************************************/

/* Generate random IP address between 169.254.1.0 and 169.254.254.255 */
/* ONLY WORKS ON LITTLE ENDIAN MACHINE! */
unsigned int ll_random_ip()
{
	unsigned int ip_addr = 0;
	ip_addr = sys_random(0, 255) << 24;
	ip_addr = ip_addr | sys_random(1, 254) << 16;
	ip_addr = ip_addr | (254<<8) | 169;
	return ip_addr;
}

/* Determine if an arp packet conflicts with the supplied mac and ip address.
 * The ip address should be in network byte order. */
int ll_arp_conflict(struct arp_pkt *arp, char mac[6], unsigned int ip)
{
	unsigned int sender_ip, target_ip;

	if( (arp->op != htons(ARP_OP_REPLY)) &&
		(arp->op != htons(ARP_OP_REQUEST)) ) {
		return 0;
	}
	
	memcpy(&sender_ip, (void *)arp->sender_ip, 4);
	if(sender_ip == ip)
		/* somebody owns this address */
		return 1;

	memcpy(&target_ip, (void *)arp->target_ip, 4);
	if( (sender_ip == 0) && (target_ip == ip) )
		/* somebody is probing this address */
		return 1;

	return 0;
}

/* Send an arp probe with the specified mac and ip */
sys_status ll_send_probe(char mac[6], unsigned int ip)
{
	struct arp_pkt arp;
	char bcast[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	/* Construct arp packet. */
	arp.hard_type = htons(0x0001); /* ethernet is type 1 */
	arp.prot_type = htons(0x0800); /* IP addr is type 0x0800 */
	arp.hard_size = 6;
	arp.prot_size = 4;
	arp.op = htons(ARP_OP_REQUEST);
	memcpy((void *)arp.sender_mac, mac, 6);
	memset((void *)arp.sender_ip, 0, 4);
	memset((void *)arp.target_mac, 0, 6);
	memcpy((void *)arp.target_ip, &ip, 4);

	return sys_link_sendto(bcast, ARP_TYPE, (char *)&arp,
						   sizeof(struct arp_pkt));
}

/* Send an arp announcement with the specified mac and ip */
sys_status ll_send_announce(char mac[6], unsigned int ip)
{
	struct arp_pkt arp;
	char bcast[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	/* Construct arp packet. */
	arp.hard_type = htons(0x0001); /* ethernet is type 1 */
	arp.prot_type = htons(0x0800); /* IP addr is type 0x0800 */
	arp.hard_size = 6;
	arp.prot_size = 4;
	arp.op = htons(ARP_OP_REQUEST);
	memcpy((void *)arp.sender_mac, mac, 6);
	memcpy((void *)arp.sender_ip, &ip, 4);
	memset((void *)arp.target_mac, 0, 6);
	memcpy((void *)arp.target_ip, &ip, 4);

	return sys_link_sendto(bcast, ARP_TYPE, (char *)&arp,
						   sizeof(struct arp_pkt));
}

/* This is a handler that monitors the ARP traffic.  It should have an
 * opportunity to inspect the ARP traffic before the ARP system gets to see it.
 */
sys_pkt_status ll_handle_arp(sys_pkt *pkt)
{

	unsigned char *link_header = (unsigned char *)pkt + L2_HEADER_OFFSET;
	unsigned short type = *(unsigned short *)(link_header + L2_TYPE_OFFSET);
	struct arp_pkt *arp;

	if(ll_running == 0)
		return SYS_PKT_PASS;

	arp = (struct arp_pkt *)(link_header + ARP_OFFSET);

	if (type == htons(ARP_TYPE)) {
		/* This is an ARP packet.  Check if it conflicts. */
		
		if(ll_arp_conflict(arp, ll_mac, ll_candidate_ip)) {
			sys_eflags_set(&ll_eflags, LL_CONFLICT_EVENT);
		}
	}

	return SYS_PKT_PASS;
}

/* This is the main thread routine.  It implements RFC 3927 for Link Local
 * Addressing.
 */
sys_thread_return ll_main(sys_thread_data data)
{
	sys_time timeout = 0;
	sys_status status = SYS_SUCCESS;
	unsigned int probe = 0, announce = 0;
	unsigned int conflict, num_conflicts;
	unsigned int flags;

	while(1) {

		flags = 0;
		status = sys_eflags_wait(&ll_eflags, LL_CONFLICT_EVENT, &flags,
								 timeout);

		if( (status != SYS_SUCCESS) && (status != SYS_NO_EVENTS) ) {
			/* This should never happen! */
			return;
		}

		if( flags & LL_CONFLICT_EVENT ) {
			conflict = 1;
			num_conflicts++;
		} else {
			conflict = 0;
		}

		switch (ll_state) {

		case LL_NO_ADDRESS:
			/* Wait for random interval, then probe candidate ip */
			ll_candidate_ip = ll_random_ip();
			timeout = sys_random(0, PROBE_WAIT*1000);
			LOG("LL %d: Sending first probe in %d ms.\r\n", sys_time_get(),
				timeout);
			probe = 0;
			ll_state = LL_PROBE;
			break;

		case LL_PROBE:

			if( (conflict) && (num_conflicts >= MAX_CONFLICTS) ) {
				/* Too many conflicts.  Time to go rate-limited */
				LOG("LL %d: Many conflicts.  Going into rate-limited mode.\r\n",
					sys_time_get());
				ll_state = LL_RATE_LIMITED_PROBE;
				ll_candidate_ip = 0;
				timeout = RATE_LIMIT_INTERVAL*1000;

			} else if(conflict) {
				/* Rats.  Somebody has our address. Try again. */
				LOG("LL %d: Somebody has our address.  Trying again.\r\n",
					sys_time_get());
				ll_state = LL_NO_ADDRESS;
				timeout = 0;

			} else if(probe == PROBE_NUM - 1) {
				/* send final probe then wait ANNOUNCE_WAIT */
				LOG("LL %d: Sending final probe.\r\n", sys_time_get());
				ll_send_probe(ll_mac, ll_candidate_ip);
				ll_state = LL_ANNOUNCE;
				timeout = ANNOUNCE_WAIT*1000;
				LOG("LL %d: Sending first announcement in %d ms.\r\n",
					sys_time_get(), timeout);
				announce = 0;

			} else if(probe < (PROBE_NUM - 1)) {
				/* our address is still good.  Send next probe then wait. */
				LOG("LL %d: Sending probe.\r\n", sys_time_get());
				ll_send_probe(ll_mac, ll_candidate_ip);
				probe++;
				timeout = sys_random(PROBE_MIN*1000, PROBE_MAX*1000);
				LOG("LL %d: Next probe in %d ms.\r\n", sys_time_get(), timeout);
			}
			break;

		case LL_ANNOUNCE:
			if( (conflict) && (num_conflicts >= MAX_CONFLICTS) ) {
				/* Too many conflicts.  Time to go rate-limited */
				ll_state = LL_RATE_LIMITED_PROBE;
				ll_candidate_ip = 0;
				timeout = RATE_LIMIT_INTERVAL*1000;

			} else if(conflict) {
				/* Rats.  Somebody has our address. Try again. */
				LOG("LL %d: Received claim after announce.  Trying again.\r\n",
					sys_time_get());
				ll_state = LL_NO_ADDRESS;
				timeout = 0;

			} else if(announce == ANNOUNCE_NUM - 1) {
				/* send final announcement then wait ANNOUNCE_INTERVAL */
				LOG("LL %d: Sending final announcement.\r\n", sys_time_get());
				ll_send_announce(ll_mac, ll_candidate_ip);
				ll_state = LL_STABLE;
				timeout = ANNOUNCE_INTERVAL*1000;

			} else if(announce < (ANNOUNCE_NUM - 1)) {
				/* our address is still good.  Send next announce then wait. */
				LOG("LL %d: Sending announcement.\r\n", sys_time_get());
				ll_send_announce(ll_mac, ll_candidate_ip);
				announce++;
				timeout = ANNOUNCE_INTERVAL*1000;
				LOG("LL %d: Sending next announcement in %d ms.\r\n",
					sys_time_get(),	timeout);
			}
			break;

		case LL_RATE_LIMITED_PROBE:
			/* We're getting lots of conflicts so we've slowed down */
			if(conflict) {
				ll_candidate_ip = 0;
				timeout = RATE_LIMIT_INTERVAL*1000;

			} else {
				/* We've waited RATE_LIMIT_INTERVAL.  Try another address. */
				ll_candidate_ip = ll_random_ip();
				timeout = 0;
				ll_state = LL_PROBE;
			}
			break;

		case LL_STABLE:
			if(conflict) {
				/* Rats.  Somebody jacked our address.  Try again. */
				sys_tcpip_halt();
				ll_state = LL_NO_ADDRESS;
				timeout = 0;

			} else {
				/* Finally!  We have an address.  Initialize the TCP/IP stack
				 * and go to sleep forever. */
				num_conflicts = 0;
				timeout = SYS_FOREVER;
				LOG("LL %d: Got address %x.  Setting up tcpip.\r\n",
					sys_time_get(), ll_candidate_ip);
				if(sys_tcpip_init(ll_candidate_ip, LL_NETMASK) != SYS_SUCCESS) {
					/* Ultimately this case should be left up to the system, not
					 * us.  But for now, we'll just sleep and try
					 * again. later. */
					timeout = LL_SYS_INIT_RETRY;
				}
			}
			break;
		}
	}
}

/******************************************************************************
 * Public Functions
 ******************************************************************************/

/* launches link-local address thread. */ 
sys_status ll_init(void)
{
	sys_status ret;

	LOG("LL %d: Initializing\r\n", sys_time_get());
	if(ll_running != 0)
		return SYS_FAIL;

	/* Shut down the tcpip stack, just in case somebody else turned it on. */
	sys_tcpip_halt();

	ll_state = LL_NO_ADDRESS;

	ret = sys_link_get_mac(ll_mac);
	if(ret != SYS_SUCCESS)
		return ret;

	/* Install the ARP packet inspector */
	ret = sys_add_rx_pkt_handler(ll_handle_arp);
	if(ret != SYS_SUCCESS)
		return ret;

	/* Create eflags for conflict events. */
	ret = sys_eflags_create(&ll_eflags);
	if(ret != SYS_SUCCESS)
		goto done1;	

	/* Launch the link-local main thread. */
	ret = sys_thread_create(&ll_main_thread, ll_main, 0, (void *)&ll_stack[0],
							sizeof(ll_stack));
	if(ret != SYS_SUCCESS)
		goto done2;

	goto done0;

 done2:
	sys_eflags_delete(&ll_eflags);
 done1:
	sys_remove_rx_pkt_handler(ll_handle_arp);
 done0:
	ll_running = 1;
	return ret;
}

/* shuts down link-local thread */
sys_status ll_shutdown(void)
{
	LOG("LL %d: Shutting down.\r\n", sys_time_get());
	sys_thread_halt(&ll_main_thread);
	sys_thread_delete(&ll_main_thread);
	sys_eflags_delete(&ll_eflags);
	sys_remove_rx_pkt_handler(ll_handle_arp);
	sys_tcpip_halt();
	ll_running = 0;
	return SYS_SUCCESS;
}
