/******************** (c) Marvell Semiconductor, Inc., 2001 *******************
 *
 * $Header: //depot/MRVL/EEBU_W8686_V5_DEV/wlan/wlandrv/incl/IEEE_types.h#9 $
 *
 * Purpose:
 *    This file contains definitions relating to messages specified in the
 *    IEEE 802.11 spec.
 *
 * Public Procedures:
 *    None.
 *
 * Notes:
 *    None.
 *
 *****************************************************************************/

#ifndef _IEEE_TYPES_H_
#define _IEEE_TYPES_H_

/*========================================================================== */
/*                               INCLUDE FILES */
/*========================================================================== */
#include "wltypes.h"

/*========================================================================== */
/*                            PUBLIC DEFINITIONS */
/*========================================================================== */

#define IS_BROADCAST(macaddr) ((*(UINT16 *)macaddr == 0xffff) &&  \
                               (*(UINT16 *)((UINT8 *)macaddr+2) == 0xffff) && \
                               (*(UINT16 *)((UINT8 *)macaddr+4) == 0xffff))

#define IS_MULTICAST(macaddr) ((*(UINT8 *)macaddr & 0x01) == 0x01)

#define IS_GROUP(macaddr)  ((*(UINT8*)macaddr & 0x01) == 0x01)

#define LLC_SNAP_SIZE         6

/*--------------------------------------------------------------*/
/* Reason Codes - these codes are used in management message    */
/* frame bodies to indicate why an action is taking place (such */
/* as a disassociation or deauthentication).                    */
/*--------------------------------------------------------------*/
#define IEEEtypes_REASON_RSVD                     0
#define IEEEtypes_REASON_UNSPEC                   1
#define IEEEtypes_REASON_PRIOR_AUTH_INVALID       2
#define IEEEtypes_REASON_DEAUTH_LEAVING           3
#define IEEEtypes_REASON_DISASSOC_INACTIVE        4
#define IEEEtypes_REASON_DISASSOC_AP_BUSY         5
#define IEEEtypes_REASON_CLASS2_NONAUTH           6
#define IEEEtypes_REASON_CLASS3_NONASSOC          7
#define IEEEtypes_REASON_DISASSOC_STA_HASLEFT     8
#define IEEEtypes_REASON_CANT_ASSOC_NONAUTH       9
/***************802.11h Reasons***************/
#define IEEEtypes_REASON_DISASSOC_BAD_POWERCAP    10
#define IEEEtypes_REASON_DISASSOC_BAD_SUPPCHAN    11
/***************WPA Reasons*******************/
#define IEEEtypes_REASON_INVALID_IE               13
#define IEEEtypes_REASON_MIC_FAILURE              14
#define IEEEtypes_REASON_4WAY_HANDSHK_TIMEOUT     15
#define IEEEtypes_REASON_GRP_KEY_UPD_TIMEOUT      16
#define IEEEtypes_REASON_IE_4WAY_DIFF             17
#define IEEEtypes_REASON_INVALID_MCAST_CIPHER     18
#define IEEEtypes_REASON_INVALID_UNICAST_CIPHER   19
#define IEEEtypes_REASON_INVALID_AKMP             20
#define IEEEtypes_REASON_UNSUPT_RSNE_VER          21
#define IEEEtypes_REASON_INVALID_RSNE_CAP         22
#define IEEEtypes_REASON_8021X_AUTH_FAIL          23
/***************802.11e Reasons***************/
#define IEEEtypes_REASON_DISASSOC_UNSPEC_QOS      32
#define IEEEtypes_REASON_DISASSOC_QAP_NO_BNDWDTH  33
#define IEEEtypes_REASON_DISASSOC_FRM_LOSS_BAD_CH 34
#define IEEEtypes_REASON_DISASSOC_QSTA_VIOL_TXOP  35
#define IEEEtypes_REASON_REQ_PEER_LEAVE_QBSS      36
#define IEEEtypes_REASON_REQ_PEER_NO_THANKS       37
#define IEEEtypes_REASON_REQ_PEER_ACM_MISMATCH    38
#define IEEEtypes_REASON_REQ_PEER_TIMEOUT         39
#define IEEEtypes_REASON_PEER_QSTA_NO_SUPP_CIPHER 45
/*********************************************/


/*------------------------------------------------------------*/
/* Status Codes - these codes are used in management message  */
/* frame bodies to indicate the results of an operation (such */
/* as association, reassociation, and authentication).        */
/*------------------------------------------------------------*/
#define IEEEtypes_STATUS_SUCCESS                 0
#define IEEEtypes_STATUS_UNSPEC_FAILURE          1
#define IEEEtypes_STATUS_CAPS_UNSUPPORTED        10
#define IEEEtypes_STATUS_REASSOC_NO_ASSOC        11
#define IEEEtypes_STATUS_ASSOC_DENIED_UNSPEC     12
#define IEEEtypes_STATUS_UNSUPPORTED_AUTHALG     13
#define IEEEtypes_STATUS_RX_AUTH_NOSEQ           14
#define IEEEtypes_STATUS_CHALLENGE_FAIL          15
#define IEEEtypes_STATUS_AUTH_TIMEOUT            16
#define IEEEtypes_STATUS_ASSOC_DENIED_BUSY       17
#define IEEEtypes_STATUS_ASSOC_DENIED_RATES      18

/* */
/* 802.11b additions */
/* */
#define IEEEtypes_STATUS_ASSOC_DENIED_NOSHORT    19
#define IEEEtypes_STATUS_ASSOC_DENIED_NOPBCC     20
#define IEEEtypes_STATUS_ASSOC_DENIED_NOAGILITY  21

/* */
/* 802.11h additions */
/* */
#define IEEEtypes_STATUS_ASSOC_DENIED_SPECMGMT_REQD  22
#define IEEEtypes_STATUS_ASSOC_DENIED_BAD_POWERCAP   23
#define IEEEtypes_STATUS_ASSOC_DENIED_BAD_SUPPCHAN   24
/* */
/* 802.11g additions */
/* */
#define IEEEtypes_STATUS_ASSOC_DENIED_NOSHORTSLOTTIME 25
#define IEEEtypes_STATUS_ASSOC_DENIED_NODSSSOFDM      26

/* 802.11e additions */
#define IEEEtypes_STATUS_UNSPEC_QOS_FAILURE               32
#define IEEEtypes_STATUS_ASSOC_DENIED_QAP_INSUFF_BNDWDTH  33
#define IEEEtypes_STATUS_ASSOC_DENIED_EXC_FRM_LOSS_BAD_CH 34
#define IEEEtypes_STATUS_ASSOC_DENIED_STA_NO_QOS_SUPP     35

#define IEEEtypes_STATUS_REQ_DECLINED                     37
#define IEEEtypes_STATUS_REQ_FAIL_INVALID_PARAMS          38
#define IEEEtypes_STATUS_FAIL_TS_AP_THINKS_ITS_SMART_THO  39

#define IEEEtypes_STATUS_FAIL_TS_TRY_LATER_AFTER_TS_DELAY 47
#define IEEEtypes_STATUS_DIRECT_LINK_NOT_ALLOWED          48
#define IEEEtypes_STATUS_DEST_STA_NOT_IN_QBSS             49
#define IEEEtypes_STATUS_DEST_STA_NOT_A_QSTA              50


/*--------------------------------------------*/
/* Various sizes used in IEEE 802.11 messages */
/*--------------------------------------------*/
#define IEEEtypes_ADDRESS_SIZE         6
#define IEEEtypes_BITMAP_SIZE          251
#define IEEEtypes_CHALLENGE_TEXT_SIZE  253
#define IEEEtypes_CHALLENGE_TEXT_LEN   128
#define IEEEtypes_MAX_DATA_RATES       8
#define IEEEtypes_MAX_DATA_BODY_LEN    2312
#define IEEEtypes_MAX_MGMT_BODY_LEN    2312
#define IEEEtypes_SSID_SIZE            32
#define IEEEtypes_TIME_STAMP_SIZE      8
#define IEEEtypes_MAX_CHANNELS         14
#define IEEEtypes_MAX_BSS_DESCRIPTS    16
#define IEEEtypes_MAX_DATA_RATES_G     14
/*---------------------------------------------------------------------*/
/* Define masks used to extract fields from the capability information */
/* structure in a beacon message.                                      */
/*---------------------------------------------------------------------*/
#define IEEEtypes_CAP_INFO_ESS             1
#define IEEEtypes_CAP_INFO_IBSS            2
#define IEEEtypes_CAP_INFO_CF_POLLABLE     4
#define IEEEtypes_CAP_INFO_CF_POLL_RQST    8
#define IEEEtypes_CAP_INFO_PRIVACY         16
#define IEEEtypes_CAP_INFO_SHORT_PREAMB    32
#define IEEEtypes_CAP_INFO_PBCC            64
#define IEEEtypes_CAP_INFO_CHANGE_AGILITY  128
#define IEEEtypes_CAP_INFO_SHORT_SLOT_TIME  0x0400
#define IEEEtypes_CAP_INFO_DSSS_OFDM        0x2000

/*---------------------------*/
/* Miscellaneous definitions */
/*---------------------------*/
#define IEEEtypes_PROTOCOL_VERSION     0

#define IEEEtypes_BASIC_RATE_FLAG 0x80
/* */
/* Used to determine which rates in a list are designated as basic rates */
/* */

#define IEEEtypes_BASIC_RATE_MASK 0x7F
/* */
/* Used to mask off the basic rate flag, if one exists, for given */
/* data rates */
/* */

#define IEEEtypes_RATE_MIN 2
/* */
/* The minimum allowable data rate in units of kb/s */
/* */

#define IEEEtypes_RATE_MAX 127
/* */
/* The maximum allowable data rate in units of kb/s */
/* */

#define IEEEtypes_TIME_UNIT 1024
#define CONVERT_TU_TO_MILLISECOND(x)    (x * 1024 / 1000)
#define CONVERT_TU_TO_MICROSECOND(x)    (x * 1024)


/* */
/* The number of microseconds in 1 time unit, as specified in the */
/* 802.11 spec */
/* */


/*========================================================================== */
/*                          PUBLIC TYPE DEFINITIONS */
/*========================================================================== */

/*---------------------------------------------------------------------------*/
/*                 Enumerated types used in 802.11 messages                  */
/*---------------------------------------------------------------------------*/
typedef enum
{
    IEEE_TYPE_MANAGEMENT = 0,
    IEEE_TYPE_CONTROL,
    IEEE_TYPE_DATA
} IEEEtypes_MsgType_e;
/* */
/* The 3 possible types of 802.11 messages */
/* */

typedef enum
{
    IEEE_MSG_ASSOCIATE_RQST = 0,
    IEEE_MSG_ASSOCIATE_RSP,
    IEEE_MSG_REASSOCIATE_RQST,
    IEEE_MSG_REASSOCIATE_RSP,
    IEEE_MSG_PROBE_RQST,
    IEEE_MSG_PROBE_RSP,
    IEEE_MSG_BEACON = 8,
    IEEE_MSG_ATIM,
    IEEE_MSG_DISASSOCIATE,
    IEEE_MSG_AUTHENTICATE,
    IEEE_MSG_DEAUTHENTICATE,
    IEEE_MSG_ACTION
} IEEEtypes_MgmtSubType_e;
/* */
/* The possible types of management messages */
/* */

typedef enum
{
    PS_POLL = 10,
    RTS,
    CTS,
    ACK,
    CF_END,
    CF_END_CF_ACK
} IEEEtypes_CtlSubType_e;
/* */
/* The possible types of control messages */
/* */

typedef enum
{
    DATA = 0,
    DATA_CF_ACK,
    DATA_CF_POLL,
    DATA_CF_ACK_CF_POLL,
    NULL_DATA,
    CF_ACK,
    CF_POLL,
    CF_ACK_CF_POLL,
	QOS_DATA,
	QOS_NULL = 12
} IEEEtypes_DataSubType_e;
/* */
/* The possible types of data messages */
/* */

typedef enum
{
    SME_CMD_NONE,

    SME_CMD_AUTHENTICATE,
    SME_CMD_ASSOCIATE,
    SME_CMD_REASSOCIATE,

    SME_CMD_DEAUTHENTICATE,
    SME_CMD_DISASSOCIATE,

    SME_CMD_START,
    SME_CMD_JOIN,

    SME_CMD_RESET,
    SME_CMD_SCAN,

} IEEEtypes_SmeCmd_e;
/* */
/* The possible types of commands sent from the SME */
/* */

typedef enum
{
    BSS_INFRASTRUCTURE = 1,
    BSS_INDEPENDENT,
    BSS_ANY
} IEEEtypes_Bss_e;
/* */
/* The possible types of Basic Service Sets */
/* */

typedef enum
{
    SSID = 0,
    SUPPORTED_RATES,
    FH_PARAM_SET,
    DS_PARAM_SET,
    CF_PARAM_SET,
    TIM,
    IBSS_PARAM_SET,
    COUNTRY,
    CHALLENGE_TEXT = 16,
    POWER_CONSTRAINT = 32,
    POWER_CAPABILITY = 33,
    TPC_REQUEST = 34,
    TPC_REPORT = 35,
	SUPPORTED_CHANNELS=36,
	CHANNEL_SWITCH_ANNOUNCEMENT,
	MEASUREMENT_REQUEST,
	MEASUREMENT_REPORT,
	QUIET,
	IBSS_DFS,
    ERP_INFO = 42,
    QOS_CAPABILITY = 46,
    RSN_IEWPA2 = 48,
    EXT_SUPPORTED_RATES = 50,

    VENDOR_SPECIFIC_221 = 221,
    RSN_IE = 221,
} IEEEtypes_ElementId_e;
/* */
/* Variable length mandatory fields or optional frame body components */
/* within management messages are called information elements; these */
/* elements all have associated with them an Element ID. IDs 7 to 15 */
/* are reserved; IDs 17 to 31 are reserved for challenge text; IDs */
/* 32 to 255 are reserved. */
/* */

typedef enum
{
    PWR_MODE_ACTIVE = 1,
    PWR_MODE_PWR_SAVE
} IEEEtypes_PwrMgmtMode_e;
/* */
/* The possible power management modes */
/* */

typedef enum
{
    AUTH_OPEN_SYSTEM = 0,
    AUTH_SHARED_KEY,
    AUTH_NETWORK_EAP = 0x80,
    AUTH_NOT_SUPPORTED
} IEEEtypes_AuthType_e;
/* */
/* The possible types of authentication */
/* */

typedef enum
{
    SCAN_RESULT_SUCCESS,
    SCAN_RESULT_INVALID_PARAMETERS
} IEEEtypes_ScanResult_e;
/* */
/* The possible responses to a request to scan */
/* */

typedef enum
{
    JOIN_RESULT_SUCCESS,
    JOIN_RESULT_INVALID_PARAMETERS,
    JOIN_RESULT_TIMEOUT
} IEEEtypes_JoinResult_e;
/* */
/* The possible responses to a request to join a BSS */
/* */

typedef enum
{
    AUTH_RESULT_SUCCESS,
    AUTH_RESULT_INTERNAL_ERROR,
    AUTH_RESULT_TIMEOUT,
    AUTH_RESULT_TOO_MANY_SIMULTANEOUS_RQSTS,
    AUTH_RESULT_REFUSED
} IEEEtypes_AuthResult_e;
/* */
/* The possible results to a request to authenticate */
/* */

typedef enum
{
    DEAUTH_RESULT_SUCCESS,
    DEAUTH_RESULT_INVALID_PARAMETERS,
    DEAUTH_RESULT_TOO_MANY_SIMULTANEOUS_RQSTS
} IEEEtypes_DeauthResult_e;
/* */
/* The possible results to a request to deauthenticate */
/* */

typedef enum
{
    ASSOC_RESULT_SUCCESS,
    ASSOC_RESULT_INTERNAL_ERROR,
    ASSOC_RESULT_TIMEOUT,
    ASSOC_RESULT_REFUSED,
    ASSOC_RESULT_AUTH_REFUSED,

} IEEEtypes_AssocResult_e;
/* */
/* The possible results to a request to associate */
/* */

typedef enum
{
    DISASSOC_RESULT_SUCCESS,
    DISASSOC_RESULT_INVALID_PARAMETERS,
    DISASSOC_RESULT_TIMEOUT,
    DISASSOC_RESULT_REFUSED
} IEEEtypes_DisassocResult_e;
/* */
/* The possible results to a request to disassociate */
/* */

typedef enum
{
    PWR_MGMT_RESULT_SUCCESS,
    PWR_MGMT_RESULT_INVALID_PARAMETERS,
    PWR_MGMT_RESULT_NOT_SUPPORTED
} IEEEtypes_PwrMgmtResult_e;
/* */
/* The possible results to a request to change the power management mode */
/* */

typedef enum
{
    RESET_RESULT_SUCCESS
} IEEEtypes_ResetResult_e;
/* */
/* The possible results to a request to reset */
/* */

typedef enum
{
    START_RESULT_SUCCESS,
    START_RESULT_INVALID_PARAMETERS,
    START_RESULT_BSS_ALREADY_STARTED_OR_JOINED
} IEEEtypes_StartResult_e;
/* */
/* The possible results to a request to start */
/* */

typedef enum 
{
    TPCADAPT_RESULT_SUCCESS,
    TPCADAPT_RESULT_INVALID_PARAMETERS,
    TPCADAPT_RESULT_UNSPECIFIED_FAILURE
} IEEEtypes_TPCAdaptResult_e;

typedef enum
{
    STATE_IDLE,
    STATE_SCANNING,
    STATE_JOINING,

    STATE_ASSOCIATING,
    STATE_ASSOCIATED,
    STATE_ROAMING,

    STATE_IBSS_ACTIVE,
    STATE_BSS_ACTIVE,
} IEEEtypes_MacMgmtStates_e;

/* */
/* The possible states the MAC Management Service Task can be in */
/* */


/*---------------------------------------------------------------------------*/
/*           Types Used In IEEE 802.11 MAC Message Data Structures           */
/*---------------------------------------------------------------------------*/
typedef UINT8 IEEEtypes_Len_t;
/* */
/* Length type */
/* */

typedef UINT8 IEEEtypes_Addr_t;
/* */
/* Address type */
/* */

typedef IEEEtypes_Addr_t IEEEtypes_MacAddr_t[IEEEtypes_ADDRESS_SIZE];
/* */
/* MAC address type */
/* */

typedef UINT8 IEEEtypes_DataRate_t;
/* */
/* Type used to specify the supported data rates */
/* */

typedef UINT8 IEEEtypes_SsId_t[IEEEtypes_SSID_SIZE];
/* */
/* SS ID type */
/* */

/*---------------------------------------------------------------------------*/
/*                 IEEE 802.11 MAC Message Data Structures                   */
/*                                                                           */
/* Each IEEE 802.11 MAC message includes a MAC header, a frame body (which   */
/* can be empty), and a frame check sequence field. This section gives the   */
/* structures that used for the MAC message headers and frame bodies that    */
/* can exist in the three types of MAC messages - 1) Control messages,       */
/* 2) Data messages, and 3) Management messages.                             */
/*---------------------------------------------------------------------------*/
typedef PACK_START struct IEEEtypes_FrameCtl_t
{
    UINT16 ProtocolVersion:2;
    UINT16 Type:2;
    UINT16 Subtype:4;
    UINT16 ToDs:1;
    UINT16 FromDs:1;
    UINT16 MoreFrag:1;
    UINT16 Retry:1;
    UINT16 PwrMgmt:1;
    UINT16 MoreData:1;
    UINT16 Wep:1;
    UINT16 Order:1;
}
PACK_END IEEEtypes_FrameCtl_t;
/* */
/* The frame control field in the header of a MAC message */
/* */

typedef PACK_START struct IEEEtypes_GenHdr_t
{
    UINT16 FrmBodyLen;
    IEEEtypes_FrameCtl_t FrmCtl;
    UINT16 DurationId;
    IEEEtypes_MacAddr_t Addr1;
    IEEEtypes_MacAddr_t Addr2;
    IEEEtypes_MacAddr_t Addr3;
    UINT16 SeqCtl;
    IEEEtypes_MacAddr_t Addr4;
}
PACK_END IEEEtypes_GenHdr_t;
/* */
/* The general header for MAC messages */
/* */

typedef PACK_START struct IEEEtypes_MgmtHdr_t
{
    UINT16 FrmBodyLen;
    IEEEtypes_FrameCtl_t FrmCtl;
    UINT16 Duration;
    IEEEtypes_MacAddr_t DestAddr;
    IEEEtypes_MacAddr_t SrcAddr;
    IEEEtypes_MacAddr_t BssId;
    UINT16 SeqCtl;
    IEEEtypes_MacAddr_t Rsrvd;
}
PACK_END IEEEtypes_MgmtHdr_t;
/* */
/* The header for MAC management messages */
/* */

typedef PACK_START struct IEEEtypes_PsPollHdr_t
{
    UINT16 FrmBodyLen;
    IEEEtypes_FrameCtl_t FrmCtl;
    UINT16 Aid;
    IEEEtypes_MacAddr_t BssId;
    IEEEtypes_MacAddr_t TxAddr;
}
PACK_END IEEEtypes_PsPollHdr_t;
/* */
/* The header for power-save poll messages (the only control message */
/* processed by the MAC software) */
/* */

typedef PACK_START struct IEEEtypes_CtlFrame_t
{
    IEEEtypes_PsPollHdr_t Hdr;
    UINT32 FCS;
}
PACK_END IEEEtypes_CtlFrame_t;
/* */
/* The structure for control frames (of which only the power-save */
/* poll is processed by the MAC software) */
/* */

typedef PACK_START struct IEEEtypes_DataFrame_t
{
    IEEEtypes_GenHdr_t Hdr;
    UINT8 FrmBody[IEEEtypes_MAX_DATA_BODY_LEN];
    UINT32 FCS;
}
PACK_END IEEEtypes_DataFrame_t;
/* */
/* The structure for data frames */
/* */


/*-------------------------------------------------*/
/* Management Frame Body Components - Fixed Fields */
/*-------------------------------------------------*/
typedef UINT16 IEEEtypes_AId_t;
/* */
/* Association ID assigned by an AP during the association process */
/* */

typedef UINT16 IEEEtypes_AuthAlg_t;
/* */
/* Number indicating the authentication algorithm used (it can take */
/* on the values given by IEEEtypes_AuthType_e): */
/*    0 = Open system */
/*    1 = Shared key */
/*    All other values reserved */
/* */

typedef UINT16 IEEEtypes_AuthTransSeq_t;
/* */
/* Authentication transaction sequence number that indicates the current */
/* state of progress through a multistep transaction */
/* */

typedef UINT16 IEEEtypes_BcnInterval_t;
/* */
/* Beacon interval that represents the number of time units between */
/* target beacon transmission times */
/* */

typedef UINT8 IEEEtypes_DtimPeriod_t;
/* */
/* Interval that represents the number of time units between DTIMs. */
/* */

typedef PACK_START struct IEEEtypes_CapInfo_t
{
    unsigned Ess:1;
    unsigned Ibss:1;
    unsigned CfPollable:1;
    unsigned CfPollRqst:1;
    unsigned Privacy:1;
    unsigned ShortPreamble:1;
    unsigned Pbcc:1;
    unsigned ChanAgility:1;
    unsigned SpectrumMgmt:1;
    unsigned Rsrvd1:1;
    unsigned ShortSlotTime:1;
    unsigned Rsrvd2:2;
    unsigned DsssOfdm:1;
    unsigned Rsrvd3:2;
}
PACK_END IEEEtypes_CapInfo_t;
/* */
/* Capability information used to indicate requested or advertised */
/* capabilities */
/* */

typedef UINT16 IEEEtypes_ListenInterval_t;
/* */
/* Listen interval to indicate to an AP how often a STA wakes to listen */
/* to beacon management frames */
/* */

typedef UINT16 IEEEtypes_ReasonCode_t;
/* */
/* Reason code to indicate the reason that an unsolicited notification */
/* management frame of type Disassociation or Deauthentication was */
/* generated */
/* */

typedef UINT16 IEEEtypes_StatusCode_t;
/* */
/* Status code used in a response management frame to indicate the */
/* success or failure of a requested operation */
/* */

typedef UINT8 IEEEtypes_TimeStamp_t[IEEEtypes_TIME_STAMP_SIZE];
/* */
/* Time stamp */
/* */


/*-------------------------------------------------------*/
/*       Action Frame Related defines.                   */
/*-------------------------------------------------------*/

typedef PACK_START enum
{
    IEEE_MGMT_ACTION_SPECTRUM_MGMT = 0,
    IEEE_MGMT_ACTION_TSPEC         = 17
} PACK_END IEEEtypes_ActionCategory_e;

typedef PACK_START enum
{
    ACTION_MEASUREMENT_REQUEST = 0,
    ACTION_MEASUREMENT_REPORT,
    ACTION_TPC_REQUEST,
    ACTION_TPC_REPORT,
    ACTION_CHANNEL_SWITCH,
} PACK_END IEEESpectrumMgmt_ActionDetail_e;

/*-------------------------------------------------------*/
/* Management Frame Body Components - Information Fields */
/*-------------------------------------------------------*/
typedef PACK_START struct IEEEtypes_InfoElementHdr_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
}
PACK_END IEEEtypes_InfoElementHdr_t;

typedef PACK_START struct IEEEtypes_IE_Param_t
{
    UINT8 ElementId;
    UINT8 Len;
    UINT8 Data[256];
}
PACK_END IEEEtypes_IE_Param_t;

/*
** SSID element that idicates the identity of an ESS or IBSS
*/
typedef PACK_START struct IEEEtypes_SsIdElement_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
    IEEEtypes_SsId_t SsId;
}
PACK_END IEEEtypes_SsIdElement_t;

/*
** Supported rates element that specifies the rates in the operational
** rate set in the MLME join request and the MLME start request
*/
typedef PACK_START struct IEEEtypes_SuppRatesElement_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
    IEEEtypes_DataRate_t Rates[IEEEtypes_MAX_DATA_RATES];
}
PACK_END IEEEtypes_SuppRatesElement_t;

/*
** FH parameter set that conatins the set of parameters necessary to
** allow sychronization for stations using a frequency hopping PHY
*/
typedef PACK_START struct IEEEtypes_FhParamSet_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
    UINT16 DwellTime;
    UINT8 HopSet;
    UINT8 HopPattern;
    UINT8 HopIndex;
}
PACK_END IEEEtypes_FhParamSet_t;


/*
** DS parameter set that contains information to allow channel number
** identification for stations using a direct sequence spread spectrum PHY
*/
typedef PACK_START struct IEEEtypes_DsParamSet_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
    UINT8 CurrentChan;
}
PACK_END IEEEtypes_DsParamSet_t;

/*
** CF parameter set that contains the set of parameters necessary to
** support the PCF
*/
typedef PACK_START struct IEEEtypes_CfParamSet_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
    UINT8 CfpCnt;
    UINT8 CfpPeriod;
    UINT16 CfpMaxDuration;
    UINT16 CfpDurationRemaining;
}
PACK_END IEEEtypes_CfParamSet_t;


/*
** TIM, which contains: 
** 1) DTIM count - how many beacons (including the current beacon 
**    frame) appear before the next DTIM; a count of 0 indicates the 
**    current TIM is the DTIM 
** 
** 2) DTIM period - indicates the number of beacon intervals between 
**    successive DTIMs 
** 
** 3) Bitmap control - contains the traffic indicator bit associated 
**    with association ID 0 - this is set to 1 for TIM elements with a 
**    a value of 0 in the DTIM count field when one or more broadcast 
**    or multicast frames are buffered at the AP. The remaining bits 
**    of the field form the bitmap offset 
** 
** 4) Partial virtual bitmap - indicates which stations have messages 
**    buffered at the AP, for which the AP is prepared to deliver at 
*    the time the beacon frame is transmitted 
*/
typedef PACK_START struct IEEEtypes_Tim_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
    UINT8 DtimCnt;
    UINT8 DtimPeriod;
    UINT8 BitmapCtl;
    UINT8 PartialVirtualBitmap[IEEEtypes_BITMAP_SIZE];
}
PACK_END IEEEtypes_Tim_t;

/* 
** IBSS parameters necessary to support an IBSS
*/
typedef PACK_START struct IEEEtypes_IbssParamSet_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
    UINT16 AtimWindow;
}
PACK_END IEEEtypes_IbssParamSet_t;


/*
** The challenge text used in authentication exchanges
*/
typedef PACK_START struct IEEEtypes_ChallengeText_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
    UINT8 Text[IEEEtypes_CHALLENGE_TEXT_SIZE];
}
PACK_END IEEEtypes_ChallengeText_t;

/*-------------------------*/
/* Management Frame Bodies */
/*-------------------------*/

/*
** The parameter set relevant to the PHY
*/
typedef PACK_START union IEEEtypes_PhyParamSet_t
{
    IEEEtypes_FhParamSet_t FhParamSet;
    IEEEtypes_DsParamSet_t DsParamSet;
} PACK_END IEEEtypes_PhyParamSet_t;

/*
** Service set parameters - for a BSS supporting, PCF, the
** CF parameter set is used; for an independent BSS, the IBSS
** parameter set is used.
*/
typedef PACK_START union IEEEtypes_SsParamSet_t
{
    IEEEtypes_CfParamSet_t CfParamSet;
    IEEEtypes_IbssParamSet_t IbssParamSet;
} PACK_END IEEEtypes_SsParamSet_t;


/*
** Power Constraint IE - for 802.11h TPC. Specifies a Local 
**  Power Constraint in a channel.
*/
typedef PACK_START struct IEEEtypes_PowerConstraintElement_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
    UINT8 locPwrConstraint;
} 
PACK_END  IEEEtypes_PowerConstraintElement_t;


/* 
** Power Capability IE - for 802.11h TPC. Specifies the  
**   min and max power the station is capable of transmitting with.
*/
typedef PACK_START struct IEEEtypes_PowerCapabilityElement_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
    UINT8 minTxPwr;
    UINT8 maxTxPwr;
}
PACK_END  IEEEtypes_PowerCapabilityElement_t;


/* 
** 802.11h TPC Request IE - used for requesting a peer station
**   to send Tx power and Link Margin. 
*/
typedef PACK_START struct IEEEtypes_TPCRequestElement_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
}
PACK_END IEEEtypes_TPCRequestElement_t; 


typedef PACK_START struct IEEEtypes_TPCReportElement_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
    UINT8 TxPwr;
    UINT8 LinkMargin;
}
PACK_END IEEEtypes_TPCReportElement_t; 
typedef enum
{
    BASIC_TYPE = 0,
    CCA_TYPE,
    RPI_TYPE
} IEEEtypes_DFS_ReportTypes_e;

typedef PACK_START struct IEEEtypes_BasicRequestElement_t
{
	UINT8 ChannelNumber;
	UINT64 StartTime;
	UINT16 Duration;
}
PACK_END IEEEtypes_BasicRequestElement_t;

typedef PACK_START struct IEEEtypes_BasicReportElement_t
{
	UINT8 ChannelNumber;
	UINT64 StartTime;
	UINT16 Duration;
	UINT8  Map;
}
PACK_END IEEEtypes_BasicReportElement_t;

typedef PACK_START struct IEEEtypes_CCARequestElement_t
{
	UINT8 ChannelNumber;
	UINT64 StartTime;
	UINT16 Duration;
}
PACK_END IEEEtypes_CCARequestElement_t;

typedef PACK_START struct IEEEtypes_CCAReportElement_t
{
	UINT8 ChannelNumber;
	UINT64 StartTime;
	UINT16 Duration;
	UINT8  BusyFraction;
}
PACK_END IEEEtypes_CCAReportElement_t;

typedef PACK_START struct IEEEtypes_RPIRequestElement_t
{
	UINT8 ChannelNumber;
	UINT64 StartTime;
	UINT16 Duration;
}
PACK_END IEEEtypes_RPIRequestElement_t;

typedef PACK_START struct IEEEtypes_RPIReportElement_t
{
	UINT8 ChannelNumber;
	UINT64 StartTime;
	UINT16 Duration;
	UINT8 RPI_Density[8];
}
PACK_END IEEEtypes_RPIReportElement_t;

typedef PACK_START struct IEEEtypes_MeasurementRequestElement_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
    UINT8 Token;
    UINT8 RequestMode;
	UINT8 Type;
	PACK_START union
	{
		IEEEtypes_BasicRequestElement_t Basic;
		IEEEtypes_CCARequestElement_t CCA;
		IEEEtypes_RPIRequestElement_t RPI;
	}
	PACK_END Request;
}
PACK_END IEEEtypes_MeasurementRequestElement_t;

typedef PACK_START struct IEEEtypes_MeasurementReportElement_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
    UINT8 Token;
    UINT8 ReportMode;
	UINT8 Type;
	PACK_START union
	{
		IEEEtypes_BasicReportElement_t Basic;
		IEEEtypes_CCAReportElement_t CCA;
		IEEEtypes_RPIReportElement_t RPI;
	}
	PACK_END Report;
}
PACK_END IEEEtypes_MeasurementReportElement_t; 

typedef PACK_START struct IEEEtypes_ExtSuppRatesElement_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
    IEEEtypes_DataRate_t Rates[IEEEtypes_MAX_DATA_RATES];
}
PACK_END IEEEtypes_ExtSuppRatesElement_t;

typedef PACK_START struct IEEEtypes_ERPInfo_t
{
    UINT8 NonERPPresent:1;
    UINT8 UseProtection:1;
    UINT8 BarkerPreamble:1;
    UINT8 reserved:5;
}
PACK_END IEEEtypes_ERPInfo_t;

typedef PACK_START struct IEEEtypes_ERPInfoElement_t
{
    IEEEtypes_ElementId_e ElementId;
    IEEEtypes_Len_t Len;
    IEEEtypes_ERPInfo_t ERPInfo;
}
PACK_END IEEEtypes_ERPInfoElement_t;

typedef PACK_START struct IEEEtypes_RSN_IE_t
{
    UINT8 ElemId;
    UINT8 Len;
    UINT8 OuiType[4];     /*00:50:f2:01 */
    UINT8 Ver[2];
    UINT8 GrpKeyCipher[4];
    UINT8 PwsKeyCnt[2];
    UINT8 PwsKeyCipherList[4];
    UINT8 AuthKeyCnt[2];
    UINT8 AuthKeyList[4];
}
PACK_END IEEEtypes_RSN_IE_t;

typedef PACK_START struct IEEEtypes_RSN_IE_WPA2_t
{
    uint8 ElemId;
    uint8 Len;
    //uint8   OuiType[4];    //00:50:f2:01
    uint8 Ver[2];
    uint8 GrpKeyCipher[4];
    uint8 PwsKeyCnt[2];
    uint8 PwsKeyCipherList[4];
    uint8 AuthKeyCnt[2];
    uint8 AuthKeyList[4];
    uint8 RsnCap[2];
    uint8 PMKIDCnt[2];
    uint8 PMKIDList[16];
}
PACK_END IEEEtypes_RSN_IE_WPA2_t;

#ifdef AP_WPA2
typedef PACK_START struct IEEEtypes_RSN_IE_WPA2MixedMode_t
{
    uint8 ElemId;
    uint8 Len;
    //uint8   OuiType[4];    //00:50:f2:01
    uint8 Ver[2];
    uint8 GrpKeyCipher[4];
    uint8 PwsKeyCnt[2];
    uint8 PwsKeyCipherList[4];
    uint8 PwsKeyCipherList2[4];
    uint8 AuthKeyCnt[2];
    uint8 AuthKeyList[4];
    uint8 RsnCap[2];
    uint8 PMKIDCnt[2];
    uint8 PMKIDList[16];
}
PACK_END IEEEtypes_RSN_IE_WPA2MixedMode_t;
#endif

typedef PACK_START struct IEEEtypes_Bcn_t
{
    IEEEtypes_TimeStamp_t TimeStamp;
    IEEEtypes_BcnInterval_t BcnInterval;
    IEEEtypes_CapInfo_t CapInfo;
    IEEEtypes_SsIdElement_t SsId;
    IEEEtypes_SuppRatesElement_t SuppRates;
    IEEEtypes_PhyParamSet_t PhyParamSet;
    IEEEtypes_SsParamSet_t SsParamSet;
    /* for ap the following structure is used only for size calculation,
       the location and size of the elements can be changed */
    IEEEtypes_RSN_IE_t RsnIE;
    IEEEtypes_RSN_IE_WPA2_t RsnIEWPA2;
    IEEEtypes_Tim_t Tim;
    IEEEtypes_ERPInfoElement_t ERPInfo;
    IEEEtypes_ExtSuppRatesElement_t ExtSuppRates;
}
PACK_END IEEEtypes_Bcn_t;

typedef PACK_START struct IEEEtypes_DisAssoc_t
{
    IEEEtypes_ReasonCode_t ReasonCode;
}
PACK_END IEEEtypes_DisAssoc_t;


typedef PACK_START struct IEEEtypes_AssocRqst_t
{
    IEEEtypes_CapInfo_t CapInfo;
    IEEEtypes_ListenInterval_t ListenInterval;
    IEEEtypes_SsIdElement_t SsId;
    IEEEtypes_SuppRatesElement_t SuppRates;
    IEEEtypes_ExtSuppRatesElement_t ExtSuppRates;
}
PACK_END IEEEtypes_AssocRqst_t;


typedef PACK_START struct IEEEtypes_AssocRsp_t
{
    IEEEtypes_CapInfo_t CapInfo;
    IEEEtypes_StatusCode_t StatusCode;
    IEEEtypes_AId_t AId;
    IEEEtypes_SuppRatesElement_t SuppRates;
    IEEEtypes_ExtSuppRatesElement_t ExtSuppRates;
}
PACK_END IEEEtypes_AssocRsp_t;


typedef PACK_START struct IEEEtypes_ReAssocRqst_t
{
    IEEEtypes_CapInfo_t CapInfo;
    IEEEtypes_ListenInterval_t ListenInterval;
    IEEEtypes_MacAddr_t CurrentApAddr;
    IEEEtypes_SsIdElement_t SsId;
    IEEEtypes_SuppRatesElement_t SuppRates;
    IEEEtypes_ExtSuppRatesElement_t ExtSuppRates;
}
PACK_END IEEEtypes_ReAssocRqst_t;


typedef PACK_START struct IEEEtypes_ReAssocRsp_t
{
    IEEEtypes_CapInfo_t CapInfo;
    IEEEtypes_StatusCode_t StatusCode;
    IEEEtypes_AId_t AId;
    IEEEtypes_SuppRatesElement_t SuppRates;
    IEEEtypes_ExtSuppRatesElement_t ExtSuppRates;
}
PACK_END IEEEtypes_ReAssocRsp_t;


typedef PACK_START struct IEEEtypes_ProbeRqst_t
{
    IEEEtypes_SsIdElement_t SsId;
    IEEEtypes_SuppRatesElement_t SuppRates;
    IEEEtypes_ExtSuppRatesElement_t ExtSuppRates;
}
PACK_END IEEEtypes_ProbeRqst_t;


typedef PACK_START struct IEEEtypes_ProbeRsp_t
{
    IEEEtypes_TimeStamp_t TimeStamp;
    IEEEtypes_BcnInterval_t BcnInterval;
    IEEEtypes_CapInfo_t CapInfo;
    IEEEtypes_SsIdElement_t SsId;
    IEEEtypes_SuppRatesElement_t SuppRates;
    IEEEtypes_PhyParamSet_t PhyParamSet;
    IEEEtypes_SsParamSet_t SsParamSet;
    IEEEtypes_Tim_t Tim;
    IEEEtypes_ERPInfoElement_t ERPInfo;
    IEEEtypes_ExtSuppRatesElement_t ExtSuppRates;
}
PACK_END IEEEtypes_ProbeRsp_t;


typedef PACK_START struct IEEEtypes_Auth_t
{
    IEEEtypes_AuthAlg_t AuthAlg;
    IEEEtypes_AuthTransSeq_t AuthTransSeq;
    IEEEtypes_StatusCode_t StatusCode;
    IEEEtypes_ChallengeText_t ChallengeText;
}
PACK_END IEEEtypes_Auth_t;


typedef PACK_START struct IEEEtypes_Deauth_t
{
    IEEEtypes_ReasonCode_t ReasonCode;
}
PACK_END IEEEtypes_Deauth_t;


typedef PACK_START struct IEEEtypes_Action_t
{
    IEEEtypes_ActionCategory_e  category;
    UINT8 actionDetail;
}
PACK_END IEEEtypes_Action_t;


typedef PACK_START struct IEEEtypes_TPCRequest_t
{
    IEEEtypes_Action_t action;
    UINT8 dialogToken;
    IEEEtypes_TPCRequestElement_t tpcRequest; 
}
PACK_END IEEEtypes_TPCRequest_t;


typedef PACK_START struct IEEEtypes_TPCReport_t
{
    IEEEtypes_Action_t action;
    UINT8 dialogToken;
    IEEEtypes_TPCReportElement_t tpcReport; 
}
PACK_END IEEEtypes_TPCReport_t;

typedef PACK_START struct IEEEtypes_MeasurementReport_t
{
    IEEEtypes_Action_t action;
    UINT8 dialogToken;
    IEEEtypes_MeasurementReportElement_t dfsReport; 
}
PACK_END IEEEtypes_MeasurementReport_t;

typedef PACK_START struct IEEEtypes_MeasurementRequest_t
{
    IEEEtypes_Action_t action;
    UINT8 dialogToken;
    IEEEtypes_MeasurementRequestElement_t dfsRequest; 
}
PACK_END IEEEtypes_MeasurementRequest_t;


/*---------------------------------------------------------------------------*/
/*              IEEE 802.11 MLME SAP Interface Data Structures               */
/*                                                                           */
/* According to IEEE 802.11, services are provided by the MLME to the SME.   */
/* In the current architecture, the services are provided to the SME by the  */
/* MAC Management Service Task. This section describes data structures       */
/* needed for these services.                                                */
/*---------------------------------------------------------------------------*/

/* 
** BSS Description Set
**
**  A description of a BSS, providing the following:
**  BssId:        The ID of the BSS
**  SsId:         The SSID of the BSS
**  BssType:      The type of the BSS (INFRASTRUCTURE or INDEPENDENT)
**  BcnPeriod:    The beacon period (in time units)
**  DtimPeriod:   The DTIM period (in beacon periods)
**  Tstamp:       Timestamp of a received frame from the BSS; this is an 8
**                   byte string from a probe response or beacon
**  StartTs:      The value of a station's timing synchronization function
**                   at the start of reception of the first octet of the
**                   timestamp field of a received frame (probe response or
**                   beacon) from a BSS; this is an 8 byte string
**  PhyParamSet:  The parameter set relevant to the PHY (empty if not
**                   needed by the PHY)
**  SsParamSet:   The service set parameters. These can consist of either
**                   the parameter set for CF periods or for an IBSS.
**  Cap:          The advertised capabilities of the BSS
**  DataRates:    The set of data rates that must be supported by all
**                   stations (the BSS basic rate set)
*/
typedef PACK_START struct IEEEtypes_BssDesc_t
{
    IEEEtypes_MacAddr_t BssId;
    IEEEtypes_SsId_t SsId;
    IEEEtypes_Bss_e BssType;
    IEEEtypes_BcnInterval_t BcnPeriod;
    IEEEtypes_DtimPeriod_t DtimPeriod;
    IEEEtypes_TimeStamp_t Tstamp;
    IEEEtypes_TimeStamp_t StartTs;
    IEEEtypes_PhyParamSet_t PhyParamSet;
    IEEEtypes_SsParamSet_t SsParamSet;
    IEEEtypes_CapInfo_t Cap;
    IEEEtypes_DataRate_t DataRates[IEEEtypes_MAX_DATA_RATES_G];
    /* 
    ** DO NOT ADD ANY FIELDS TO THIS STRUCTURE.  It is used in the middle of
    **   the adhoc join command.  Any changes will shift the binary layout
    **   of the following fields in the command from the driver
    */
}
PACK_END IEEEtypes_BssDesc_t;

typedef enum
{
    RADIO_BG = 0,
    RADIO_A
} RadioType_e;

typedef PACK_START struct
{
    UINT8 passiveScan     : 1;
    UINT8 disableChanFilt : 1;
    UINT8 reserved_2_7    : 6;
} PACK_END ChanScanMode_t;

typedef PACK_START struct
{
    RadioType_e      radioType;
    UINT8            chanNum;
    ChanScanMode_t   scanMode;
    UINT16           minScanTime;
    UINT16           maxScanTime;
} PACK_END channelInfo_t;


/*
** Scan request message from the SME to determine if there are BSSs
** that can be joined
*/
typedef PACK_START struct IEEEtypes_ScanCmd_t
{

    IEEEtypes_Bss_e BssType;
    IEEEtypes_MacAddr_t BssId;
    IEEEtypes_SsId_t SsId;
    channelInfo_t ChanList[IEEEtypes_MAX_CHANNELS];
    IEEEtypes_DataRate_t OpRateSet[IEEEtypes_MAX_DATA_RATES_G];
    UINT16  NumProbes;
    UINT8   snrThreshold;
    UINT8   pm_scan_disabled;
    UINT16  bcSSIDprobReq;
    UINT16  numSSIDforProbReq;
    UINT16  NumProbes_bc;   // number of broadcast probe requests sent
}
PACK_END IEEEtypes_ScanCmd_t;


/*
** Scan confirm message sent from the MLME as a result of a scan request;
**  it reports the results of the scan
*/
typedef PACK_START struct IEEEtypes_ScanCfrm_t
{
    IEEEtypes_ScanResult_e Result;
    UINT8 NumSets;
}
PACK_END IEEEtypes_ScanCfrm_t;


/*
**  IBSS DFS element, Channel Switch Element and quiet element
**   is needed in the join cmd and start cmd by 802.11h DFS
*/
typedef PACK_START struct IEEEtypes_CHANNEL_SWITCH_t
{
    uint8 ElemId;
    uint8 Len;
	uint8 Ch_Sw_Mode;
	uint8 ChNum;
	uint8 ChSwCnt;
}
PACK_END IEEEtypes_CHANNEL_SWITCH_t;

typedef PACK_START struct IEEEtypes_CHANNEL_SWITCH_ACTION_t
{
    IEEEtypes_Action_t action;
	IEEEtypes_CHANNEL_SWITCH_t Channel_Switch_Elem; 
}
PACK_END IEEEtypes_CHANNEL_SWITCH_ACTION_t;

typedef PACK_START struct IEEEtypes_DFS_Map_t
{
    unsigned BSS:1;
    unsigned OFDM_Preamble:1;
    unsigned Unidentified:1;
    unsigned Radar:1;
    unsigned Unmeasured:1;
    unsigned Reserved:3;
}
PACK_END IEEEtypes_DFS_Map_t;

typedef PACK_START struct IEEEtypes_ChannelMap_t
{
    uint8 ChannelNumber;
    IEEEtypes_DFS_Map_t DFS_Map;
}
PACK_END IEEEtypes_ChannelMap_t;

#define MAX_NUMBER_OF_DFS_CHANNELS 25
typedef PACK_START struct IEEEtypes_IBSS_DFS_t
{
    uint8 ElemId;
    uint8 Len;
	IEEEtypes_MacAddr_t DFS_Owner;
	uint8 DFS_Recovery_Interval;

    /* For the channels in .11a */
	IEEEtypes_ChannelMap_t	Channel_Map[MAX_NUMBER_OF_DFS_CHANNELS]; 
}
PACK_END IEEEtypes_IBSS_DFS_t;

typedef PACK_START struct IEEEtypes_QUIET_ELEMENT_t
{
    uint8 ElemId;
    uint8 Len;
	uint8 QuietCnt;
	uint8 QuietPeriod;
	uint16 QuietDuration;
	uint16 QuietOffset;
}
PACK_END IEEEtypes_QUIET_ELEMENT_t;


/*
** Join request message from the SME to establish synchronization with
** a BSS
*/
typedef PACK_START struct IEEEtypes_JoinCmd_t
{
    IEEEtypes_BssDesc_t BssDesc;
#ifdef DFS
    IEEEtypes_QUIET_ELEMENT_t Quiet_Element;
    IEEEtypes_IBSS_DFS_t	  IBSS_DFS_Element;
#endif
    channelInfo_t JoinChannel;
}
PACK_END IEEEtypes_JoinCmd_t;


/* 
** Join confirm message sent from the MLME as a result of a join request;
** it reports the result of the join
*/
typedef PACK_START struct IEEEtypes_JoinCfrm_t
{
    IEEEtypes_JoinResult_e Result;
}
PACK_END IEEEtypes_JoinCfrm_t;


/*
** Authenticate request message sent from the SME to establish
** authentication with a specified peer MAC entity
*/
typedef PACK_START struct IEEEtypes_AuthCmd_t
{
    IEEEtypes_MacAddr_t PeerStaAddr;
    IEEEtypes_AuthType_e AuthType;
    channelInfo_t AuthChannel;
}
PACK_END IEEEtypes_AuthCmd_t;


/*
** Authenticate confirm message sent from the MLME as a result of an
** authenticate request; it reports the result of the authentication
*/
typedef PACK_START struct IEEEtypes_AuthCfrm_t
{
    IEEEtypes_MacAddr_t PeerStaAddr;
    IEEEtypes_AuthType_e AuthType;
    IEEEtypes_AuthResult_e Result;
}
PACK_END IEEEtypes_AuthCfrm_t;


/*
** Authenticate indication message sent from the MLME to report
** authentication with a peer MAC entity that resulted from an
** authentication procedure that was initiated by that MAC entity
*/
typedef PACK_START struct IEEEtypes_AuthInd_t
{
    IEEEtypes_MacAddr_t PeerStaAddr;
    IEEEtypes_AuthType_e AuthType;
}
PACK_END IEEEtypes_AuthInd_t;


/*
** Deauthenticate request message sent from the SME to invalidate
** authentication with a specified peer MAC entity
*/
typedef PACK_START struct IEEEtypes_DeauthCmd_t
{
    IEEEtypes_MacAddr_t PeerStaAddr;
    IEEEtypes_ReasonCode_t Reason;
}
PACK_END IEEEtypes_DeauthCmd_t;


/*
** Deauthenticate confirm message sent from the MLME as a result of a
** deauthenticate request message; it reports the result of the
** deauthentication
*/
typedef PACK_START struct IEEEtypes_DeauthCfrm_t
{
    IEEEtypes_MacAddr_t PeerStaAddr;
    IEEEtypes_DeauthResult_e Result;
}
PACK_END IEEEtypes_DeauthCfrm_t;

/*
** Deauthentication indication message sent from the MLME to report
** invalidation of an authentication with a peer MAC entity; the message
** is generated as a result of an invalidation of the authentication
*/
typedef PACK_START struct IEEEtypes_DeauthInd_t
{
    IEEEtypes_MacAddr_t PeerStaAddr;
    IEEEtypes_ReasonCode_t Reason;
}
PACK_END IEEEtypes_DeauthInd_t;

/* Supported Channels and Channels Tuple structures are used by 802.11H-DFS */
typedef PACK_START struct ChannelsTuple_t
{
    uint8 FirstChannelNumber;
    uint8 NumberOfChannels;
}
PACK_END ChannelsTuple_t;

#define MAX_CHANNEL_TUPLES 5
typedef PACK_START struct IEEEtypes_SupportedChannels_t
{
    uint8 ElemId;
    uint8 Len;
	ChannelsTuple_t ChannelTuple[MAX_CHANNEL_TUPLES];
}
PACK_END IEEEtypes_SupportedChannels_t;


/*
** Association request message sent from the SME to establish an
**   association with an AP
*/
typedef PACK_START struct IEEEtypes_AssocCmd_ext_t
{
	IEEEtypes_MacAddr_t PeerStaAddr;
	IEEEtypes_CapInfo_t CapInfo;
	IEEEtypes_ListenInterval_t ListenInterval;
	IEEEtypes_BcnInterval_t BcnPeriod;
	IEEEtypes_DtimPeriod_t DtimPeriod;
	UINT8 IEBuffer[1];
}
PACK_END IEEEtypes_AssocCmd_ext_t;


/* 
** Internal Association command constructed from the TLV based host
**   command struct (IEEEtypes_AssocCmd_ext_t)
*/
typedef PACK_START struct
{
    IEEEtypes_MacAddr_t PeerStaAddr;
    IEEEtypes_CapInfo_t CapInfo;
    IEEEtypes_ListenInterval_t ListenInterval;
    IEEEtypes_SsId_t SsId;
    IEEEtypes_DataRate_t RateSet[IEEEtypes_MAX_DATA_RATES_G];
    channelInfo_t AssocChannel;

} IEEEtypes_AssocCmd_t;

/*
** Reassociation request message sent from the SME to change association 
**   to a specified new peer MAC entity acting as an AP
*/
typedef IEEEtypes_AssocCmd_t IEEEtypes_ReAssocCmd_t;


/* 
** Association confirm message sent from the MLME as a result of an
**   association request message; it reports the result of the assoication
*/
typedef PACK_START struct IEEEtypes_AssocCfrm_t
{
    IEEEtypes_AssocResult_e Result;
}
PACK_END IEEEtypes_AssocCfrm_t;


/*
** Disassociate request message sent from the SME to establish
** disassociation with an AP
*/
typedef PACK_START struct IEEEtypes_DisassocCmd_t
{
    IEEEtypes_MacAddr_t PeerStaAddr;
    IEEEtypes_ReasonCode_t Reason;
}
PACK_END IEEEtypes_DisassocCmd_t;

/*
** Disassociate confirm message sent from the MLME as a result of a
** disassociate request message; it reports the result of the
** disassociation
*/
typedef PACK_START struct IEEEtypes_DisassocCfrm_t
{
    IEEEtypes_DisassocResult_e Result;
}
PACK_END IEEEtypes_DisassocCfrm_t;

/*
** Disassociate indication message sent from the MLME to report the
** invalidation of an association relationship with a peer MAC entity;
** the message is generated as a result of an invalidation of an
** association relationship
*/
typedef PACK_START struct IEEEtypes_DisassocInd_t
{
    IEEEtypes_MacAddr_t PeerStaAddr;
    IEEEtypes_ReasonCode_t Reason;
}
PACK_END IEEEtypes_DisassocInd_t;


/*
** Start request message sent from the SME to start a new BSS; the BSS
**   may be either an infrastructure BSS (with the MAC entity acting as the
**   AP) or an independent BSS (with the MAC entity acting as the first
**   station in the IBSS)
*/
typedef PACK_START struct IEEEtypes_StartCmd_t
{
    IEEEtypes_SsId_t SsId;
    IEEEtypes_Bss_e BssType;
    IEEEtypes_BcnInterval_t BcnPeriod;
    IEEEtypes_DtimPeriod_t DtimPeriod;
    IEEEtypes_SsParamSet_t SsParamSet;
    IEEEtypes_PhyParamSet_t PhyParamSet;
    UINT16 Reserved;
    IEEEtypes_CapInfo_t CapInfo;
    IEEEtypes_DataRate_t OpRateSet[IEEEtypes_MAX_DATA_RATES_G];
#ifdef DFS
   IEEEtypes_QUIET_ELEMENT_t Quiet_Element;
   IEEEtypes_IBSS_DFS_t	IBSS_DFS_Element;
#endif
}
PACK_END IEEEtypes_StartCmd_t;

/*
** Start confirm message sent from the MLME as a result of a start request
**   message; it reports the results of the BSS creation procedure
*/
typedef PACK_START struct IEEEtypes_StartCfrm_t
{
    IEEEtypes_StartResult_e Result;
}
PACK_END IEEEtypes_StartCfrm_t;


typedef PACK_START struct IEEEtypes_TPCAdaptCmd_t
{
    IEEEtypes_MacAddr_t    PeerStaAddr;
}
PACK_END IEEEtypes_TPCAdaptCmd_t;


typedef PACK_START struct IEEEtypes_TPCAdaptCfrm_t
{
    IEEEtypes_TPCAdaptResult_e Result;
}
PACK_END IEEEtypes_TPCAdaptCfrm_t;


typedef PACK_START struct IEEEtypes_Frame_t
{
    IEEEtypes_GenHdr_t Hdr;
    UINT8 Body[8];
}
PACK_END IEEEtypes_Frame_t;


typedef PACK_START struct ether_hdr_t
{
    IEEEtypes_MacAddr_t da;
    IEEEtypes_MacAddr_t sa;
    UINT16 type;
}
PACK_END ether_hdr_t;


typedef PACK_START struct IEEEtypes_8023_Frame_t
{
    ether_hdr_t Hdr;
    UINT8 Body[1600 - 14];
}
PACK_END IEEEtypes_8023_Frame_t;


typedef enum
{
    IEEEtypes_RSN_AUTH_KEY_SUITE_RSVD = 0,
    IEEEtypes_RSN_AUTH_KEY_SUITE_8021X,
    IEEEtypes_RSN_AUTH_KEY_SUITE_PSK
} IEEEtypes_RSN_Auth_Key_Suite;

/* Cipher Suite Selector */
typedef enum
{
    IEEEtypes_RSN_CIPHER_SUITE_NONE = 0,
    IEEEtypes_RSN_CIPHER_SUITE_WEP40,
    IEEEtypes_RSN_CIPHER_SUITE_TKIP,
    IEEEtypes_RSN_CIPHER_SUITE_WRAP,
    IEEEtypes_RSN_CIPHER_SUITE_CCMP,
    IEEEtypes_RSN_CIPHER_SUITE_WEP104
} IEEEtypes_RSN_Cipher_Suite;

typedef PACK_START struct IEEEtypes_BGScan_Info_t
{
    UINT8  Enabled;          /* Is BG option enabled (1 enabled, 0 disabled)*/
    UINT8  BssType;          /* Independent, Infra, Any */
    UINT8  ChannelsPerScan;  /* Num channels to scan each scan session */
    UINT8  DiscardWhenFull;  /* Which result to discard when list is full */
    UINT16 Reserved;         /* Reserved */
    UINT32 ScanInterval;     /* Interval between consecutive scan sessions */
    UINT32 StoreConditions;  /* Bitmap of conditions to store result */
    UINT32 ReportConditions; /* Bitmap conditions to trigger report to host */
    UINT16 MaxScanResults;   /* Max Number of scan results to store */
    UINT8  TlvData[1];       /* Start of TLV Data. Need parsing */
}
PACK_END IEEEtypes_BGScan_Info_t;

#endif /* _IEEE_TYPES_H_ */
