/*******************************************************************************************
*
* File: queue.h
*
*
*
* Date:         2005-09-12
* Description:  simple software queue definition.
*
*******************************************************************************************/
#ifndef _QUEUE_H_
#define _QUEUE_H_
#include "wltypes.h"

#define HEAD   1  /* indicate element need to be put in the head of the queue */
#define TAIL   0  /* indicate element need to be put in the tail of the queue */

struct qElement
{
   struct qElement *qElement_next;  /* it's just a pointer to next element */
};

typedef struct qElement *qp;        /* q pointer */

#define qElement qp

typedef struct Queue_t   
{        
   qElement q_head;        /* first element in queue */
   qElement q_tail;        /* last element in queue */
   unsigned int   q_len;         /* number of elements in queue */
   unsigned int   q_max;         /* maximum length */
   unsigned int   q_min;         /* minimum length */
} Queue_t;

/* Prototypes declaration */
extern void putq(Queue_t*, void*);      /* add item to tail of queue */
extern void putq_head(Queue_t*, void*); /* add item to head of queue */
extern void *getq(Queue_t*);            /* de-queue item from head of queue */
extern void *peekq_head (Queue_t *q);   /* Peek the head of the queue */
extern void *peekq_tail (Queue_t *q);   /* Peek the tail of the queue */
extern void flushq (Queue_t *pDestQ, Queue_t *pSrcQ); /* flush src queue to dest queue */

#endif //_QUEUE_H_
