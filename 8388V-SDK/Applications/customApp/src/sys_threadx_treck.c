/******************************************************************************
 * sys_threadx_treck.c
 *
 * This file implements the system abstraction for cozybit components on
 * threadx/treck systems.  See system.h for details on how each function works.
 *
 * (c) 2007 cozybit, inc.
 ******************************************************************************/

#include "system.h"

/******************************************************************************
 * Packet Handler Interface
 ******************************************************************************/

sys_status sys_add_rx_pkt_handler(sys_pkt_handler handler)
{
	if(mli_installCustomRxDataHandler(handler) == 0)
		return SYS_SUCCESS;

	return SYS_FAIL;
}

extern MLI_CUSTOM_DATA_RX_HANDLER_TYPE rxDataHandler[MLI_MAX_CUSTOM_HANDLERS];

sys_status sys_remove_rx_pkt_handler(sys_pkt_handler handler)
{
	if(mli_removeCustomRxDataHandler(handler) == 0)
		return SYS_SUCCESS;

	return SYS_FAIL;
}

/******************************************************************************
 * Thread Interface
 ******************************************************************************/

sys_status sys_thread_create(sys_thread *t, sys_thread_entry entry,
							 sys_thread_data data, void *stack,
							 unsigned int stack_size)
{
	return tx_thread_create(t,"cozybit thread", entry, data, stack,
							stack_size, 20, 20, 2, TX_AUTO_START);
	
}

sys_status sys_thread_halt(sys_thread *t)
{
	return tx_thread_terminate(t);
}

sys_status sys_thread_delete(sys_thread *t)
{
	return tx_thread_delete(t);
}

sys_status sys_thread_sleep(sys_time ms, sys_thread *t)
{
	unsigned int ticks = ms / SYS_MSEC_PER_TICK;

	if(t == 0)
		t = tx_thread_identify();

	if(ms == SYS_FOREVER)
		return tx_thread_suspend(t);

	return tx_thread_sleep(ticks);
}

sys_status sys_thread_wake(sys_thread *t)
{
	return tx_thread_resume(t);
}

/******************************************************************************
 * Queue Interface
 ******************************************************************************/

/* For now, to avoid dynamic memory allocation, we totally fudge this queue
 * interface.  It allocates one queue from this static memory.  This is a totaly
 * unsafe implementation thread wise.
 */

static sys_queue *q_ptr = 0;

#define Q_SIZE 1024
static char q_mem[Q_SIZE];
static int item_size_in_queue = 0;
static int item_size_out_of_queue = 0;

sys_status sys_queue_create(sys_queue *q, unsigned int q_items, int q_item_size)
{
	int item_size;

	if(q_ptr != 0)
		/* Somebody already owns the only queue. */
		return SYS_FAIL;

	/* Round item_size up to the nearest supported item size */
	item_size = TX_1_ULONG;
	while(item_size*4 < q_item_size) {
		item_size <<= 1;
		if(item_size > TX_16_ULONG)
			/* We don't support items bigger than TX_16_ULONG */
			return SYS_FAIL;
	}

	/* Now check for enough memory */
	if( (q_items * item_size * 4) > Q_SIZE )
		return TX_NO_MEMORY;

	item_size_in_queue = item_size * 4;
	item_size_out_of_queue = q_item_size;

	q_ptr = q;

	return tx_queue_create(q, "cozybit queue", item_size,
						   q_mem, Q_SIZE);
}

sys_status sys_queue_delete(sys_queue *q)
{
	q_ptr = 0;
	return tx_queue_delete(q);
}

sys_status sys_queue_put(sys_queue *q, void *qdata, sys_time ms)
{
	unsigned int ticks;
	char *item[TX_16_ULONG * 4];
	sys_status stat;

	if(ms == SYS_FOREVER)
		ticks = TX_WAIT_FOREVER;
	else
		ticks = ms / SYS_MSEC_PER_TICK;

	memset(item, 0, item_size_in_queue);
	memcpy(item, qdata, item_size_out_of_queue);

	stat = tx_queue_send(q, item, ticks);

	return stat;
}

sys_status sys_queue_get(sys_queue *q, void *qdata, sys_time ms)
{
	unsigned int ticks;
	char *item[TX_16_ULONG * 4];
	sys_status stat;

	if(ms == SYS_FOREVER)
		ticks = TX_WAIT_FOREVER;
	else
		ticks = ms / SYS_MSEC_PER_TICK;

	stat = tx_queue_receive(q, item, ticks);

	if(stat == TX_SUCCESS)
		memcpy(qdata, item, item_size_out_of_queue);

	return stat;
}

/******************************************************************************
 * Mutex Interface
 ******************************************************************************/

sys_status sys_mutex_create(sys_mutex *m)
{
	return tx_mutex_create(m, "cozybit mutex", TX_INHERIT);
}

sys_status sys_mutex_delete(sys_mutex *m)
{
	return tx_mutex_delete(m);
}

sys_status sys_mutex_get(sys_mutex *m, sys_time ms)
{
	unsigned int ticks;

	if(ms == SYS_FOREVER)
		ticks = TX_WAIT_FOREVER;
	else
		ticks = ms / SYS_MSEC_PER_TICK;

	return tx_mutex_get(m, ticks);
}

sys_status sys_mutex_release(sys_mutex *m)
{
	return tx_mutex_put(m);
}

/******************************************************************************
 * Random Number Generator Interface
 ******************************************************************************/

unsigned short sys_random(unsigned short floor, unsigned short ceiling)
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


/******************************************************************************
 * Link-Layer Network Interface
 ******************************************************************************/

/* This function is in the Marvell SDK, but not in the headers. */
extern int GetMACAddr(void *, char *);
sys_status sys_link_get_mac(char mac[6])
{
	return GetMACAddr(NULL, mac);
}

#define RAW_SOCKET_DATA 0x07
#define RAW_DATA 0x08

void done(wcb_t *buffer)
{
	DEBUG("HERE!!");
}

sys_status sys_link_sendto(char mac_dest[6], short type, char *payload,
						   unsigned int len)
{
	wcb_t *buffer;
	char *wp;
	char mac[6];

	buffer = (wcb_t *)mli_GetTxFreeBuffer(); /* How big is it?? */
	if(buffer == 0)
		return SYS_FAIL;
	
	/* Create packet */
	memset((void*)buffer, 0x00, sizeof(wcb_t));	
	buffer->TxPacketType = RAW_DATA;
	buffer->TxPacketOffset = sizeof(wcb_t);
	wp = (char *)buffer + sizeof(wcb_t);

	/* Create mac header */
	memcpy((void *)(&buffer->TxDestAddrHigh), mac_dest, 6);
	memcpy(wp, mac_dest, 6);
	wp += 6;
	if(sys_link_get_mac(mac) != SYS_SUCCESS) {
		/* Should free buffer here, but don't know how! */
		return SYS_FAIL;
	}
	memcpy(wp, mac, 6);
	wp += 6;
	*(short *)wp = htons(type);
	wp += sizeof(short);
	buffer->TxPacketLength = 6 + 6 + sizeof(short);
	
	/* Copy payload */
	memcpy(wp, payload, len);
	buffer->TxPacketLength += len;

	/* Send packet */
	mli_SendTxBuffer((uint8*)buffer); /* error code? */
	return SYS_SUCCESS;
}

/******************************************************************************
 * TCP/IP-Layer Network Interface
 ******************************************************************************/

extern char ip_addr[4];
extern char net_mask[4];
extern char def_gtwy[4];
extern int tcp_ready;
extern 

sys_status sys_tcpip_init(unsigned int ip, unsigned int netmask)
{
	/* This code borrowed from cmd_parser.c.  It uses lots of global variables
	 * and doesn't check for errors.  Yuck.
	 */
	memcpy(ip_addr, &ip, 4);
	memcpy(net_mask, &netmask, 4);
	memset(def_gtwy, 0, 4);
	userif_prepare_config_etcp();
	tcp_ready = 1;

	/* Eventually, command thread will config stack. */

	return SYS_SUCCESS;
}

sys_status sys_tcpip_halt(void)
{
	/* Functionality not exported by SDK. */
	memset(ip_addr, 0, 4);
	memset(net_mask, 0, 4);
	memset(def_gtwy, 0, 4);
	tcp_ready = 0;
	return SYS_SUCCESS;
}
