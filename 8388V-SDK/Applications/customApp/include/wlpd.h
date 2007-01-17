#ifndef _WLPD_H_
#define _WLPD_H_
/*
 * $Revision: #8 $
 * $Date: 2006/10/03 $
 *
 *                Copyright 2005, Marvell Semiconductor, Inc.
 * This code contains confidential information of Marvell Semiconductor, Inc.
 * No rights are granted herein under any patent, mask work right or copyright
 * of Marvell or any third party.
 * Marvell reserves the right at its sole discretion to request that this code
 * be immediately returned to Marvell. This code is provided "as is".
 * Marvell makes no warranties, expressed, implied or otherwise, regarding its
 * accuracy, completeness or performance.
 */

/**
 * @file    wlpd.h
 * @brief   RX and TX packet descriptor
 * @author (c) Marvell Semiconductor, Inc.
 * Created Fri Feb  4 10:28:53 PST 2005 by cvu@marvell.com
 *
 * REVISION HISTORY:
 *
 */


/** include files **/

 /** @defgroup PacketTypes Tx Rx Data Packet Types
 *  Functions exported by wlpd.h
 *  @{
 */


/***************************************************************************************************/
/**        
*** @brief Enumeration of action to be take for returned Rx Packets.
**/        


#define	RX_PKT_TYPE_SEND_TO_HOST	0xFF
#define	RX_PKT_TYPE_UNKNOWN			0


/**        
*** @brief Receive Packet Descriptor
**/        
typedef PACK_START struct RxPD_t
{
    UINT16 RxStatus;	
    UINT8 RSSI;
    UINT8 RxControl;
    UINT16 RxPacketLength;		//!< Rx Packet Length
    UINT8 RxSQ2;
    UINT8 RxRate;
    UINT32 RxPacketOffset;		//!< Offset to the Rx Data
    struct RxPD_t *NextRxpd;
	uint8  Priority;
	uint8  Reserved[3];
#if defined(EMBEDDED_TCPIP) && !defined(MARVELL_SDK_SUPPORT)
    UINT8	RxPacketType;
    UINT8	Event;
    UINT16	SocketId;
#endif /* EMBEDDED_TCPIP */
#ifdef MARVELL_SDK_SUPPORT	
    UINT8 RxPacketType;	
#ifdef EMBEDDED_TCPIP
    UINT8	Event;
    UINT16	SocketId;
#else
    uint8 Reserved1[3];
#endif
#endif 	// MARVELL_SDK_SUPPORT

}
PACK_END RxPD_t, *PRxPD;

#define RxNF    RxSQ2

#if defined(AUPSD) || defined(ADHOC_PPS)
/* The following fields have been added for Null frame handling 
   in Power Save Mode.
 */
typedef PACK_START struct psm_t {
    UINT8 nullPkt:1;
    UINT8 overRideFwPM:1;
    UINT8 pmVal:1;
    UINT8 lastTxPkt:1;
    UINT8 rsvd:4;
} PACK_END psm_t;
#endif

/**        
*** @brief Transmit Packet Descriptor
**/        
typedef PACK_START struct wcb_t
{

#if defined(buildModes_HIF_USB) || defined(buildModes_HIF_SDIO)
	uint32 Pattern;
#endif
    uint32 TxStatus;
    uint32 TxControl;
    uint32 TxPacketOffset;	//!< Offset to Tx Data
    uint16 TxPacketLength;	//!< Tx Packet Length
    uint16 TxDestAddrHigh;  //!< [bits 4:5]
    uint32 TxDestAddrLow;   //!< [bits 0:3]
	uint8  Priority;
#if defined(AUPSD) || defined(ADHOC_PPS)
    psm_t  psmBits; // These BitFields are for Null Frame Handling and other Power Save requirements.
#else
    uint8  Reserved;
#endif
	uint8 PktDelay_2ms; /* Driver queue delay used in stats and MSDU 
                        **  lifetime expiry calcs; value is represented 
                        **  by 2ms units (ms bit shifted by 1)
                        */
    uint8 Reserved1;
#if defined(EMBEDDED_TCPIP) && !defined(MARVELL_SDK_SUPPORT)
    UINT8   TxPacketType;
    UINT8   Reserved2;
    UINT16  SocketId;
    SINT32  TcpFlags;	
#endif
#ifdef MARVELL_SDK_SUPPORT		// To build SDK use build target "buildsdk"
    UINT8 TxPacketType;		//!< Tx Packet Type
#ifdef EMBEDDED_TCPIP
    UINT8   Reserved2;
    UINT16  SocketId;
    SINT32  TcpFlags;	
#else
    UINT8 Reserved2[7];
#endif
#endif 	// MARVELL_SDK_SUPPORT

}
PACK_END wcb_t;

/*@}*/

#endif /* _WLPD_H_ */

