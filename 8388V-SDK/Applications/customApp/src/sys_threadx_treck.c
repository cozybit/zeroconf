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

sys_status sys_remove_rx_pkt_handler(sys_pkt_handler handler)
{
	/* Marvell SDK has no facility to unregister pkt handlers. */
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
							stack_size, 9, 9, 29, TX_AUTO_START);
	
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

sys_status sys_queue_create(sys_queue *q, void *qmem, unsigned int qsize)
{
	return tx_queue_create(q, "cozybit queue", sizeof(sys_queue_item),
						   qmem, qsize*sizeof(sys_queue_item));
}

sys_status sys_queue_delete(sys_queue *q)
{
	return tx_queue_delete(q);
}

sys_status sys_queue_put(sys_queue *q, sys_queue_item *qdata, sys_time ms)
{
	unsigned int ticks;

	if(ms == SYS_FOREVER)
		ticks = TX_WAIT_FOREVER;
	else
		ticks = ms / SYS_MSEC_PER_TICK;

	return tx_queue_send(q, qdata, ticks);
}

sys_status sys_queue_get(sys_queue *q, sys_queue_item *qdata, sys_time ms)
{
	unsigned int ticks;

	if(ms == SYS_FOREVER)
		ticks = TX_WAIT_FOREVER;
	else
		ticks = ms / SYS_MSEC_PER_TICK;

	return tx_queue_receive(q, qdata, ticks);
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
