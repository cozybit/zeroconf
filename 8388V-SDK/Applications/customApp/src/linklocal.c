/******************************************************************************
 * linklocal.c
 *
 * 
 ******************************************************************************/

#include "system.h"
#include "linklocal.h"

/******************************************************************************
 * Link Local Variables and Macros
 ******************************************************************************/

#define ARP_TYPE 0x0608

/* Constants called out by RFC 3927.  All time values are in seconds. */
#define PROBE_WAIT           1 /* initial random delay */
#define PROBE_NUM            3 /* number of probe packets */
#define PROBE_MIN            1 /* minimum delay till repeated probe */
#define PROBE_MAX            2 /* maximum delay till repeated probe */
#define ANNOUNCE_WAIT        2 /* delay before announcing */
#define ANNOUNCE_NUM         2 /* number of announcement packets */
#define ANNOUNCE_INTERVAL    2 /* time between announcement packets */
#define MAX_CONFLICTS       10 /* max conflicts before rate limiting */
#define RATE_LIMIT_INTERVAL 60 /* delay between successive attempts */
#define DEFEND_INTERVAL     10 /* minimum interval between defensive ARPs */

enum {
	LL_NO_ADDRESS = 0,
	LL_PROBE,
	LL_ANNOUNCE,
	LL_ADDR,
	LL_RATE_LIMITED_PROBE,
	LL_RATE_LIMITED_ANNOUNCE,
};

#define LL_ARP_QUEUE_SIZE 10
static sys_queue_item ll_arp_queue_mem[LL_ARP_QUEUE_SIZE];
static sys_queue ll_arp_queue; /* Queue of arp packets to process */

static unsigned int ll_state = LL_NO_ADDRESS;
static unsigned int ll_num_conflicts = 0;
static unsigned int ll_candidate_ip = 0;

/* Task information for the User Interface task*/
static sys_thread ll_main_thread;
static unsigned char ll_stack[2048];

/******************************************************************************
 * Private Functions
 ******************************************************************************/

/* Generate random IP address between 169.254.1.0 and 169.254.254.255 */
/* ONLY WORKS ON LITTLE ENDIAN MACHINE! */
unsigned int ll_random_ip()
{
	unsigned int ip_addr;
	ip_addr = sys_random(0x0100, 0xfeff) << 16;
	ip_addr = ip_addr | (254<<8) | 169;
	return ip_addr;
}

/* This is a handler that monitors the ARP traffic.  It should have an
 * opportunity to inspect the ARP traffic before the ARP system gets to see it.
 */
sys_pkt_status ll_handle_arp(sys_pkt *pkt)
{

	unsigned char *link_header = (unsigned char *)pkt + L2_HEADER_OFFSET;
	unsigned short type = *(unsigned short *)(link_header + L2_TYPE_OFFSET);
	
	if (type == ARP_TYPE) {
		/* This is an ARP packet. */

		/* Are we probing? */
		/* Is the sender_ip == ll_candidate_ip? */
		/* Is the target_ip == ll_candidate_ip && sender_mac != my_mac */
		/* ll_num_conflicts++; */
		DEBUG("ARP PACKET!\r\n");
		
	}

	return SYS_PKT_PASS;
}

/* This is the main thread routine.  It is a state machine that implements RFC
 * 3927 for Link Local Addressing. 
 */
sys_thread_return ll_main(sys_thread_data data)
{
	sys_time timeout = 0;
	sys_status status = SYS_SUCCESS;
	unsigned int probe = 0, announce = 0;

	while(1) {

		status = sys_thread_sleep(timeout, &ll_main_thread);
		if( (status != SYS_SUCCESS) && (status != SYS_THREAD_AWOKEN) ) {
			/* This should never happen! */
			return;
		}

		switch (ll_state) {
			
		case LL_NO_ADDRESS:
			/* Wait for random interval, then probe candidate ip */
			ll_candidate_ip = ll_random_ip();
			timeout = sys_random(0, PROBE_WAIT*1000);
			probe = 0;
			announce = 0;
			ll_state = LL_PROBE;
			break;

		case LL_PROBE:
			if(ll_num_conflicts >= MAX_CONFLICTS) {
				/* Somebody is being hostile or stupid.  Back off. */
				ll_state = LL_RATE_LIMITED_PROBE;
				ll_candidate_ip = 0;
				timeout = RATE_LIMIT_INTERVAL;
				break;
			}

			if(status == SYS_SUCCESS) {
				/* We're probing, and our address hasn't been claimed */
				if(probe++ < PROBE_NUM) {
					/* send probe then wait */
					timeout = sys_random(PROBE_MIN*1000, PROBE_MAX*1000);
				} else {
					/* Yay!  We can try claiming this address. */
					timeout = ANNOUNCE_WAIT;
					ll_state = LL_ANNOUNCE;
				}
			} else if(status == SYS_THREAD_AWOKEN) {
				/* The ARP handler awoke us, so we must choose another addr */
				ll_state = LL_NO_ADDRESS;
				timeout = 0;
			}
			break;

		case LL_ANNOUNCE:
			if(ll_num_conflicts >= MAX_CONFLICTS) {
				/* Somebody is being hostile or stupid.  Back off. */
				ll_state = LL_RATE_LIMITED_PROBE;
				ll_candidate_ip = 0;
				timeout = RATE_LIMIT_INTERVAL;
				break;
			}

			if(status == SYS_SUCCESS) {
				/* We're announcing, and there's no conflict yet */
				if(announce++ < ANNOUNCE_NUM) {
					/* send announcement then wait */
					timeout = ANNOUNCE_INTERVAL;
				} else {
					/* We have claimed this address. */
					timeout = ANNOUNCE_WAIT;
					ll_state = LL_ADDR;
				}
			} else if(status == SYS_THREAD_AWOKEN) {
				/* The ARP handler awoke us, so we must choose another addr */
				ll_state = LL_NO_ADDRESS;
				timeout = 0;
			}
			break;

		case LL_RATE_LIMITED_PROBE:
			/* Somebody is being hostile so we've slowed down our attempts */
			/* We've waited RATE_LIMIT_INTERVAL.  Try another address. */
			ll_candidate_ip = ll_random_ip();
			/* send probe */
			timeout = ANNOUNCE_WAIT;
			ll_state = LL_RATE_LIMITED_ANNOUNCE;
			break;

		case LL_RATE_LIMITED_ANNOUNCE:
			if(status == SYS_SUCCESS) {
				/* Our address is available. */
				timeout = 0;
				ll_state = LL_ADDR;
			} else if(status == SYS_THREAD_AWOKEN) {
				/* The ARP handler saw a conflict, so choose another addr */
				ll_state = LL_RATE_LIMITED_PROBE;
				ll_candidate_ip = 0;
				timeout = RATE_LIMIT_INTERVAL;
			}
			break;

		case LL_ADDR:
			if(status == SYS_SUCCESS) {
				/* Finally!  We have an address.  Initialize the TCP/IP stack
				 * and go to sleep forever. */
				ll_num_conflicts = 0;
				timeout = SYS_FOREVER;

				/* If we ever wake up, it's because somebody stole our address
				 * and we need to get another. */
				ll_state = LL_NO_ADDRESS;
				
			} else if(status == SYS_THREAD_AWOKEN) {
				/* Rats!  Somebody stole our address!  Get a new one. */
				ll_state = LL_NO_ADDRESS;
				timeout = 0;
			}
			break;
		}
	}
}

/******************************************************************************
 * Public Functions
 ******************************************************************************/

/* This function launches link-local address thread.  If link-local addressing
 * is to be used, link-local manager should be called prior to treck stack
 * initialization.  Otherwise, the Treck tcpip handler will consume ARP packets
 * before the link-local manager ever sees them.  Applications should poll the
 * state variable treck_ready to know if the treck stack is up.
 *
 * Return values: SYS_SUCCESS for success, other for failure.
 */
sys_status ll_init(void)
{
	sys_status ret;

	ll_state = LL_NO_ADDRESS;
	ll_num_conflicts = 0;
	ll_candidate_ip = 0;

	/* Install the ARP packet inspector */
	ret = sys_add_rx_pkt_handler(ll_handle_arp);
	if(ret != SYS_SUCCESS)
		return ret;

	ret = sys_queue_create(&ll_arp_queue, ll_arp_queue_mem, LL_ARP_QUEUE_SIZE);
	if(ret != SYS_SUCCESS)
		goto done1;

	/* Launch the link-local main thread. */
	ret = sys_thread_create(&ll_main_thread, ll_main, 0, (void *)&ll_stack[0],
							sizeof(ll_stack));
	if(ret != SYS_SUCCESS)
		goto done2;

	goto done0;

 done2:
	sys_queue_delete(&ll_arp_queue);
 done1:
	sys_remove_rx_pkt_handler(ll_handle_arp);
 done0:
	return ret;
}

/*
 * Return values: SYS_SUCCESS for success, other for failure.
 */
sys_status ll_shutdown(void)
{
	sys_thread_halt(&ll_main_thread);
	sys_thread_delete(&ll_main_thread);
	sys_queue_delete(&ll_arp_queue);
	sys_remove_rx_pkt_handler(ll_handle_arp);
	return SYS_SUCCESS;
}
