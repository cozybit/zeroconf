/*
*                Copyright 2006, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
 *
 *  Public Procedures:
 *     ** Initialization
 *     os_OsInit             Initialize OS structures
 *
 *     ** Task management
 *     os_TaskCreate         Creates a new task
 *     os_TaskSuspend        Suspends a task
 *     os_TaskResume         Start or resume a task
 *     os_TaskKill           Kills a task
 *     os_TaskDelay          Delays the currently running task
 *
 *     ** Queues
 *     os_QueueCreate        Create a queue
 *     os_QueueDelete        Delete a queue
 *     os_QueueReadNoBlock   Read a message from a queue without blocking
 *     os_QueueWriteNoBlock  Write a message to a queue without blocking
 *     os_QueuePeek          Retrieve how many messages are in a queue
 *
 *     ** Interrupt Handling
 *     os_InterruptCreate    Create an interrupt
 *     os_InterruptAttach    Attach a hardware interrupt to an ISR
 *     os_InterruptDisable   Disable an interrupt
 *     os_InterruptEnable    Enable an Interrupt
 *     
 *     ** Synchronization
 *     os_SemaphoreInit      Initialize a semaphore
 *     os_SemaphoreDestroy   Destroy a semaphore
 *     os_SemaphoreGet       Get a semaphore
 *     os_SemaphorePut       Return a semaphore
 *
 *     ** Events
 *     os_EventInit          Initialize an event flag
 *     os_EventTrigger       Trigger an event to occur
 *     os_EventWait          Wait for an event to occur
 *     os_EventDestroy       Destroy an event flag
 *     os_EventClear         Clear events from an event flag
 *
 *     ** Timer
 *     os_TimerCreate        Create a timer
 *     os_TimerInit          Initialize a timer
 *     os_TimerDelete        Delete a timer
 *     os_TimerEnable        Enable a timer
 *     os_TimerDisable       Disable a timer
 *     
 *  Private Procedures:
 *     None.
 *
 *  Notes:
 *     None.
 *****************************************************************************
# History:
#          5/14/06   RamyaC  Including comments for DOXYGEN style documentation

*/

/**
 * @file os_if.h
 *
 * @brief This file contains the function prototypes and definitions for the OS
 *  wrapper.
 *
 * 
 *
 * @author (c) Marvell Semiconductor, Inc.
 */

#ifndef __OS_IF_H__
#define __OS_IF_H__


/**
 *
 * @brief INCLUDE FILES
 *
 * 
 *
 **/
#include "wltypes.h"

#include <string.h> /**< for memcpy, memset*/

#define os_SUCCESS             0
#define os_FAIL                1
#define os_ERR_NO_HANDLE       0x00000099
#define os_ERR_ILLEGAL_ID      0x00000098
#define os_ERR_HANDLE_IN_USE   0x00000097

#define os_START               cyg_user_start

typedef UINT32    os_ISR_t( UINT32 Vector, UINT32 Data );
typedef void      os_DSR_t( UINT32 Vector, UINT32 Count, UINT32 Data );
typedef void      os_Timer_t(UINT32 handle, UINT32 Data );
typedef UINT32    os_Addrword_t;
typedef UINT32    os_Id_t;
typedef void 	  os_TaskEntry_t(UINT32);
typedef UINT32    os_Ucount32;
typedef UINT32    os_Vector_t;
typedef UINT32    os_Priority_t;
typedef UINT32    os_IndicatedEvents_t;
typedef UINT32    os_EventMode_t;
typedef UINT32        os_Alarm_t;
typedef UINT32        os_TickCount_t;

#define os_EVENT_WAITMODE_OR       0
#define os_EVENT_WAITMODE_AND      2
#define os_EVENT_WAITMODE_CLR      0
#define os_EVENT_WAITMODE_CLR_AND  3
#define os_EVENT_WAITMODE_CLR_OR   1

#define os_EnterCriticalSection os_if_EnterCriticalSection()
#define os_ExitCriticalSection  os_if_ExitCriticalSection()
extern void os_if_EnterCriticalSection(void);
extern void os_if_ExitCriticalSection(void);

extern UINT32 os_if_save_EnterCriticalSection(void);
extern void os_if_save_ExitCriticalSection(UINT32 /* flags */);

#define OS_ISR_HANDLED 1
#define OS_ISR_CALL_DSR	2


/** @defgroup OSAL Operating System Interface functions
 *  Functions exported by os_if.h
 *  @{
 */

/**
 *
 * @brief  PUBLIC DATA
 *
 * 
 *
 **/
#define os_SUCCESS             0 /**< STATUS **/
#define os_FAIL                1
#define os_ERR_NO_HANDLE       0x00000099
#define os_ERR_ILLEGAL_ID      0x00000098
#define os_ERR_HANDLE_IN_USE   0x00000097

#define ENTER_CRITICAL  os_EnterCriticalSection
#define EXIT_CRITICAL   os_ExitCriticalSection


/** @brief PUBLIC PROCEDURES (ANSI Prototypes)
 *
 * 
 *
*/


/** @brief os_OsInit: Initialize OS structures
 *
 * This function is installed by os_if.h during system initialisation. 
 * It is responsible for OS initializations
 *
 * @param :None
 *
 * 
 * @return :None
 */


extern WL_STATUS os_OsInit( void );

/** @brief os_TaskCreate: Creates a new task
 *
 * This routine is called to create a task.The OS initialization routine should already have been executed.
 *
 *
 * @param TaskId		: Id (index) of the task to be created
 * @param Entry     		: Pointer to the routine executed as a task
 * @param Data   		: Data supplied to the task routine
 * @param TaskName  		: String name of the task
 * @param StackSize 		: Stack size used by the task
 * @param Priority 		: Priority of the task
 * 
 * @return Sucess		: Sucess of task creation
 * Failure			: Failure of task creation
 *
 */

extern WL_STATUS os_TaskCreate( os_Id_t         TaskId,
                               os_TaskEntry_t *Entry_p,
                               os_Addrword_t   Data,
                               char           *TaskName_p,
                               os_Ucount32     StackSize,
                               os_Addrword_t   Priority );


/** @brief os_TaskSuspend	: Suspends a task
 *
 * This routine is called to suspend a task.The task to be suspended should been created already.
 * 
 * @param TaskId		: Id (index) of the task to be suspended
 *
 * 
 * @return 			: None
 *
 */


extern void os_TaskSuspend( os_Id_t TaskId );

/** @brief os_TaskResume	: Start or resume a task
 *
 * This routine is called to resume a task.The task to be suspended should been created already.
 * 
 * @param TaskId		: Id (index) of the task to be suspended
 *
 * 
 * @return 			: None
 *
 */


extern void os_TaskResume( os_Id_t TaskId );

/** @brief os_TaskKill		: Kills a task
 **
 * This routine is called to kill a given task.The task to be suspended should been created already.
 * 
 * @param TaskId		: Id (index) of the task to be suspended
 *
 * 
 * @return 			: None
 *
 */

extern void os_TaskKill( os_Id_t TaskId );

/** @brief os_TaskDelay		: Delays the currently running task
 **
 * This routine is called to put the currently running task in sleep modefor given time
 * 
 * @param Count			: The number of ticks to delay where 1 tick = 100 us
 *
 * 
 * @return 			: None
 * PDL:
 *    Call cyg_thread_delay() to delay the currently task
 * END PDL
 *
 */


extern void os_TaskDelay( os_TickCount_t Count );


/** @brief os_QueueCreate	: Create a queue
 **
 * This routine is called to create a message queue for a task. The OS initialization routine has already been executed
 * 
 * @param QueueId		: Id (index) of the queue to be created
 *
 * 
 * @return Sucess		: Sucess of task creation
 * Failure			: Failure of task creation
 * 
 *
 */


extern UINT8 os_QueueCreate( os_Id_t QueueId );

/** @brief os_QueueDelete	: Delete a queue
 **
 *  This routine is called to delete a given queue. The queue to be deleted has been created.
 * 
 * @param QueueId		: Id (index) of the queue to be created
 *
 * 
 * @return 			: None
 * 
 *
 */


extern void os_QueueDelete( os_Id_t QueueId );

/** @brief os_QueueReadNoBlock	: Read a message from a queue without blocking
 **
 *  This routine is called to read a message from a queue; if there are no messages on the queue it returns immediately (with	out blocking).The queue to read from should have been created
 * 
 * @param QueueId		: Id (index) of the queue to be created
 *
 * 
 * @return 			: None
 * 
 *
 */


extern void* os_QueueReadNoBlock( os_Id_t QueueId );

/** @brief os_QueueWriteNoBlock	: Write a message to a queue without blocking
 **
 * This routine writes a message to a queue; if the message cannot be written, the routine returns immediately (without blocking).The queue to write from should have been created
 * 
 * @param QueueId		: Id (index) of the queue to be created
 * @param Item			: Pointer to the message to be written to the queue
 * 
 * @return Sucess		: Sucess of task creation
 * Failure			: Failure of task creation
 * 
 *
 */


extern WL_STATUS os_QueueWriteNoBlock( os_Id_t QueueId, void *Item_p );

/** @brief os_QueuePeek		: Retrieve how many messages are in a queue
 **
 *This routine is called to find out how many messages are in a queue.The queue to peek from should have been created
 * 
 * @param QueueId		: Id (index) of the queue to be created
 * 
 * 
 * @return 			: None
 * 
 ** PDL:
 *    If the queue Id is valid Then
 *       Call cyg_mbox_peek() to get the number of messages in the queue
 *       Return the number of message found in the queue
 *    Else
 *       Return 0
 *    End If
 * END PDL
 */


extern UINT32 os_QueuePeek( os_Id_t QueueId );

/** @brief  os_InterruptCreate	: Create an interrupt
 **
 * This routine is called to create a interrupt object.The OS initialization routine should already have been executed
 * 
 * 
 * @param InterruptId 		: Id (index) of the interrupt to be created
 * @param Vector      		: Vector associated with the interrupt to be created
 * @param Priority    		: Priority of the interrupt to be created
 * @param *Isr_p      		: Pointer to the interrupt service routine handler
 * @param *Dsr_p      		: Pointer to the deferred service routine  handler
 * 
 * @return Sucess		: Sucess of task creation
 * Failure			: Failure of task creationy
 * 
 *
 */

extern WL_STATUS os_InterruptCreate( os_Id_t        InterruptId,
                                    os_Vector_t    Vector,
                                    os_Priority_t  Priority,
                                    os_ISR_t      *Isr_p,
                                    os_DSR_t      *Dsr_p );


/** @brief os_InterruptAttach	: Attach a hardware interrupt to an ISR
 **
 * This routine is called to attach an interrupt to the vector specified
 *   in the create routine. This allows for hardware interrupts to be
 *   triggered according to the vector and processed by the ISR/DSR given
 *   in the create routine.The interrupt object should have been created already.
 * 
 * @param InterruptId		: Id (index) of the interrupt to be attached
 *
 * 
 * @return			: None
 * 
 *
 */


extern void os_InterruptAttach( os_Id_t InterruptId );

/** @brief os_InterruptDisable	: Disable an interrupt
 **
 * This routine is called to disable all ISRs
 * 
 * 
 * @return			: None
 * 
 *
 */


extern void os_InterruptDisable( void );


/** @brief os_InterruptDisable	: Enable an interrupt
 **
 * This routine is called to enable all ISRs
 *  * @return Sucess		: Sucess of task creation
 * Failure			: Failure of task creation
 * 
 * @return			: None
 * 
 *
 */

extern void os_InterruptEnable( void );


/** @brief os_InterruptMask	: Mask given interrupt
 **
 * This routine is called to mask (disable) given interrupts specified by
 *   the supplied vector.
 * 
 * @param Vector		: Value whose bits indicate which interrupts to mask
 * 
 * @return			: None
 * 
 *
 */


extern void os_InterruptMask( os_Vector_t Vector );

/** @brief os_InterruptUnmask	: Unmask given interrupt
 **
 * This routine is called to unmask (enable) given interrupts specified by
 *   the supplied vector.
 * 
 * @param Vector		: Value whose bits indicate which interrupts to unmask
 * 
 * @return			: None
 * 
 *
 */


extern void os_InterruptUnmask( os_Vector_t Vector );

/** @brief os_InterruptAck	: Acknowledge given interrupt
 **
 *This routine is used to acknowledge receipt of an interrupt; it is
 *   called from within an ISR.Interrupts must be
 *   acknowledged using this routine to prevent the interrupt from
 *   reoccurring.
 * 
 * @param Vector		: Value whose bits indicate which interrupts to unmask
 * 
 * @return			: None
 * 
 * * PDL:
 *    Call cyg_interrupt_acknowledge() to acknowledge the interrupt
 * END PDL
 */


extern void os_InterruptAck( os_Vector_t Vector );


/** @brief os_SemaphoreInit	: Initialize a semaphore
 **
 * This routine is called to initialize a semaphore.
 * 
 * @param SemId			: Id (index) of the semaphore to be initialized
 * @param Count			: The count the semaphore is initialized to
 * 
 * @return			: None
 * 
 * 
 */
extern void os_SemaphoreInit( os_Id_t SemId, os_Ucount32 Count );


/******************************************************************************
 *
 * Name: os_SemaphoreDestroy
 *
 * Description:
 *   This routine is called to destroy a semaphore.
 *
 * Conditions For Use:
 *   The semaphore has been initialized.
 *
 * Arguments:
 *   Arg1 (i  ): SemId - Id (index) of the semaphore to be destroyed
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   This operation must not be performed while there are any threads
 *   waiting on the semaphore.
 *
 *****************************************************************************/
extern void os_SemaphoreDestroy( os_Id_t SemId );


/******************************************************************************
 *
 * Name: os_SemaphoreGet
 *
 * Description:
 *   This routine is called to claim a semaphore; if one is not available,
 *   then the calling task waits until the semaphore is available.
 *
 * Conditions For Use:
 *   The semaphore has been initialized.
 *
 * Arguments:
 *   Arg1 (i  ): SemId - Id (index) of the semaphore to be claimed
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   When a semaphore gets claimed by a task, the semaphore count is
 *   decremented; when the count goes to zero, any tasks wishing to claim
 *   a semaphore must wait on the semaphore.
 *
 *****************************************************************************/
extern void os_SemaphoreGet( os_Id_t SemId );


/******************************************************************************
 *
 * Name: os_SemaphoreGetNoWait
 *
 * Description:
 *   This routine is called to claim a semaphore; 
 *      return immediately if no one is available.
 *
 * Conditions For Use:
 *   The semaphore has been initialized.
 *
 * Arguments:
 *   Arg1 (i  ): SemId - Id (index) of the semaphore to be claimed
 *
 * Return Value:
 *   SUCCESS : Semaphore acquisition was okay
 *   FAIL : Fail to acquire semaphore since the semaphore count is 0
 * 
 * Notes:
 *
 *****************************************************************************/
extern Status_e os_SemaphoreGetNoWait(os_Id_t SemId);

/******************************************************************************
 *
 * Name: os_SemaphorePut
 *
 * Description:
 *   This routine is called to return a semaphore.
 *
 * Conditions For Use:
 *   The semaphore has been initialized.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   By returning a semaphore, a waiting task is woken to claim the
 *   semaphore; if there are no waiting tasks, the semaphore count is
 *   incremented.
 *
 *****************************************************************************/
extern void os_SemaphorePut( os_Id_t SemId );


/******************************************************************************
 *
 * Name: os_EventInit
 *
 * Description:
 *   This routine is called to initialize an event flag.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   Arg1 (i  ): EventId - Id (index) of the event to be initialized
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
extern void os_EventInit( os_Id_t EventId );


/******************************************************************************
 *
 * 
 * @param EventsToTrigger	: The event(s) to be triggered
 * 
 * @return			: None
 * 
 * 
 */


extern void os_EventTrigger( os_Id_t              EventId,
                             os_IndicatedEvents_t EventsToTrigger );

/** @brief os_EventWait		: Wait for an event to occur
 **
 * This routine is called to wait on a given event(s) for a given event flag.The event should have been initialized
 * 
 * @param EventId		: Id (index) of the event to be initialized
 * 
 * @param EventsToWaitFor	: The event(s) to be waited on
 * 
 * @param Mode			: Mode specifying the conditions for waking up on the event(s) waited for
 * 
 * @return			: None
 * 
 * 
 */


extern os_IndicatedEvents_t os_EventWait(
                                        os_Id_t              EventId,
                                        os_IndicatedEvents_t EventsToWaitFor,
                                        os_EventMode_t       Mode );

/** @brief os_EventDestroy	: Destroy an event flag
 **
 * This routine is called to destroy an event.The event should have been initialized
 * 
 * @param EventId		: Id (index) of the event to be initialized
 * 
 * 
 * @return			: None
 * 
 * 
 */


extern void os_EventDestroy( os_Id_t EventId );

/** @brief os_EventPeek		: Peek an event flag
 **
 * This routine is called to peek for a given event flag.The event should have been initialized
 * 
 * @param EventId		: Id (index) of the event to be initialized
 * 
 * 
 * @return			: None
 * 
 * 
 */

extern os_IndicatedEvents_t os_EventPeek(os_Id_t EventId );


/** @brief os_EventClear	: Clear events from an event flag
 **
 * This routine is called to clear events in an event flag.The event should have been initialized
 * 
 * @param EventId		: Id (index) of the event to be initialized
 * 
 * @param EventsToClear		: The event(s) to be cleared
 *  
 * @return			: None
 * 
 * 
 */


extern void os_EventClear( os_Id_t               EventId,
                           os_IndicatedEvents_t  EventsToClear );

/** @brief Name: os_TimerCreate	: Create a timer
 **
 * This routine is called to create a time. The OS initialization routine
 *   should have been executed already
 * 
 * @param TimerId 		: Id (index) of the timer to be created
 * 
 * @param TimerFunc		:  Pointer to the routine called when a timer expires
 *  
 * @param Data 			:   Data supplied to the timer function
 *
 * @return Sucess		: Sucess of task creation
 * Failure			: Failure of task creation
 * 
 * 
 */
extern WL_STATUS os_TimerCreate( os_Id_t        TimerId,
                                 os_Timer_t    *TimerFunc_p,
                                 os_Addrword_t  Data );


/******************************************************************************
 *
 * Name: os_TimerInit
 *
 * Description:
 *   This routine is called to initialize a timer.
 *
 * Conditions For Use:
 *   The timer has been created.
 *
 * Arguments:
 *   Arg1 (i  ): TimerId  - Id (index) of the timer to be initialized
 *   Arg2 (i  ): Trigger  - The tick at which the timer is to trigger
 *   Arg3 (i  ): Interval - Number of ticks before the timer triggers again
 *                          (if 0, the timer is disabled after the initial
 *                          trigger).
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
extern void os_TimerInit( os_Id_t         TimerId,
                          os_TickCount_t  Trigger,
                          os_TickCount_t  Interval );


/******************************************************************************
 *
 * Name: os_TimerDelete
 *
 * Description:
 *   This routine is called to delete a timer.
 *
 * Conditions For Use:
 *   The timer has been created.
 *
 * Arguments:
 *   Arg1 (i  ): TimerId - Id (index) of the timer to be deleted
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
extern void os_TimerDelete( os_Id_t TimerId );


/******************************************************************************
 *
 * Name: os_TimerEnable
 *
 * Description:
 *   This routine is called to enable a timer.
 *
 * Conditions For Use:
 *   The timer has been created.
 *
 * Arguments:
 *   Arg1 (i  ): TimerId - Id (index) of the timer to be enabled
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
extern void os_TimerEnable( os_Id_t TimerId );


/******************************************************************************
 *
 * Name: os_TimerDisable
 *
 * Description:
 *   This routine is called to disable a timer.
 *
 * Conditions For Use:
 *   The timer has been created.
 *
 * Arguments:
 *   Arg1 (i  ): TimerId - Id (index) of the timer to be disabled
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
extern void os_TimerDisable( os_Id_t TimerId );



/******************************************************************************
 *
 * Name: os_Time
 *
 * Description:
 *   Return current time.
 *
 * Conditions For Use:
 *   The OS initialization routine has already been executed.
 *
 * Arguments:
 *  None
 *
 *
 * Return Value:
 *    Current Time 
 *
 *****************************************************************************/
extern void os_Time(uint32 * value);

typedef void *MHANDLE;
extern void os_var_pool_create( void *, UINT32, MHANDLE *, void * );
extern void *os_var_alloc( MHANDLE handle, UINT32 size );
extern void os_var_free( MHANDLE handle, void *buf_p );
extern void os_fix_pool_create( void *, UINT32, UINT32, MHANDLE *, void * );
extern void *os_fix_alloc( MHANDLE handle );
extern void os_fix_free( MHANDLE handle, void *buf_p );

extern void os_Time(uint32 * value);
extern void os_ConditionInit(os_Id_t CondsId);

extern void cyg_user_start( void );

extern uint32 timer_isr(uint32 vector, uint32 data);

#endif /* __OS_IF_H__ */
