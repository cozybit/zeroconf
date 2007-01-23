/******************************************************************************
 * linklocal.c
 *
 * 
 ******************************************************************************/

#include "linklocal.h"

/******************************************************************************
 * System Interface
 ******************************************************************************/

/* To port linklocal to other platforms, these functions and macros should be
 * re-implemented.  Also, they should probably be moved to another file. */

#define THREADX
#ifdef THREADX

#include "mli.h"
#include "dbg_Facility.h"
#include "trsocket.h"
#include "trmacro.h"
#include "trtype.h"
#include "trproto.h"
/*
 * Return Values
 */
#define LL_STATUS int /* General return type */
#define LL_SUCCESS TX_SUCCESS
#define LL_THREAD_CREATED TX_SUCCESS
#define LL_THREAD_HALTED TX_SUCCESS
#define LL_THREAD_DELETED TX_SUCCESS
#define LL_SLEEP_END TX_SUCCESS /* Thread awoke from complete sleep */
#define LL_AWOKEN TX_WAIT_ABORTED /* Thread was awoken by another process */

/*
 * Packet Handler Interface
 */
#define LL_PACKET RxPD_t /* Packet data type */
/* pointer to top of the link-layer header */
#define LL_LINK_HEADER(pkt) (((unsigned char *)(pkt)) + sizeof(RxPD_t))
#define LL_PACKET_STATUS MLI_CUSTOM_DATA_T /* Return type for packet handler */
#define LL_PACKET_HANDLER MLI_CUSTOM_DATA_RX_HANDLER_TYPE /* Packet handler
														   * function type */
#define LL_PACKET_PASS MLI_CUSTOM_DATA_NOT_MY_PKT /* Return for packets that we
												   * didn't consume */
#define LL_PACKET_CONSUME MLI_CUSTOM_DATA_PKT_NULL /* Return for packets that we
													* consumed. */
/* Function to add a packet receive handler to the packet processing chain.
 */
LL_STATUS ll_install_rx_handler(LL_PACKET_HANDLER handler)
{
	
	if(mli_installCustomRxDataHandler(handler) == 0)
		return LL_SUCCESS;
	return 1;
}

/*
 * Time Interface
 */

/* ThreadX ticks are 10ms long. */
#define TICKS_PER_SEC 100
#define MSEC_PER_TICK 10
#define LL_FOREVER 0xffffffff

/*
 * Thread Interface
 */
#define LL_THREAD TX_THREAD /* Thread data type */

/* Function to create a thread */
LL_STATUS ll_thread_create(LL_THREAD *t, void (*entry)(unsigned long),
						   void *stack, unsigned int stack_size)
{
	return tx_thread_create(t,"Link Local Manager", entry, 0, stack, stack_size,
							9, 9, 29, TX_AUTO_START);
}

/* Function to halt a thread */
LL_STATUS ll_thread_terminate(LL_THREAD *t)
{
	return tx_thread_terminate(t);
}

/* Function to delete a thread */
LL_STATUS ll_thread_delete(LL_THREAD *t)
{
	return tx_thread_delete(t);
}

/* Function to sleep a thread.  Must return LL_SLEEP_END after msec milliseconds
 * or LL_AWOKEN if awoken by another thread prior to msec milliseconds. */
LL_STATUS ll_thread_sleep(unsigned int msec, LL_THREAD *t)
{
	unsigned int ticks = msec / MSEC_PER_TICK;

	if(msec == LL_FOREVER)
		return tx_thread_suspend(t);

	return tx_thread_sleep(ticks);
}

/*
 * Random Number Generator Interface
 */
/* Generate a random number between floor and ceiling */
unsigned short ll_random(unsigned short floor, unsigned short ceiling)
{
	unsigned int rand = tfGetRandom();

	/* round to 16 bits so we have room to do some math. */
	rand += 1<<15;
	rand >>= 16;
	
	/* map the range 0-2^16-1 to floor-ceiling */
	rand = (ceiling - floor) * rand + floor;
	rand /= 65535;
	rand &= 0xffff;

	return (unsigned short)rand;
}

/*
 * Debug Print Interface
 */
#define DEBUG(...) DBG_P(( DBG_L0 __VA_ARGS__));

#endif

/******************************************************************************
 * Link Local Variables and Macros
 ******************************************************************************/

#define ARP_TYPE 0x0608
#define PACKET_TYPE_OFFSET 20
#define PACKET_DATA_OFFSET 22

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

unsigned int ll_state = LL_NO_ADDRESS;
/* ll_candidate_ip and ll_num_conflicts are accessed in two different contexts.
 * They are not currently protected in any way.  Can we protect it without
 * blocking in ll_arp_handler?  How? */
unsigned int ll_num_conflicts = 0;
unsigned int ll_candidate_ip = 0;

/* Task information for the User Interface task*/
LL_THREAD ll_main_thread;
static unsigned char ll_stack[2048];

/******************************************************************************
 * Private Functions
 ******************************************************************************/

/* Generate random IP address between 169.254.1.0 and 169.254.254.255 */
/* ONLY WORKS ON LITTLE ENDIAN MACHINE! */
unsigned int ll_random_ip()
{
	unsigned int ip_addr;
	ip_addr = ll_random(0x0100, 0xfeff) << 16;
	ip_addr = ip_addr | (254<<8) | 169;
	return ip_addr;
}

/* This is a handler that monitors the ARP traffic.  It should have an
 * opportunity to inspect the ARP traffic before the ARP system gets to see it.
 */
LL_PACKET_STATUS ll_handle_arp(LL_PACKET *pkt)
{

	unsigned char *link_header = LL_LINK_HEADER(pkt);
	unsigned short type = *(unsigned short *)(link_header + PACKET_TYPE_OFFSET);
	
	if (type == ARP_TYPE) {
		/* This is an ARP packet. */

		/* Are we probing? */
		/* Is the sender_ip == ll_candidate_ip? */
		/* Is the target_ip == ll_candidate_ip && sender_mac != my_mac */
		/* ll_num_conflicts++; */
		DEBUG("ARP PACKET!\r\n");
		
	}

	return LL_PACKET_PASS;
}

/* This is the main thread routine.  It is a state machine that implements RFC
 * 3927 for Link Local Addressing. 
 */
void ll_main(unsigned long data)
{
	unsigned long timeout = 0;
	int status = LL_SLEEP_END;
	unsigned int probe = 0, announce = 0;

	while(1) {

		status = ll_thread_sleep(timeout, &ll_main_thread);
		if( (status != LL_SLEEP_END) && (status != LL_AWOKEN) ) {
			/* This should never happen! */
			return;
		}

		switch (ll_state) {
			
		case LL_NO_ADDRESS:
			/* Wait for random interval, then probe candidate ip */
			ll_candidate_ip = ll_random_ip();
			timeout = ll_random(0, PROBE_WAIT*1000);
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

			if(status == LL_SLEEP_END) {
				/* We're probing, and our address hasn't been claimed */
				if(probe++ < PROBE_NUM) {
					/* send probe then wait */
					timeout = ll_random(PROBE_MIN*1000, PROBE_MAX*1000);
				} else {
					/* Yay!  We can try claiming this address. */
					timeout = ANNOUNCE_WAIT;
					ll_state = LL_ANNOUNCE;
				}
			} else if(status == LL_AWOKEN) {
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

			if(status == LL_SLEEP_END) {
				/* We're announcing, and there's no conflict yet */
				if(announce++ < ANNOUNCE_NUM) {
					/* send announcement then wait */
					timeout = ANNOUNCE_INTERVAL;
				} else {
					/* We have claimed this address. */
					timeout = ANNOUNCE_WAIT;
					ll_state = LL_ADDR;
				}
			} else if(status == LL_AWOKEN) {
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
			if(status == LL_SLEEP_END) {
				/* Our address is available. */
				timeout = 0;
				ll_state = LL_ADDR;
			} else if(status == LL_AWOKEN) {
				/* The ARP handler saw a conflict, so choose another addr */
				ll_state = LL_RATE_LIMITED_PROBE;
				ll_candidate_ip = 0;
				timeout = RATE_LIMIT_INTERVAL;
			}
			break;

		case LL_ADDR:
			if(status == LL_SLEEP_END) {
				/* Finally!  We have an address.  Initialize the TCP/IP stack
				 * and go to sleep forever. */
				ll_num_conflicts = 0;
				timeout = LL_FOREVER;

				/* If we ever wake up, it's because somebody stole our address
				 * and we need to get another. */
				ll_state = LL_NO_ADDRESS;
				
			} else if(status == LL_AWOKEN) {
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
 * Return values: LL_SUCCESS for success, other for failure.
 */
LL_STATUS ll_init(void)
{
	LL_STATUS ret;

	ll_state = LL_NO_ADDRESS;
	ll_num_conflicts = 0;
	ll_candidate_ip = 0;

	/* Install the ARP packet inspector */
	ret = ll_install_rx_handler(ll_handle_arp);
	if(ret != LL_SUCCESS)
		return ret;

	/* Launch the link-local main thread. */
	ret = ll_thread_create(&ll_main_thread, ll_main, (void *)&ll_stack[0],
						   sizeof(ll_stack));
	if(ret == LL_THREAD_CREATED)
		ret = LL_SUCCESS;

	return ret;
}

/*
 * Return values: LL_SUCCESS for success, other for failure.
 */
LL_STATUS ll_shutdown(void)
{
	LL_STATUS ret;
	ret = ll_thread_terminate(&ll_main_thread);
	if(ret == LL_THREAD_HALTED)
		ret = ll_thread_delete(&ll_main_thread);
	if(ret == LL_THREAD_DELETED)
		ret = LL_SUCCESS;
	return ret;
}
