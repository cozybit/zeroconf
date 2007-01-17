/*
*                Copyright 2006, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
# History:
#          5/3/06    sanjayg  Created Initial version

*/
/**
 * @file mli.c
 *
 * @brief This file contains Marvell application Interface APIs.
 *
 * 
 *
 * @author (c) Marvell Semiconductor, Inc.
 */
#include "customApp.h"
#include "mli.h"
#include "os_if.h"
#include "tx_api.h"
#include "queue.h"
#include "sysinfo.h"
//#include "psmgr.h"
//#include "w81TxMDS.h"

extern int IsRxAllowFromMAC(void);
extern int psmgr_IsTxAllowed(void);
extern int TxMacDataService_SendEvent(uint16 Event);

// actual rx data process function
extern void wlan_cf_rx_pkts(void);

// to return the command response
extern void cb_HandleCmdDone(int intr);

//place holder to keep installed command handlers
MLI_CUSTOM_CB_TYPE cmdHandler[MLI_MAX_CUSTOM_HANDLERS];
static int numberCustomCommandHandlers = 0;

#ifdef SDK_FEATURE_XX
MLI_CUSTOM_RCB_TYPE respHandler[MLI_MAX_CUSTOM_HANDLERS];
static int numberCustomResponseHandlers = 0;

MLI_CUSTOM_ECB_TYPE evtHandler[MLI_MAX_CUSTOM_HANDLERS];
static int numberCustomEventHandlers = 0;
#endif

// place holder for installed Tx data handlers
MLI_CUSTOM_DATA_TX_HANDLER_TYPE txDataHandler[MLI_MAX_CUSTOM_HANDLERS];
static int numberTxDataHandlers = 0;

//place holder for installed Rx data handlers
MLI_CUSTOM_DATA_RX_HANDLER_TYPE rxDataHandler[MLI_MAX_CUSTOM_HANDLERS];
static int numberRxDataHandlers = 0;

//place holder for installed tick functions
MLI_TICKFUNC tickfunctions[MLI_MAX_CUSTOM_HANDLERS];
static int numberCustomTickfunctions = 0;

//SDK uses separate task for Rx data handling legacy firmware uses common task for
// Tx and Rx data path. Separate Rx task is required because of etcp/Trect implementation issues.
TX_THREAD				stSDKRxThread;
TX_THREAD				stSDKTxThread;


/* SDK Stack Size for mli_RX_Main, mli_TX_Main;
   if you encounter stack overflow, increase this stack size.
*/   
static UINT8 			SDKRxstack[2048];
static UINT8 			SDKTxstack[2048];


// place holder for storing Tx Packets and corresponding return addresses
sdkretbuff_t	mbufs[SDK_MAX_TX_Q];

#define RAW_SOCKET_DATA 			0x07
#define RAW_DATA                    0x08

Queue_t mli_TxFreeQueue;
Queue_t mli_TxGlobalQueue;       
Queue_t mli_TxWlanQueue;       


// element type
typedef struct mli_tx_pkt_q_element mli_tx_pkt_q_element_t;
struct mli_tx_pkt_q_element {
    mli_tx_pkt_q_element_t       *next;          // for queuing
    void*  ptr;
};
// array of tx elements
mli_tx_pkt_q_element_t tx_e[SDK_MAX_TX_Q];




// Custom command handler
MLI_CUSTOM_CMD_T mli_commandHandlerInterface(const uint16 cmd, uint8 *i_pRecCmdBuf, uint8 *o_pRetCmdBuf) 
{
	int i;
	MLI_CUSTOM_CMD_T result = MLI_CUSTOM_CMD_HANDLED;

	//Note:
	//----
	//This is the default implementation for custom commands.
	//In this implementation, it traverses through all custom application
	//command handlers one by one.
	//This code can be modified but it should return proper
	//error codes similar to this implementation.
	
	// check if any custom handler is installed
	if(numberCustomCommandHandlers > 0)
	{
		for(i=0; i<numberCustomCommandHandlers; i++)
		{

			// if so, call custom handlers one by one untill one of them handles the command
			result = cmdHandler[i](cmd, i_pRecCmdBuf, o_pRetCmdBuf);
			if( MLI_CUSTOM_CMD_HANDLED == result)
			{
				// the command was handled, simply return
				return MLI_CUSTOM_CMD_HANDLED;
			}
			else if (MLI_CUSTOM_CMD_SEND_RESPONSE== result)
			{
				// send command response here
				cb_HandleCmdDone(1); 
				return MLI_CUSTOM_CMD_HANDLED;
			}
		}
	}
	return MLI_CUSTOM_CMD_NOT_HANDLED;
}
#ifdef SDK_FEATURE_XX
MLI_CUSTOM_RESP_T mli_responseHandlerInterface(const uint16 cmd, uint8 *io_pRetCmdBuf) 
{
	int i;
	MLI_CUSTOM_RESP_T result = MLI_CUSTOM_RESP_HANDLED;

	//Note:
	//----
	//This is the default implementation for custom commands.
	//In this implementation, it traverses through all custom application
	//command handlers one by one.
	//This code can be modified but it should return proper
	//error codes similar to this implementation.
	
	// check if any custom handler is installed
	if(numberCustomResponseHandlers > 0)
	{
		for(i=0; i<numberCustomResponseHandlers; i++)
		{

			// if so, call custom handlers one by one untill one of them handles the command
			result = respHandler[i](cmd, io_pRetCmdBuf);
			if( MLI_CUSTOM_RESP_HANDLED == result)
			{
				// the command was handled, simply return
				return MLI_CUSTOM_RESP_HANDLED;
			}
		}
	}
	return MLI_CUSTOM_RESP_NOT_HANDLED;
}

MLI_CUSTOM_EVT_T mli_EventHandlerInterface(void * io_pEvtBuf) 
{
	int i;
	MLI_CUSTOM_EVT_T result = MLI_CUSTOM_EVT_HANDLED;

	//Note:
	//----
	//This is the default implementation for custom commands.
	//In this implementation, it traverses through all custom application
	//command handlers one by one.
	//This code can be modified but it should return proper
	//error codes similar to this implementation.
	
	// check if any custom handler is installed
	if(numberCustomEventHandlers > 0)
	{
		for(i=0; i<numberCustomEventHandlers; i++)
		{

			// if so, call custom handlers one by one untill one of them handles the command
			result = evtHandler[i](io_pEvtBuf);
			if( MLI_CUSTOM_EVT_HANDLED == result)
			{
				// the command was handled, simply return
				return MLI_CUSTOM_EVT_HANDLED;
			}
		}
	}
	return MLI_CUSTOM_EVT_NOT_HANDLED;
}
#endif

// SDK Rx data handler task
void mli_RX_Main(ULONG data)  
{
	UINT32	 Events;

	
	for (;;) 
	{
		// wait for rx data event to occur
		Events = os_EventWait(sysinfo_SDK_RX_EVENT,
								SDK_RX_EVENT_TRIGGERS,
								os_EVENT_WAITMODE_CLR_OR);
		// make sure it is mac rx data event
		if(Events & SDK_MAC_ISR_DATA_RCV) 
		{
			// process rx packet
			wlan_cf_rx_pkts();
		}

	}
}


// SDK Tx data handler task
// this is a tx data dispatcher task
// host interface drivers puts tx packet in the queue and invokes this task
// similarly when a packet arrives from a custom module, this task gets invoked
void mli_TX_Main(ULONG data)  
{
	UINT32	 Events;
	int i = 0;
	wcb_t *pkt;
	MLI_CUSTOM_DATA_T retPkt;
	mli_tx_pkt_q_element_t *tmpelement;
	
	for (;;) 
	{
		// wait for tx data event to occur
		Events = os_EventWait(sysinfo_SDK_TX_EVENT,
								SDK_TX_EVENT_TRIGGERS,
								os_EVENT_WAITMODE_CLR_OR);
		// make sure it is mac tx data event
		if(Events & SDK_TX_PKT) 
		{

// Tx data processing task will call this function on getting an even for Tx data Packet 
// This function should do the following:
// 1. getPacketfromTxQueue--- if return value is NULL, exit and return NULL
// 2. call txDataHandler if installed
// 3. check return value of txDataHandler
// 4. 	MLI_CUSTOM_DATA_NOT_MY_PKT---> return the packet to caller
//		MLI_CUSTOM_DATA_PKT_NULL-----> go back to step number 1
//		pkt----------------------> return new packet to caller

			while(1)
			{
				// get Packet from Tx Data Global Queue

				tmpelement = getq(&mli_TxGlobalQueue); 
				// if there is nothing, return NULL
				if(NULL == tmpelement)
				{
					pkt = NULL;
					break;
				}
				
				pkt = (wcb_t *)tmpelement->ptr;
			    putq(&mli_TxFreeQueue, tmpelement);

				// Traverse Through Tx Data Handlers
				for(i=0;i<numberTxDataHandlers;i++)
				{
					retPkt = txDataHandler[i](pkt);
					if(MLI_CUSTOM_DATA_NOT_MY_PKT == retPkt)
					{
						// go to the next Tx Data Handler
						continue;
					}
					else if(MLI_CUSTOM_DATA_PKT_NULL == retPkt)
					{
						// Packet was consumed, go get next packet from Tx Data Queue
						pkt = NULL;
						break;
					}
					else
					{
						// packet was modified, or we have a new packet
						pkt = (wcb_t *)retPkt;
					}
				}
				// After traversing, if we still have a valid packet, pass it to WLAN DRIVER
				if(NULL != pkt)
				{

					break;
				}
			}

			if(NULL != pkt)
			{
				// packet for wlan driver!

				// get element from free queue
				tmpelement = getq(&mli_TxFreeQueue);
				tmpelement->ptr = (void *)pkt;

				// put it in wlan driver queue
			    putq(&mli_TxWlanQueue, tmpelement);

				// and give trigger to wlan driver
				// notify Tx Task
		    	os_EventTrigger(sysinfo_802_11_TX_Q_EVENT, TX_EV_DATA_PDU_RCVD);
			}
		}
	}
}


// application Interface Init
Status_e  mli_appInterfaceInit(void)
{

	int i;

    // Init rx queue as empty
    memset(&mli_TxGlobalQueue, 0, sizeof(mli_TxGlobalQueue));
    memset(&mli_TxWlanQueue, 0, sizeof(mli_TxWlanQueue));



    // Chain rx elements as a link list
    for (i=0; i < SDK_MAX_TX_Q; i++)
    {
        // last element points to null
        if (i == (SDK_MAX_TX_Q-1))
            tx_e[i].next = 0;
        else
            tx_e[i].next = &tx_e[i+1];
    }

    // Init rx_freeq to contain pointers to all rx elements
    mli_TxFreeQueue.q_head = (qp)&tx_e[0];
    mli_TxFreeQueue.q_tail = (qp)&tx_e[SDK_MAX_TX_Q-1];
    mli_TxFreeQueue.q_len  = SDK_MAX_TX_Q;
    mli_TxFreeQueue.q_max  = SDK_MAX_TX_Q;
    mli_TxFreeQueue.q_min  = SDK_MAX_TX_Q;


	// events for rx and tx data tasks
	os_EventInit((os_Id_t)sysinfo_SDK_RX_EVENT);
	os_EventInit((os_Id_t)sysinfo_SDK_TX_EVENT);
	
	// initialise stack buffer for rx and tx tasks
	for (i = 0; i < sizeof(SDKRxstack); SDKRxstack[i++] = 0xff);
	for (i = 0; i < sizeof(SDKTxstack); SDKTxstack[i++] = 0xff);

	// create rx data task
	if (tx_thread_create(&stSDKRxThread,
			 	"sdkRxTask",
				mli_RX_Main,
				(UINT32)NULL,
				(VOID *)&SDKRxstack[0],
				sizeof(SDKRxstack),
				9,
				9,
				2,
				TX_AUTO_START) != TX_SUCCESS) {
		return FAIL;
	}


	// create tx data task
	if (tx_thread_create(&stSDKTxThread,
			 	"sdkTxTask",
				mli_TX_Main,
				(UINT32)NULL,
				(VOID *)&SDKTxstack[0],
				sizeof(SDKTxstack),
				9,
				9,
				2,
				TX_AUTO_START) != TX_SUCCESS) {
		return FAIL;
	}

	// Note:
	//------
	//if there are more custom Modules or applications, all of them need to be initialised here!
	// call custom application Init by default
	return (customAppInit());
}

// Function to install custom command callback function
Status_e mli_installCustomCmdCBFunc(MLI_CUSTOM_CB_TYPE cbfunc)
{
	// check if we have space for more command handlers!
	if(MLI_MAX_CUSTOM_HANDLERS<=numberCustomCommandHandlers)
	{
		return FAIL;	
	}
	// store the command handler function pointer
	cmdHandler[numberCustomCommandHandlers++] = cbfunc;
	return SUCCESS;
}

#ifdef SDK_FEATURE_XX
// Function to install custom response callback function
Status_e mli_installCustomRespCBFunc(MLI_CUSTOM_RCB_TYPE cbfunc)
{
	// check if we have space for more command handlers!
	if(MLI_MAX_CUSTOM_HANDLERS<=numberCustomResponseHandlers)
	{
		return FAIL;
	}
	// store the command handler function pointer
	respHandler[numberCustomResponseHandlers++] = cbfunc;
	return SUCCESS;
}

Status_e mli_installEventHandler(MLI_CUSTOM_ECB_TYPE cbfunc)
{
	// check if we have space for more command handlers!
	if(MLI_MAX_CUSTOM_HANDLERS<=numberCustomEventHandlers)
	{
		return FAIL;
	}
	// store the command handler function pointer
	evtHandler[numberCustomEventHandlers++] = cbfunc;
	return SUCCESS;
}
#endif
// function to install tick function which is called repeatedly
Status_e mli_installTickFunction(MLI_TICKFUNC tickfunc)
{
	// check if we have space for more tick functions!
	if(MLI_MAX_CUSTOM_HANDLERS<=numberCustomTickfunctions)
	{
		return FAIL;	
	}
	// store the tick function function pointer
	tickfunctions[numberCustomTickfunctions++] = tickfunc;
	return SUCCESS;
}

// function to install Tx Data Handler
Status_e mli_installCustomTxDataHandler(MLI_CUSTOM_DATA_TX_HANDLER_TYPE cbfunc)
{
	// check if we have space for more Tx Data Handlers
	if(MLI_MAX_CUSTOM_HANDLERS<=numberTxDataHandlers)
	{
		return FAIL;	
	}
	// save Tx Data Handler function pointer
	txDataHandler[numberTxDataHandlers++] = cbfunc;
	return SUCCESS;
}

// function to install Rx data Handler
Status_e mli_installCustomRxDataHandler(MLI_CUSTOM_DATA_RX_HANDLER_TYPE cbfunc)
{
	// check if we have space for more Rx Data Handlers
	if(MLI_MAX_CUSTOM_HANDLERS<=numberRxDataHandlers)
	{
		return FAIL;	
	}
	// save Rx Data Handler function pointer
	rxDataHandler[numberRxDataHandlers++] = cbfunc;
	return SUCCESS;
}



// store the tx data packet pointer in a queue
void mli_TxDataEnqueue(wcb_t *io_pPkt, MLI_RET_TX_BUFF retTxBuff)
{
   UINT32 int_sta;
   mli_tx_pkt_q_element_t *tmpelement;
   int bindex;


	int_sta = os_if_save_EnterCriticalSection();

	// find a free spot
	for(bindex=0;bindex<SDK_MAX_TX_Q;bindex++)
	{
		if(0 == mbufs[bindex].pkt)
		{	
			break;
		}
	}
	// got a free spot
	if(bindex < SDK_MAX_TX_Q)
	{
		// at this point wlan got tx packet, we need to remember the return buffer function pointer
		mbufs[bindex].pkt = io_pPkt;
		mbufs[bindex].returnTxBuffer = retTxBuff;
		bindex++;
	}

   	os_if_save_ExitCriticalSection(int_sta);   /* restore caller's int state */
	// ooops! we have no room... drop the pack
	if(bindex >= SDK_MAX_TX_Q)
	{
		// no place to hold the packet, simply return the packet.. can not process!
		retTxBuff(io_pPkt);
		return;
	}
	
	// get element from free queue
	tmpelement = getq(&mli_TxFreeQueue);
	tmpelement->ptr = (void *)io_pPkt;


	// check for data, if it is for wlan, put it in wlan queue

	if (io_pPkt->TxPacketType == RAW_DATA) 
	{
	    putq(&mli_TxWlanQueue, tmpelement);
    	os_EventTrigger(sysinfo_802_11_TX_Q_EVENT, TX_EV_DATA_PDU_RCVD);
		
	}
	else
	{
		// otherwise put it in global queue
    	putq(&mli_TxGlobalQueue, tmpelement);
		// notify Tx Dispatcher task
	    os_EventTrigger(sysinfo_SDK_TX_EVENT, SDK_TX_PKT);			     	
	}
	return;

		// we need to check if error.. queue full etc.
#if 0
{
		// error! drop the Tx Packet
		retTxBuff(io_pPkt);
		return;
}

#endif
}

wcb_t *mli_getTxPktforWlan()
{
	wcb_t *pkt;
	mli_tx_pkt_q_element_t *tmpelement;
	// simply get the packet from the wlan driver tx packet queue

	tmpelement = getq(&mli_TxWlanQueue); 
	// if there is nothing, return NULL
	if(NULL == tmpelement)
	{
		pkt = NULL;
	}
	else
	{
		pkt = (wcb_t *)tmpelement->ptr;
	    putq(&mli_TxFreeQueue, tmpelement);
	}
	os_EventTrigger(sysinfo_SDK_TX_EVENT, SDK_TX_PKT);			     	

	return pkt;
}

BOOLEAN mli_peekTxPktforWlan(void)
{
    if (peekq_head(&mli_TxWlanQueue) != NULL) {
       return TRUE;  //queue not empty
    }
    return FALSE;
}

// rx packet return function
void mli_returnRxPacket(RxPD_t *io_pPkt, const MLI_CUSTOM_RXACTION_T action)
{
	if(MLI_CUSTOM_RX_PACK_DISCARD == action)
	{
		// return Packet to Host Interface Driver to free the buffer
		mli_returnRxBuffToHost(); 
	}
	else
	{
		// return packet to Host Interface Driver to be sent to the Host
		io_pPkt->RxPacketType = RX_PKT_TYPE_SEND_TO_HOST;
		mli_sendRxBuffToHost(io_pPkt); 

	}
}

// Rx data packet handler.. traverse thru installed rx data handlers
RxPD_t *mli_PreProcessRxPacket(RxPD_t *io_pPkt)
{
	int i = 0;
	MLI_CUSTOM_DATA_T retPkt;
	RxPD_t *pkt;

	pkt = io_pPkt;

	// Traverse Through the Rx Packet Handlers
	for(i=0;i<numberRxDataHandlers;i++)
	{
		retPkt = rxDataHandler[i](pkt);
		if(MLI_CUSTOM_DATA_NOT_MY_PKT == retPkt)
		{
			// go to the next Rx Packet Handler
			continue;
		}
		else if(MLI_CUSTOM_DATA_PKT_NULL == retPkt)
		{
			// Packet was consumed, return!
			return NULL;
		}
		else
		{
			// The Rx Data Handler has returned modified or new packet... keep traversing
			pkt = (RxPD_t *)retPkt;
		}
	}
	return pkt;
}

/* traverse through installed tx done handlers. one of them should free the buffer */
void mli_HandleTxDone(uint32 wcbPtr, uint32 localWcb)
{
	int i;
	UINT32 int_sta;


	for(i=0;i<SDK_MAX_TX_Q;i++)
	{
		if(wcbPtr == ((uint32)mbufs[i].pkt))
		{
			mbufs[i].returnTxBuffer(mbufs[i].pkt);
			int_sta = os_if_save_EnterCriticalSection();
			mbufs[i].pkt = 0;
			mbufs[i].returnTxBuffer = 0;
   			os_if_save_ExitCriticalSection(int_sta);   /* restore caller's int state */
			break;
		
		}
	}
}

// Reset all Tx data buffers
void mli_ResetTxData()
{
    int i;
	UINT32 int_sta;

	for(i=0; i<SDK_MAX_TX_Q; i++)
    {
        if( mbufs[i].pkt )
        {
			mbufs[i].returnTxBuffer(mbufs[i].pkt);
			int_sta = os_if_save_EnterCriticalSection();
			mbufs[i].pkt = 0;
			mbufs[i].returnTxBuffer = 0;
   			os_if_save_ExitCriticalSection(int_sta);   /* restore caller's int state */
        }
    }

	int_sta = os_if_save_EnterCriticalSection();
    // Init rx queue as empty
    memset(&mli_TxGlobalQueue, 0, sizeof(mli_TxGlobalQueue));
    memset(&mli_TxWlanQueue, 0, sizeof(mli_TxWlanQueue));

    // Chain rx elements as a link list
    for (i=0; i < SDK_MAX_TX_Q; i++)
    {
        // last element points to null
        if (i == (SDK_MAX_TX_Q-1))
            tx_e[i].next = 0;
        else
            tx_e[i].next = &tx_e[i+1];
    }

    // Init rx_freeq to contain pointers to all rx elements
    mli_TxFreeQueue.q_head = (qp)&tx_e[0];
    mli_TxFreeQueue.q_tail = (qp)&tx_e[SDK_MAX_TX_Q-1];
    mli_TxFreeQueue.q_len  = SDK_MAX_TX_Q;
    mli_TxFreeQueue.q_max  = SDK_MAX_TX_Q;
    mli_TxFreeQueue.q_min  = SDK_MAX_TX_Q;
	os_if_save_ExitCriticalSection(int_sta);   /* restore caller's int state */
}

// traverse thru all installed tick functions and call them
void mli_Tick()
{
	int i;

	for(i = 0; i < numberCustomTickfunctions; i++)
	{
        tickfunctions[i]();
	}
	// legacy code? to give a tick to the main rx data handler task
	if (IsRxAllowFromMAC())
    {
        os_EventTrigger(sysinfo_SDK_RX_EVENT, SDK_MAC_ISR_DATA_RCV);	
    }
    
	if (psmgr_IsTxAllowed())
    {
        os_EventTrigger(sysinfo_SDK_TX_EVENT, SDK_TX_PKT);	
        TxMacDataService_SendEvent(TX_EV_DATA_PDU_RCVD);  
    }
}
