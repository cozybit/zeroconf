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


/** @file customApp.c
  * @brief This file contains the handling of custom commands.
  *
  *  Copyright (c) Marvell Semiconductor, Inc.
  */

#include "wltypes.h"
#include "ecl_hostcmd.h"
#include "hostMsgHdr.h"
#include "customApp.h"

/********************************************************
		Local Variables
********************************************************/

/********************************************************
		Global Variables
********************************************************/

/********************************************************
		Local Functions
********************************************************/
#ifdef buildModes_HIF_NO_HOST
#include "eping.h"
extern Status_e  userif_init(void);
extern uint32 	peersta_Init(void);
extern int use_peer_sta_api; 
#endif


extern MLI_CUSTOM_CMD_T eTCPCommandHandler(const uint16 cmd, uint8 *i_pRecCmdBuf, uint8 *o_pRetCmdBuf);
extern char sdkVersionNumber[];
//
MLI_CUSTOM_CMD_T customCommandHandler(const uint16 cmd, uint8 *i_pRecCmdBuf, uint8 *o_pRetCmdBuf)
{
	MLI_CUSTOM_CMD_T result = MLI_CUSTOM_CMD_HANDLED;
    void *verptr;
	uint32 length;


	switch((host_Custom_Cmd_e)cmd)
	{
		case host_CMD_SDK_VER:
			// point to the command - skip header
            verptr = (void *)((uint32)o_pRetCmdBuf + sizeof(host_MsgHdr_t));
			length = (uint32)strlen(sdkVersionNumber);
			if(length > SDK_MAX_VERSION_LENGTH)
			{
				length = SDK_MAX_VERSION_LENGTH;
			}
			// fill the sdk version
            memcpy(verptr, (void *)sdkVersionNumber, length);

			(uint32)verptr = (uint32)verptr + length;
			// null termination
			*(uint8 *)verptr = '\0';
			// set command result
            ((host_MsgHdr_t *)o_pRetCmdBuf)->Result = host_RESULT_OK;
            ((host_MsgHdr_t *)o_pRetCmdBuf)->Msg = host_RET_SDK_VER;
			// add one more byte at the end to insert null.
            ((host_MsgHdr_t *)o_pRetCmdBuf)->Size = sizeof(host_MsgHdr_t) + length + 1;
			result = MLI_CUSTOM_CMD_SEND_RESPONSE;
			break;

		default:
			// not-supported
			((host_MsgHdr_t *)o_pRetCmdBuf)->Result = host_RESULT_NOT_SUPPORT;
			result = MLI_CUSTOM_CMD_NOT_HANDLED;
			break;
	}
	return result;
}

#ifdef SDK_FEATURE_XX
MLI_CUSTOM_RESP_T customResponseHandler(const uint16 cmd, uint8 *io_pRecCmdBuf)
{
	return MLI_CUSTOM_RESP_NOT_HANDLED;
}

MLI_CUSTOM_EVT_T customEventHandler(void * io_evtBuf)
{
	return MLI_CUSTOM_CMD_NOT_HANDLED;
}
#endif


//
MLI_CUSTOM_DATA_T customTxDataHandler(wcb_t *io_pPkt)
{
	return MLI_CUSTOM_DATA_NOT_MY_PKT;
}



//
MLI_CUSTOM_DATA_T customRxDataHandler(RxPD_t *io_pPkt)
{
#ifdef buildModes_HIF_NO_HOST
	// Call Eping Receive if non Null packet
    if ( (io_pPkt != NULL) && (!use_peer_sta_api)) {
		eping_process_pkt((uint8*)io_pPkt + sizeof(RxPD_t));
	}
#endif
 	return MLI_CUSTOM_DATA_NOT_MY_PKT;
}



//
Status_e  customAppInit (void)
{

#ifdef EMBEDDED_TCPIP
	mli_installCustomCmdCBFunc(eTCPCommandHandler);
#endif
	// Install callback function for custom commands
	mli_installCustomCmdCBFunc(customCommandHandler);

	// Install  Tx Data Handler
	mli_installCustomTxDataHandler(customTxDataHandler);

	// Install  Rx Data Handler
	mli_installCustomRxDataHandler(customRxDataHandler);

#ifdef SDK_FEATURE_XX
	// Install callback function for custom commands
	mli_installCustomRespCBFunc(customResponseHandler);

	// install custom event handler
	mli_installEventHandler(customEventHandler);
#endif

#ifdef buildModes_HIF_NO_HOST
	//Initialize the Eping Application
    userif_init();
	peersta_Init();
#endif
    return SUCCESS;;
}


