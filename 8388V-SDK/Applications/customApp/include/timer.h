/*************************************************************
 *
 *   Timer.h                                                
 *   Copy Right(2003) Marvell Semiconductors, Inc.           
 *
 *
 *   Author(s):    Jiaqi Shen                                 
 *   Date:         2003-06-16                                 
 *   Description:  This file defined a Timer list object 
 *
 *************************************************************
 */
#ifndef timer_h
#define timer_h

#include "os_if.h"

#define CALL_BACK 0
#define NO_CALL_BACK 1

typedef struct Timer Timer;

struct Timer
{
    Timer *next;        /*link timers in the list */
    int active;
    int cb;
    unsigned short count;
    unsigned short interval;
    unsigned char *userdata_p;
    void (*callback)(unsigned char *);
};
extern void TimerFireIn(Timer *me, int act, void *callback,
                        unsigned char *data_p, unsigned short ticks);
extern void TimerFireEvery(Timer *me, int act, void *callback,
                           unsigned char *data_p, unsigned short ticks);
extern void TimerDisarm(Timer *me);
extern void TimerRearm(Timer *me, unsigned short ticks);
extern void TimerTick(void);
extern void TimerInit(Timer *me);
extern void TimerRemove(Timer *me);
extern unsigned short TimerGetRemainingTicks(Timer *me);

#endif
