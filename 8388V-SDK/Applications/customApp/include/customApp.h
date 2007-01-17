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
 * @file customApp.h
 *
 * @brief This file contains custom application Implementation details.
 *
 * 
 *
 * @author (c) Marvell Semiconductor, Inc.
 */

 
#ifndef _CUSTOM_H_
#define _CUSTOM_H_

#include "wltypes.h"
#include "mli.h"

/**        
*** @brief Enumeration of custom command types.
**/        
typedef enum
{
    HOST_CMD_GET_SDK_VERSION	= 0x4000,	/**< 0x4000 */
	HOST_CMD_CUSTOM_END	= 0x40FF 	/**< 0x40FF */
} host_Custom_Cmd_e;


// maximum SDK version length
#define SDK_MAX_VERSION_LENGTH 50

/**        
*** @brief data structure for SDK version.
**/        
typedef PACK_START struct host_sdkVersion_t
{
    UINT32 sdkVersion;
}
PACK_END host_sdkVersion_t;


/** @brief customAppInit: This function intialises custom application module
 *
 * This function intialises custom application module. It calls custom application public
 * interface APIs to register callback functions for custom commands
 *
 */
Status_e  customAppInit (void);

/** @brief customCommandHandler: Custom Command handler
 *
 * This function is installed by customAppInit during system initialisation. 
 * It is called when a custom command is received from the host. This function is responsible
 * for handling custom commands.
 *
 * @param cmd			: command code
 *
 * @param i_pRecCmdBuf 	: pointer to the received command from the host
 *
 * @param o_pRetCmdBuf 	: pointer to command response buffer
 *
 * @return MLI_CUSTOM_CMD_HANDLED	- custom command was handled\n
 * MLI_CUSTOM_CMD_NOT_HANDLED	- custom command was not handled.
 */
MLI_CUSTOM_CMD_T customCommandHandler(const uint16 cmd, uint8 *i_pRecCmdBuf, uint8 *o_pRetCmdBuf);

/** @brief customTxDataHandler: Tx Data Handler
 *
 * This function is installed by customAppInit during system initialisation. 
 * It is called when a data packet is received from the host. 
 * This function is responsible for processing the data packet if it belongs 
 * to the custom Tx Data Handler
 *
 * @param io_pPkt			: data packet of type wcb_t
 *
 * @return MLI_CUSTOM_DATA_NOT_MY_PKT	- if the data packet does not belong to this module
 *	MLI_CUSTOM_DATA_PKT_NULL	- if the data packet has been completely processed
 *                            and no further processing is required.
 *  pkt						- if the processed packet needs further processing.
 * \image html moduledatahandler.gif "Custom Data Handler"
 */
MLI_CUSTOM_DATA_T customTxDataHandler(wcb_t *io_pPkt);

/** @brief customRxDataHandler: Rx Data Handler
 *
 * This function is installed by customAppInit during system initialisation. 
 * It is called when a data packet is received from the MAC. 
 * This function is responsible for processing the data packet if it belongs 
 * to the custom Rx Data Handler
 *
 * @param io_pPkt			: data packet of type RxPD_t
 *
 * @return MLI_CUSTOM_DATA_NOT_MY_PKT	- if the data packet does not belong to this module
 *	MLI_CUSTOM_DATA_PKT_NULL	- if the data packet has been completely processed
 *                            and no further processing is required.
 *  pkt						- if the processed packet needs further processing.
 *  
 */
MLI_CUSTOM_DATA_T customRxDataHandler(RxPD_t *io_pPkt);


#endif /* _CUSTOM_H_ */
