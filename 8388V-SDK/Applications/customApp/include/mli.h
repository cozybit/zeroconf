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
 * @file mli.h
 *
 * @brief This file contains Marvell Application Interface APIs.
 *
 *
 *
 * @author (c) Marvell Semiconductor, Inc.
 */


#ifndef _MLI_INTERFACE_H_
#define _MLI_INTERFACE_H_
#include "wltypes.h"
#include "wlpd.h"
#include "os_if.h"
#include "sysinfo.h"
#include "tx_api.h"


 /** @defgroup mli Marvell Application Interface functions
 *  Functions exported by mli.h
 *  @{
 */

#define MLI_MAX_CUSTOM_HANDLERS 4

/***************************************************************************/
/**
*** @brief Enumeration of custom command handler return types.
**/
typedef enum
{
    MLI_CUSTOM_CMD_HANDLED= 1,		/**< 1 */
    MLI_CUSTOM_CMD_NOT_HANDLED,		/**< 2 */
    MLI_CUSTOM_CMD_SEND_RESPONSE	/**< 3 */
}MLI_CUSTOM_CMD_T;

/***************************************************************************/
/**
*** @brief Enumeration of custom response handler return types.
**/
typedef enum
{
    MLI_CUSTOM_RESP_HANDLED= 1,		/**< 1 */
    MLI_CUSTOM_RESP_NOT_HANDLED,		/**< 2 */
    MLI_CUSTOM_RESP_SEND_RESPONSE	/**< 3 */
}MLI_CUSTOM_RESP_T;

/***************************************************************************/
/**
*** @brief Enumeration of custom event handler return types.
**/
typedef enum
{
    MLI_CUSTOM_EVT_HANDLED= 1,		/**< 1 */
    MLI_CUSTOM_EVT_NOT_HANDLED,		/**< 2 */
    MLI_CUSTOM_EVT_SEND_RESPONSE	/**< 3 */
}MLI_CUSTOM_EVT_T;

/**************************************************************************/
/**
*** @brief Enumeration of action to be take for returned Rx Packets.
**/
typedef enum
{
    MLI_CUSTOM_RX_PACK_DISCARD= 0,		/**< 0 */
    MLI_CUSTOM_RX_PACK_VALID			/**< 1 */
}MLI_CUSTOM_RXACTION_T;



/***********************************************************************/
/**
*** @brief Enumeration of custom data handler return types.
**/
typedef enum
{
    MLI_CUSTOM_DATA_NOT_MY_PKT= 0xFFFFFFFF,		/**< 0xFFFFFFFF */
    MLI_CUSTOM_DATA_PKT_NULL = 0,				/**< 0 */
    MLI_CUSTOM_DATA_NULL_PKT,
	MLI_CUSTOM_DATA_LAST_PKT,
	MLI_CUSTOM_DATA_MORE_PKT
}MLI_CUSTOM_DATA_T;

//separate rx task
#define SDK_MAC_ISR_DATA_RCV		(1<<1)
#define SDK_TX_PKT					(1<<2)

#define SDK_RX_EVENT_TRIGGERS   	SDK_MAC_ISR_DATA_RCV
#define SDK_TX_EVENT_TRIGGERS   	SDK_TX_PKT

#define TX_EV_DATA_PDU_RCVD         (1<<0)

#define CBP_EV_HOST_OPC_RECEIVED    (1U<<1)

#define SDK_MAX_TX_Q	16


typedef  struct sdkretbuff
{
	wcb_t *pkt;
	void  (*returnTxBuffer)(struct wcb_t *pkt);
}
sdkretbuff_t;


/**************************************************************************/
/** @brief Typedef tick function
 *
 *
 * Application can install timer tick function which is called from idle task
 *
 */
typedef void (*MLI_TICKFUNC)(void);
typedef void  (*MLI_RET_TX_BUFF)(struct wcb_t *pkt);
extern Status_e mli_installTickFunction(MLI_TICKFUNC tickfunc);


/**************************************************************************/
/** @brief Typedef customCommandHandler: Custom Command handler
 *
 *
 * This function is installed by customAppInit during system initialisation.
 * It is called when a custom command is received from the host. 
 * This function is responsible for handling custom commands.
 *
 * @param cmd			: command code
 *
 * @param i_pRecCmdBuf 	: pointer to the received command from the host
 *
 * @param o_pRetCmdBuf 	: pointer to command response buffer
 *
 * @return MLI_CUSTOM_CMD_HANDLED   custom command was handled and command
 *                                  response is written to o_pRetCmdBuf
 *	MLI_CUSTOM_CMD_NOT_HANDLED		- custom command was not handled.
 *  MLI_CUSTOM_CMD_SEND_RESPONSE	- custom command was handled, 
 *                                    send command response.
 */
typedef MLI_CUSTOM_CMD_T (*MLI_CUSTOM_CB_TYPE)(const uint16 cmd, 
                                               uint8 *i_pRecCmdBuf,
                                               uint8 *o_pRetCmdBuf);

#ifdef SDK_FEATURE_XX
/**************************************************************************/
/** @brief Typedef customResponseHandler: Custom response handler
 *
 *
 * This function is installed by customAppInit during system initialisation.
 * It is called when a response is available from base firmware
 * This function is responsible for handling custom responses.
 *
 * @param resp			: response code
 *
 * @param io_pRespBuf 	: pointer to the received response
 *
 * @return MLI_CUSTOM_RESP_HANDLED   response was handled
 *	MLI_CUSTOM_RESP_NOT_HANDLED		- response was not handled.
 */
typedef MLI_CUSTOM_RESP_T (*MLI_CUSTOM_RCB_TYPE)(const uint16 cmd, 
                                               uint8 *io_pRespBuf);
/**************************************************************************/
/** @brief Typedef customEventHandler: Custom event handler
 *
 *
 * This function is installed by customAppInit during system initialisation.
 * It is called when a event is available by base firmware. 
 * This function is responsible for handling custom events.
 *
 * @param io_pEvtBuf 	: pointer to the event buffer
 *
 * @return MLI_CUSTOM_EVT_HANDLED   event handled
 *                                  response is written to o_pRetCmdBuf
 *	MLI_CUSTOM_EVT_NOT_HANDLED		- event was not handled, so to be forwarded to host.
 */
typedef MLI_CUSTOM_EVT_T (*MLI_CUSTOM_ECB_TYPE)(void *io_pEvtBuf);

#endif
/****************************************************************************/
/** @brief Typedef Rx Data Handler:
 *
 * This function is installed by customAppInit during system initialisation.
 * It is called when a data packet is received from WLAN interface
 * This function is responsible for processing the data
 * packet if it belongs to the custom Rx Data Handler
 *
 * @param io_pPkt			: data packet of type RxPD_t
 *
 * @return MLI_CUSTOM_DATA_NOT_MY_PKT	- if the data packet does not belong to this module
 *	MLI_CUSTOM_DATA_PKT_NULL	- if the data packet has been completely processed
 *                            and no further processing is required.
 *  pkt						- if the processed packet needs further processing.
 *
 */
typedef MLI_CUSTOM_DATA_T (*MLI_CUSTOM_DATA_RX_HANDLER_TYPE)(RxPD_t *io_pPkt);


/************************************************************************/
/** @brief Typedef Tx Data Handler:
 *
 * This function is installed by customAppInit during system initialisation.
 * It is called when a data packet is received from Host
 . This function is responsible for processing the data
 * packet if it belongs to the custom Tx Data Handler
 *
 * @param io_pPkt			: data packet of type wcb_t
 *
 * @return MLI_CUSTOM_DATA_NOT_MY_PKT	- if the data packet does not belong to this module
 *	MLI_CUSTOM_DATA_PKT_NULL	- if the data packet has been completely processed
 *                            and no further processing is required.
 *  pkt						- if the processed packet needs further processing.
 *
 */
typedef MLI_CUSTOM_DATA_T (*MLI_CUSTOM_DATA_TX_HANDLER_TYPE)(wcb_t *io_pPkt);


/**********************************************************************/
/** @brief mli_installCustomCmdCBFunc: installs custom command handler
 *
 * This function installs custom command handler with the base firmware. 
 *  It is called by the main firmware to handle custom commands received 
 *  from the host.
 *
 * @param cbfunc - pointer to the custom command handler
 *
 * @return FAIL if error, SUCCESS otherwise.
 */
extern Status_e mli_installCustomCmdCBFunc(MLI_CUSTOM_CB_TYPE cbfunc);

#ifdef SDK_FEATURE_XX
/**********************************************************************/
/** @brief mli_installCustomRespCBFunc: installs custom response handler
 *
 * This function installs custom response handler with the base firmware. 
 *  It is called by the main firmware to handle responses received 
 *  from the host.
 *
 * @param cbfunc - pointer to the custom response handler
 *
 * @return FAIL if error, SUCCESS otherwise.
 */
// Function to install custom response callback function
extern Status_e mli_installCustomRespCBFunc(MLI_CUSTOM_RCB_TYPE cbfunc);


/** @brief mli_installEventHandler: installs event handler
 *
 * This function installs event handler with the base firmware. 
 *
 * @param cbfunc - pointer to the custom event handler function
 *
 * @return FAIL if error, SUCCESS otherwise.
 */
// Function to install custom response callback function
extern Status_e mli_installEventHandler(MLI_CUSTOM_ECB_TYPE cbfunc);
#endif


/************************************************************************/
/** @brief mli_installCustomTxDataHandler: installs Tx Data handler
 *
 * This function installs custom Tx Data handler with the base firmware. It is called
 * by the main firmware to handle Tx Data Packets received from the host.
 *
 * @param cbfunc - pointer to the custom Tx handler
 *
 * @return FAIL if error, SUCCESS otherwise.
 */
extern Status_e
mli_installCustomTxDataHandler(MLI_CUSTOM_DATA_TX_HANDLER_TYPE cbfunc);

/***************************************************************************/
/** @brief mli_installCustomRxDataHandler: installs Rx Data handler
 *
 * This function installs custom Rx Data handler with the base firmware. It is called
 * by the main firmware to handle Rx Data Packets received from the Mac.
 *
 * @param cbfunc - pointer to the custom Rx handler
 *
 * @return FAIL if error, SUCCESS otherwise.
 */
extern Status_e 
mli_installCustomRxDataHandler(MLI_CUSTOM_DATA_RX_HANDLER_TYPE cbfunc);

/***************************************************************************/
/** @brief mli_removeCustomRxDataHandler: removes Rx Data handler
 *
 * @param cbfunc - pointer to the custom Rx handler
 *
 * @return FAIL if error, SUCCESS otherwise.
 */
extern Status_e
mli_removeCustomRxDataHandler(MLI_CUSTOM_DATA_RX_HANDLER_TYPE cbfunc);

/*************************************************************************/
/** @brief mli_appInterfaceInit Initialise all custom modules and applications
 *
 * This function is called during system initialisation and is a single entry point of initialisation
 * for all custom modules/applications built under SDK environment.
 * mli_appInterfaceInit internally should call init funtions of all application modules.
 *
 *
 * @return FAIL if error, SUCCESS otherwise.
 */
extern Status_e mli_appInterfaceInit(void);


/**************************************************************************/
/** @brief mli_commandHandlerInterface: custom command handler interface.
 *
 * mli_commandHandlerInterface function is called by the system command handler for host commands
 * if the HOST_CMD_CUSTOM (bit 30 or 0x4000) of the host command is set
 * mli_commandHandlerInterface calls all the installed custom command handlers one by one until
 * one of the custom command hadler handles the command.
 *
 * @param cmd			: command code
 *
 * @param i_pRecCmdBuf 	: pointer to the received command from the host
 *
 * @param o_pRetCmdBuf 	: pointer to command response buffer
 *
 * @return MLI_CUSTOM_CMD_HANDLED	- custom command was handled and command response is written
 *                                to o_pRetCmdBuf
 *	MLI_CUSTOM_CMD_NOT_HANDLED		- custom command was not handled.
 * \image html cmdhandler.gif "Custom Command Handler"
 */
extern MLI_CUSTOM_CMD_T mli_commandHandlerInterface(const uint16 cmd, 
                                                    uint8 *i_pRecCmdBuf, 
                                                    uint8 *o_pRetCmdBuf); 


#ifdef SDK_FEATURE_XX
/**************************************************************************/
/** @brief mli_responseHandlerInterface: custom command response handler interface.
 *
 * mli_responseHandlerInterface function is called by the system response handler for host responses
 * mli_responseHandlerInterface calls all the installed custom response handlers one by one until
 * one of the custom response hadler handles the response.
 *
 * @param cmd			: command code
 *
 * @param io_pRetCmdBuf 	: pointer to command response buffer
 *
 * @return MLI_CUSTOM_CMD_HANDLED	- custom response was handled
 *
 *	MLI_CUSTOM_CMD_NOT_HANDLED		- custom response was not handled.
 * \image html cmdhandler.gif "Custom Command Handler"
 */
extern MLI_CUSTOM_RESP_T mli_responseHandlerInterface(const uint16 cmd, 
                                                    uint8 *io_pRetCmdBuf);

/** @brief mli_EventHandlerInterface: custom command response handler interface.
 *
 * mli_EventHandlerInterface function is called by the system event handler
 * mli_EventHandlerInterface calls all the installed custom event handlers one by one until
 * one of the event hadler handles the response.
 *
 * @param io_evtBuf 	: pointer toevtn buffer
 *
 * @return MLI_CUSTOM_CMD_HANDLED	- event was handled
 *
 *	MLI_CUSTOM_CMD_NOT_HANDLED		- event was not handled.
 * \image html cmdhandler.gif "Custom Command Handler"
 */
extern MLI_CUSTOM_EVT_T mli_EventHandlerInterface(void * io_evtBuf);
#endif
/**************************************************************************/
/** @brief mli_TxDataEnqueue: Puts Tx packet coming from Host in the queue
 *
 * mli_TxDataEnqueue puts the incling Tx data packet pointer in to the Tx Queue.
 * If the queue is full, it calls io_pPkt->returnTxBuffer function to discard the packet.
 * After queuing the Tx Packet in the queue successfully, it sends an event to the Tx data
 * handler task in order to notify it that Tx Data Packet is available.
 * Host Interface driver calls this function after receiving a Tx Data Packet from the Host interface.
 *
 * Application modules (e.g. TCP/IP or IPSEC etc.) can also call this function in order to send the data
 * packet to subsequent Tx Data Packet Handlers or to WLAN driver.
 *
 * @param io_pPkt	: pointer to Tx Packet (wcb_t)
 *
 * @param retTxBuff	: function poiter to be called in order to return the Tx Buffer
 *
 * @return None.
 */
extern void mli_TxDataEnqueue(wcb_t *io_pPkt, MLI_RET_TX_BUFF retTxBuff);

/**************************************************************************/
/** @brief mli_getTxPktforWlan: Executes Tx data dispatcher and returns Tx Data Packet if present
 *
 * mli_getTxPktforWlan is called by the Tx data handler task on receiving Tx data available event.
 *
 * This function gets a Tx packet from Tx packet queue, and traverses through all installed
 * Tx Data Handler functions (application modules) in a loop until one of the following
 * condition is met
 * 1. Tx Data Queue becomes empty - in this case, this function returns NULL
 * 2. A valid Tx Data Packet is found which should be sent to WLAN Driver.
 *
 * If the Tx Packet is not handled by any Tx Data Handler function, it is returned to the
 * Tx Data Handler Task (the calling task) which will send the packet over WLAN interface.
 *
 *
 * @return NULL if there is no Tx Packet, pointer to Tx Packet otherwise.
 * \image html txdatahandler.gif "Tx Data Handler"
 */
extern wcb_t *mli_getTxPktforWlan(void);


/**************************************************************************/
/** @brief mli_PreProcessRxPacket: pre-processing on Rx Packets
 *
 * mli_PreProcessRxPacket is called by WLAN driver when a packet is received from WLAN
 * This function traverses through installed Rx Data Handlers. This function returns NULL if
 * any installed Rx data handler consumes the packet. Otherwise, it returns the original/modified
 * packet.
 *
 * @param io_pPkt - Pointer to Rx Packet
 *
 * @return NULL if the Packet was consumed, pointer to Rx packet otherwise.
 * \image html rxdatahandler.gif "Rx Data Handler"
 */
extern RxPD_t *mli_PreProcessRxPacket(RxPD_t *io_pPkt);


/**************************************************************************/
/** @brief mli_returnRxPacket: Returns Rx Data Packet to Host Interface Driver
 *
 * mli_returnRxPacket is called by application modules (Rx Data Handler) after processing of the
 * Rx Packet in the following conditions:
 * 1. Application module consumed the Rx Packet: In this case, the application module should call
 *    mli_returnRxPacket(pkt, MLI_CUSTOM_RX_PACK_DISCARD);
 * 2. After processing the data, now the Rx Packet should be sent to the host, in this case
 *    the application module should call
 *    mli_returnRxPacket(pkt, MLI_CUSTOM_RX_PACK_VALID);
 *
 * @param io_pPkt -	Pointer to Rx Data Packet to be returned
 *
 * @param action -	MLI_CUSTOM_RX_PACK_DISCARD if the packet should be discarde
 * 					MLI_CUSTOM_RX_PACK_VALID   if the packet should be transfered to the host
 *
 * @return None.
 */
extern void mli_returnRxPacket(RxPD_t *io_pPkt,
                               const MLI_CUSTOM_RXACTION_T action);


/* get hostinterface rx buffer */
extern RxPD_t *mli_getEventBuffPtr(void);


/* Send the event to host*/
extern uint8 mli_sendEvent(uint16 evtBodyLen);


/* This routine is used to free the Rx interface buffer. */
extern void mli_freeEventBuffer(void);

/* get buffer for Tx */
#define mli_getTxBuffer() TxBufMgr_GetTxBuf(0, 0)

/* Free tx buffer */
#define mli_freeTxBuffer(x) msi_wl_FreePktBuf(x)

//global tx packet return policy
/*handle tx done, i.e. release tx buffer */
extern void mli_HandleTxDone(uint32 wcbPtr, uint32 localWcb);


//sdk tick for now in idlemain
extern void mli_Tick(void);

/* check if upload event to host can be done, i.e. if the upload interface
**   buffer is available
**
**  return 1 if buffer available, return 0 if buffer not available.
*/
extern uint8 mli_isHostEventUploadReady(void);


// function to return Rx Buffer to the host driver
extern void mli_returnRxBuffToHost(void);

// function to send the rx buffer to the host through host interface
extern void mli_sendRxBuffToHost(RxPD_t *io_pPkt);

// function to get high priority rx buffer for host interface
extern RxPD_t *mli_getHighPriRxBuffPtr(void);

// function to send high priority packet to host
extern void mli_sendHighPriRxBuffToHost(RxPD_t *io_pPkt);

extern void mli_ResetTxData(void);
extern BOOLEAN mli_peekTxPktforWlan(void);

extern TX_THREAD stSDKRxThread;
extern TX_THREAD stSDKTxThread;

/* Get Command buffer */
uint8* mli_GetCmdReqBuffer();

/* Get Command Result buffer */
uint8* mli_GetCmdResBuffer();

/* Function to get free Tx buffer */
uint8* mli_GetTxFreeBuffer(void);

/* Function to send Tx buffer */
void mli_SendTxBuffer(uint8* buffer);

/**
 * Function to get Raw Tx buffer
 * 
 * @return 
 */
uint8* mli_GetRawTxBuffer(void);

/**
 * Update the IE in the Adhoc Beacon
 * 
 * @param IE
 */
void mli_updateIEinBeacon(uint8 *IE);

/*@}*/

#endif /* _CUSTOM_INTERFACE_H_ */

