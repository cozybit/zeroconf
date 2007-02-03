/******************************************************************************
 * system.h
 *
 * This file represents the system abstraction for cozybit components.  Porting
 * cozybit components between systems should be as simple as implementing the
 * functions in this header file.
 *
 * (c) 2007 cozybit, inc.
 ******************************************************************************/

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

/******************************************************************************
 * Platform-Specific Macros and Types
 ******************************************************************************/

#if defined SYS_THREADX_TRECK

/* System-specific includes */
#include "wltypes.h"
#include "mli.h"
#include "dbg_Facility.h"
#include "trsocket.h"
#include "trmacro.h"
#include "trtype.h"
#include "trproto.h"
#include "userif.h"

/* Return Values */
#define	SYS_SUCCESS TX_SUCCESS
#define SYS_FAIL TX_START_ERROR
#define SYS_THREAD_AWOKEN TX_WAIT_ABORTED
#define SYS_QUEUE_EMPTY TX_QUEUE_EMPTY
#define SYS_QUEUE_FULL TX_QUEUE_FULL
#define SYS_MUTEX_IN_USE
typedef unsigned int sys_status;

/* Time Types */
#define SYS_TICKS_PER_SEC 100 /* ThreadX ticks are 10ms long. */
#define SYS_MSEC_PER_TICK 10
#define SYS_FOREVER TX_WAIT_FOREVER
typedef unsigned int sys_time;

/* Packet Types */
#define	SYS_PKT_PASS MLI_CUSTOM_DATA_NOT_MY_PKT
#define	SYS_PKT_CONSUME MLI_CUSTOM_DATA_PKT_NULL
typedef MLI_CUSTOM_DATA_T sys_pkt_status;
typedef RxPD_t sys_pkt;
typedef sys_pkt_status (*sys_pkt_handler)(sys_pkt *pkt);

#define L2_HEADER_OFFSET (sizeof(RxPD_t)) /* native packet to L2 header */
#define L2_TYPE_OFFSET 20 /* Offset from L2 packet to SNAP type */
#define ARP_OFFSET 22 /* Offset from L2 packet to ARP */

/* Thread Types */
typedef TX_THREAD sys_thread;
typedef unsigned long sys_thread_data;
typedef void sys_thread_return;
typedef sys_thread_return (*sys_thread_entry)(sys_thread_data data);

/* Queue Types */
typedef TX_QUEUE sys_queue;

/* Mutex Types */
typedef TX_MUTEX sys_mutex;

/* Link Layer Types */

/* Debug print macro */
#define DEBUG(...) DBG_P(( DBG_L0 __VA_ARGS__));

/* Compiler stuff */
#define PACKED_STRUCT_BEGIN __packed
#define PACK_STRUCT_END

#elif defined SYS_LINUX

#error SYS_LINUX is not supported yet.

#else

#error No Supported System Selected.

#endif

/******************************************************************************
 * Packet Handler Interface
 ******************************************************************************/

/**
 * sys_add_rx_pkt_handler: Add a packet handler to the packet processing chain.
 *
 * handler: pointer to packet handler function.  This is a function that takes a
 *          pointer to an ethernet packet and returns a sys_pkt_status.
 *
 * Returns: SYS_SUCCESS
 *          SYS_FAIL
 */
sys_status sys_add_rx_pkt_handler(sys_pkt_handler handler);

/**
 * sys_remove_rx_pkt_handler: Remove packet handler to the packet processing
 *                            chain.
 *
 * handler: pointer to packet handler function.
 *
 * Returns: SYS_SUCCESS
 *          SYS_FAIL
 */
sys_status sys_remove_rx_pkt_handler(sys_pkt_handler handler);

/******************************************************************************
 * Thread Interface
 ******************************************************************************/

/**
 * sys_thread_create: Create a thread.
 *
 * t: pointer to sys_thread to create
 * entry: thread entry point function
 * data: data to be passed to entry when entry is called
 * stack: pointer to stack for thread
 * stack_size: size of the stack pointed to by *stack
 *
 * Returns: SYS_SUCCESS
 *          Other: system defined
 */
sys_status sys_thread_create(sys_thread *t, sys_thread_entry entry,
							 sys_thread_data data, void *stack,
							 unsigned int stack_size);

/**
 * sys_thread_halt: Halt a thread
 *
 * t: pointer to thread to be halted
 *
 * Returns: SYS_SUCCESS
 *          Other: system defined
 */
sys_status sys_thread_halt(sys_thread *t);

/**
 * sys_thread_delete: Delete a thread
 *
 * t: pointer to thread to be deleted
 *
 * Returns: SYS_SUCCESS
 *          Other: system defined
 */
sys_status sys_thread_delete(sys_thread *t);

/**
 * sys_thread_sleep: Put current thread to sleep
 *
 * ms: Number of milliseconds to sleep.  Use SYS_FOREVER to sleep forever.
 * t: thread to sleep.  Pass 0 for current the thread.
 *
 * Returns: SYS_SUCCESS: thread slept for entire duration of ms
 *          SYS_THREAD_AWOKEN: thread was awoken prior to duration of ms
 *          Other: system defined
 */
sys_status sys_thread_sleep(sys_time ms, sys_thread *t);

/**
 * sys_thread_wake: Wake the thread
 *
 * t: thread to wake
 *
 * Returns: SYS_SUCCESS
 *          Other: system defined
 */
sys_status sys_thread_wake(sys_thread *t);

/******************************************************************************
 * Queue Interface
 ******************************************************************************/

/**
 * sys_queue_create: Create a queue
 *
 * q: pointer to queue to create
 * q_items: number of elements in q
 * q_item_size: size of each item in queue
 *
 * Returns: SYS_SUCCESS
 *          Other: system defined
 */
sys_status sys_queue_create(sys_queue *q, unsigned int q_items, int q_item_size);

/**
 * sys_queue_delete: Delete a queue
 *
 * q: pointer to queue to delete
 *
 * Returns: SYS_SUCCESS
 *          Other: system defined
 */
sys_status sys_queue_delete(sys_queue *q);

/**
 * sys_queue_put: Put item in queue
 *
 * q: pointer to queue
 * qdata: pointer to item that should be copied into queue.  It should be the
 *        same size as q_item_size used to initialize q.
 * ms: Maximum time in milliseconds to wait for space in queue.  Use SYS_FOREVER
 *     to wait indefinitely, or 0 to not wait at all.
 *
 * Returns: SYS_SUCCESS: Item successfully put in queue
 *          SYS_QUEUE_EMPTY: Timed out waiting for space in queue
 *          Other: system defined
 */
sys_status sys_queue_put(sys_queue *q, void *qdata, sys_time ms);

/**
 * sys_queue_get: Get next item from queue
 *
 * q: pointer to queue
 * qdata: pointer to memory where item from queue should be copied.  It should
 *        be the same size as q_item_size used to initialize q.
 * ms: Maximum time in milliseconds to wait for next item.  Use SYS_FOREVER to
 *     wait indefinitely, or 0 to not wait at all.
 *
 * Returns: SYS_SUCCESS: Item successfully retrieved.
 *          SYS_QUEUE_EMPTY: Timed out waiting for next item.
 *          Other: system defined
 */
sys_status sys_queue_get(sys_queue *q, void *qdata, sys_time ms);

/******************************************************************************
 * Mutex Interface
 ******************************************************************************/

/**
 * sys_mutex_create: Create a mutex
 *
 * m: pointer to mutex to create
 *
 * Returns: SYS_SUCCESS
 *          Other: system defined
 */
sys_status sys_mutex_create(sys_mutex *m);

/**
 * sys_mutex_delete: Delete a mutex
 *
 * m: pointer to mutex to delete
 *
 * Returns: SYS_SUCCESS
 *          Other: system defined
 */
sys_status sys_mutex_delete(sys_mutex *m);

/**
 * sys_mutex_get: Get the mutex
 *
 * m: pointer to mutex
 * ms: Maximum time in milliseconds to wait for mutex.  Use SYS_FOREVER to wait
 * indefinitely, or 0 to not wait at all.
 *
 * Returns: SYS_SUCCESS: You've got the mutex
 *          SYS_MUTEX_IN_USE: Timed out waiting for mutex
 *          Other: system defined
 */
sys_status sys_mutex_get(sys_mutex *m, sys_time ms);

/**
 * sys_mutex_release: Release the mutex
 *
 * m: pointer to mutex
 *
 * Returns: SYS_SUCCESS: Mutex released
 *          Other: system defined
 */
sys_status sys_mutex_release(sys_mutex *m);

/******************************************************************************
 * Random Number Generator Interface
 ******************************************************************************/

/**
 * sys_random: Choose a uniform random number
 *
 * floor: bottom of uniform range
 * ceiling: top of uniform range
 *
 * Returns: Random number chosen uniformly between floor and ceiling.
 */
unsigned short sys_random(unsigned short floor, unsigned short ceiling);

/******************************************************************************
 * Link-Layer Network Interface
 ******************************************************************************/

/**
 * sys_link_get_mac: Retrieve MAC address from system
 *
 * mac: pointer to location to copy the mac address.
 *
 * Returns: SYS_SUCCESS
 *          Other: system defined
 */
sys_status sys_link_get_mac(char mac[6]);

/**
 * sys_link_sendto: Send a packet to mac_dest
 *
 * mac_dest: destination mac address
 * type: type of link packet (i.e., arp, etc.)
 * payload: payoad of packet to be sent
 * len: length of payload to be sent
 *
 * Returns: SYS_SUCCESS
 *          Other: system defined
 *
 * Note: payload should not contain link layer header.
 */
sys_status sys_link_sendto(char mac_dest[6], short type, char *payload,
						   unsigned int len);

/******************************************************************************
 * TCP/IP-Layer Network Interface
 ******************************************************************************/

/**
 * sys_tcpip_init: Initialize TCP/IP stack
 *
 * ip: ip address
 * netmask: netmask
 *
 * Returns: SYS_SUCCESS
 *          Other: system defined
 */
sys_status sys_tcpip_init(unsigned int ip, unsigned int netmask);

/**
 * sys_tcpip_halt: Halt TCP/IP stack
 *
 * Returns: SYS_SUCCESS
 *          Other: system defined
 */
sys_status sys_tcpip_halt(void);

#endif
