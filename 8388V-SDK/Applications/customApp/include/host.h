/******************** (c) Marvell Semiconductor, Inc., 2001 *******************
*
* $Header: //depot/MRVL/EEBU_W8686_V5_DEV/wlan/prod/ecl/STA/Incl/host.h#22 $
*
* Purpose:
*    This file contains definitions relating to commands sent from the
*    host and the responses that are returned to the host.
*
* Public Procedures:
*    None.
*
* Notes:
*    None.
*
*****************************************************************************/

#ifndef _HOST_H_
#define _HOST_H_

#include "wltypes.h"
#include "IEEE_types.h"
#include "ecl_hostcmd.h"
#include "tlv.h"
#include "wl_mib.h"
#include "wl_msgdefs.h"

#include "mac_events.h"

#include "wmm.h"

#include "hostMsgHdr.h"

/*Define data structure for CMD_CODE_DNLD*/
typedef PACK_START struct host_codeDnld_t
{
    UINT8 Code[4096];
}
PACK_END host_codeDnld_t, *PDS_CODE_DNLD;

/*Define data structure for CMD_OP_PARAM_DNLD*/
typedef PACK_START struct host_opParamDnld_t
{
    UINT8 OpParam[4096];
}
PACK_END host_opParamDnld_t, *PDS_OP_PARAM_DNLD;

#define FW_CAPINFO_WPA       (1 << 0)
#define FW_CAPINFO_PS        (1 << 1)
#define FW_CAPINFO_GM        (1 << 2)
#define FW_CAPINFO_NO_EEPROM (1 << 3)
#define FW_CAPINFO_TX_ANT_BITNO 4   /* bit 5 & bit 4 */
#define FW_CAPINFO_RX_ANT_BITNO 6   /* bit 7 & bit 6 */
#define FW_CAPINFO_802_11B    (1 << 8)
#define FW_CAPINFO_802_11G    (1 << 9)
#define FW_CAPINFO_802_11A    (1 << 10)

typedef PACK_START struct host_getHwSpec_t
{
    UINT16 HwIfVersion;     /* HW Interface Version */
    UINT16 HwVersion;       /* HW/FW version */
    UINT16 NumOfWCB;        /* Max. number of WCB FW can handle */
    UINT16 NumOfMCastAdr;   /* Max. number of Multicast address FW can handle */
    UINT8 PermanentAddr[6]; /* MAC address */
    UINT16 RegionCode;      /* Region code */
    UINT16 NumberOfAntenna; /* Number of antenna used */
    UINT32 FWReleaseNumber; /* 4 byte of FW release number. Ex. 0x1234 = 1.2.3.4 */
    UINT32 WcbBase;
    UINT32 RxPdRdPtr;
    UINT32 RxPdWrPtr;
    UINT32 fwCapInfo;       /* Firmware Capability Info */
}
PACK_END host_getHwSpec_t, *PDS_GET_HW_SPEC;

/*Define data structure for CMD_EEPROM_UPDATE*/
typedef PACK_START struct host_eepromUpdate_T
{
    UINT16 Action;      /*Detailed action or option */
}
PACK_END host_eepromUpdate_T, *PDS_EEPROM_UPDATE;

/*Define data structure for CMD_802_11_QUERY_AP*/
typedef PACK_START struct host_802_11QueryAp_t
{
    /*TBD*/ UINT32 dummy;
}
PACK_END host_802_11QueryAp_t, *PDS_802_11_QUERY_AP;

/*Define data structure for CMD_802_11_QUERY_IBSS_STA*/
typedef PACK_START struct host_802_11QueryIbssSta_t
{
    /*TBD*/ UINT32 dummy;
}
PACK_END host_802_11QueryIbssSta_t, *PDS_802_11_QUERY_IBSS_STA;

/*Define data structure for CMD_802_11_QUERY_TRAFFIC*/
typedef PACK_START struct host_802_11QueryTraffic_t
{
    /*TBD*/ UINT32 Traffic; /* Traffic in bps */
}
PACK_END host_802_11QueryTraffic_t, *PDS_802_11_QUERY_TRAFFIC;

/*Define data structure for CMD_802_11_QUERY_STATUS*/
typedef PACK_START struct host_802_11QueryStatus_t
{
    UINT16 FWStatus;
    UINT16 MACStatus;
    UINT16 RFStatus;
    UINT16 CurentChannel;   /*1..99 */
    UINT8 APMACAdr[6];      /* Associated AP MAC address */
    UINT16 Reserved;
    UINT32 MaxLinkSpeed;    /*Allowable max.link speed in unit of 100bps */
}
PACK_END host_802_11QueryStatus_t, *PDS_802_11_QUERY_STATUS;

/*Define data structure for CMD_802_11_GET_LOG*/
typedef PACK_START struct host_802_11GetLog_t
{
    UINT32   mcasttxframe;
    UINT32   failed;
    UINT32   retry;
    UINT32   multiretry;
    UINT32   framedup;
    UINT32   rtssuccess;
    UINT32   rtsfailure;
    UINT32   ackfailure;
    UINT32   rxfrag;
    UINT32   mcastrxframe;
    UINT32   fcserror;
    UINT32   txframe;
    UINT32   reserved;
}
PACK_END host_802_11GetLog_t, *PDS_802_11_GET_LOG;

/*Define data structure for CMD_MAC_TX_ENABLE*/
typedef PACK_START struct host_macTxEnable_t
{
    UINT16 Action;      /*ACT_ON, ACT_OFF */
    UINT16 Reserved;
}
PACK_END host_macTxEnable_t, *PDS_MAC_TX_ENABLE;

/*Define data structure for CMD_MAC_RX_ENABLE*/
typedef PACK_START struct host_macRxEnable_t
{
    UINT16 Action;      /*ACT_ON, ACT_OFF */
    UINT16 Reserved;
}
PACK_END host_macRxEnable_t, *PDS_MAC_RX_ENABLE;

/*Define data structure for CMD_MAC_LOOP_BACK_ENABLE*/
typedef PACK_START struct host_macLoopBackEnable_t
{
    UINT16 Action;      /*ACT_ON, ACT_OFF */
    UINT16 Reserved;
}
PACK_END host_macLoopBackEnable_t, *PDS_MAC_LOOP_BACK_ENABLE;

/*Define data structure for CMD_MAC_INT_ENABLE*/
typedef PACK_START struct host_macIntEnable_t
{
    UINT16 Action;      /*ACT_ON, ACT_OFF */
    UINT16 Reserved;
}
PACK_END host_macIntEnable_t, *PDS_MAC_INT_ENABLE;

/*Define data structure for CMD_MAC_MULTICAST_ADR*/
typedef PACK_START struct host_macMulticastAdr_t
{
    UINT16 Action;
    UINT16 NumOfAdrs;
    UINT8 MACList[host_SIZE_MAC_ADR * host_MAX_MCAST_ADRS];
}
PACK_END host_macMulticastAdr_t, *PDS_MAC_MULTICAST_ADR;

/*Define data structure for CMD_802_11_ASSOCIATE*/
typedef PACK_START struct host_802_11Associate_t
{
    /*TBD*/ UINT32 dummy;
}
PACK_END host_802_11Associate_t, *PDS_802_11_ASSOCIATE;

/*Define data structure for CMD_802_11_SET_WEP*/
typedef PACK_START struct host_802_11SetWep_t
{
    UINT16 Action;      /*ACT_ADD, ACT_REMOVE, ACT_ENABLE */
    UINT16 TxKeyIndx;
    UINT8 K1WEPType;        /*40, 128bit or TXWEP */
    UINT8 K2WEPType;        /*40, 128bit or TXWEP */
    UINT8 K3WEPType;        /*40, 128bit or TXWEP */
    UINT8 K4WEPType;        /*40, 128bit or TXWEP */
    UINT8 WEP1[16];
    UINT8 WEP2[16];
    UINT8 WEP3[16];
    UINT8 WEP4[16];
}
PACK_END host_802_11SetWep_t, *PDS_802_11_SET_WEP;

/*Define data structure for CMD_802_3_GET_STAT*/
typedef PACK_START struct host_802_3GetStat_t
{
    UINT32 dummy;
}
PACK_END host_802_3GetStat_t, *PDS_802_3_GET_STAT;

/*Define data structure for CMD_802_11_GET_STAT*/
typedef PACK_START struct host_802_11GetStat_t
{
    UINT32 TXFragmentCnt;
    UINT32 MCastTXFrameCnt;
    UINT32 FailedCnt;
    UINT32 RetryCnt;
    UINT32 MultipleRetryCnt;
    UINT32 RTSSuccessCnt;
    UINT32 RTSFailureCnt;
    UINT32 ACKFailureCnt;
    UINT32 FrameDuplicateCnt;
    UINT32 RXFragmentCnt;
    UINT32 MCastRXFrameCnt;
    UINT32 FCSErrorCnt;
    UINT32 BCastTXFrameCnt;
    UINT32 BCastRXFrameCnt;
    UINT32 TXBeacon;
    UINT32 RXBeacon;
    UINT32 WEPUndecryptable;
}
PACK_END host_802_11GetStat_t, *PDS_802_11_GET_STAT;

/*Define data structure for CMD_802_11_SNMP_MIB*/
typedef PACK_START struct host_802_11SnmpMib_t
{
    UINT16 Action;
    UINT16 OID;
    UINT16 Size;
    UINT8 Value[128];
}
PACK_END host_802_11SnmpMib_t, *PDS_802_11_SNMP_MIB;

/*Define data structure for CMD_MAC_REG_MAP*/
typedef PACK_START struct host_macRegMap_t
{
    /*TBD*/ UINT8 RegMap[128];
}
PACK_END host_macRegMap_t, *PDS_MAC_REG_MAP;

/*Define data structure for CMD_BBP_REG_MAP*/
typedef PACK_START struct host_bbpRegMap_t
{
    UINT8 RegMap[128];
}
PACK_END host_bbpRegMap_t, *PDS_BBP_REG_MAP;

/*Define data structure for CMD_RF_REG_MAP*/
typedef PACK_START struct host_rfRegMap_t
{
    UINT8 RegMap[64];
}
PACK_END host_rfRegMap_t, *PDS_RF_REG_MAP;

/*Define data structure for CMD_MAC_REG_ACCESS*/
typedef PACK_START struct host_macRegAccess_t
{
    UINT16 Action;
    /*TBD*/
    UINT16 Offset;  /*byte offset, aligned on DWORD boundary */
    UINT32 Value;
}
PACK_END host_macRegAccess_t, *PDS_MAC_REG_ACCESS;

/*Define data structure for CMD_BBP_REG_ACCESS*/
typedef PACK_START struct host_bbpRegAccess_t
{
    UINT16 Action;
    UINT16 Offset;      /*byte offset */
    UINT8 Value;
    UINT8 Reserverd[3];
}
PACK_END host_bbpRegAccess_t, *PDS_BBP_REG_ACCESS;

/*Define data structure for CMD_RF_REG_ACCESS*/
typedef PACK_START struct host_rfRegAccess_t
{
    UINT16 Action;
    UINT16 Offset;      /*byte offset */
    UINT8 Value;
    UINT8 Reserverd[3];
}
PACK_END host_rfRegAccess_t, *PDS_RF_REG_ACCESS;

typedef PACK_START struct host_memAccess_t
{
    UINT16 Action;
    /*TBD*/
    UINT16 reserved;  /*byte offset, aligned on DWORD boundary */
    UINT32 addr;
    UINT32 value;
}
PACK_END host_memAccess_t, *PDS_MEM_ACCESS;

/*Define data structure for CMD_802_11_SLEEP_PARAMS*/
typedef PACK_START struct host_SleepParams_t
{
	UINT16 Action;
	UINT16 SleepClkErr;
	UINT16 Offset;
	UINT16 StableTime;
	UINT8  CalControl;
	UINT8  ExternalSleepClk;
	UINT16 Reserved;
} PACK_END host_SleepParams_t, *PDS_SleepParams_t;

typedef PACK_START struct host_SleepPeriod_t
{
	UINT16 Action;
    UINT16 SleepPeriod;
} PACK_END host_SleepPeriod_t, *PDS_SleepPeriod_t;

typedef PACK_START struct host_InactivityTmo_t
{
	UINT16 Action;
    UINT16 InactivityTmo;
} PACK_END host_InactivityTmo_t, *PDS_InactivityTmo_t;

/*Define data structure for CMD_802_11_RADIO_CONTROL*/
typedef PACK_START struct host_802_11RadioCtl_t
{
    UINT16 Action;      /*ON, OFF, long or short preamble */
    UINT16 Control;
}
PACK_END host_802_11RadioCtl_t, *PDS_802_11_RADIO_CONTROL;

/*Define data structure for CMD_802_11_RF_CHANNEL*/
typedef PACK_START struct host_802_11RfChnl_t
{
    UINT16 Action;
    UINT16 CurentChannel;
    UINT16 RFType;      /*802.11a or 802.11b */
    UINT16 Reserved;
    UINT8 ChannelList[32];
}
PACK_END host_802_11RfChnl_t, *PDS_802_11_RF_CHANNEL;

/*Define data structure for CMD_802_11_RSSI*/
typedef PACK_START struct host_802_11RssiReq_t
{
    UINT16 avgWeightFactor;
}
PACK_END host_802_11RssiReq_t, *PDS_802_11_RSSI_REQ;

typedef PACK_START struct host_802_11TxRate_t
{
    UINT16 TxRate;
}
PACK_END host_802_11TxRate_t, *PDS_802_11_TXRATE;

typedef PACK_START struct host_802_11Rssi_t
{
    UINT16 SNR;         /*SNR in dB */
    UINT16 NoiseFloor;
    UINT16 AvgSNR;
    UINT16 AvgNoiseFloor;
}
PACK_END host_802_11Rssi_t, *PDS_802_11_RSSI;

typedef PACK_START struct host_802_11RfTxPowerRequest_t
{
	SINT16 CurrentLevel;    /* 1..8 */
}
PACK_END host_802_11RfTxPowerRequest_t;

typedef PACK_START struct host_802_11RfTxPowerResponse_t
{
	SINT16 CurrentLevel;    /* 1..8 */
	SINT8  MaxPower;
	SINT8  MinPower;
}
PACK_END host_802_11RfTxPowerResponse_t;

/*Define data structure for CMD_802_11_RF_TX_POWER*/
typedef PACK_START struct host_802_11RfTxPower_t
{
    UINT16 Action;
	PACK_START union
	{
		host_802_11RfTxPowerRequest_t Request;
		host_802_11RfTxPowerResponse_t Response;
	} PACK_END Body;
}
PACK_END host_802_11RfTxPower_t, *PDS__802_11_RF_TX_POWERDS;

/*Define data structure for CMD_802_11_RF_ANTENNA*/
typedef PACK_START struct host_802_11RfAntenna_t
{
    UINT16 Action;
    UINT16 AntennaMode;     /*Number of antennas or 0xffff(diversity) */
}
PACK_END host_802_11RfAntenna_t, *PDS_802_11_RF_ANTENNA;

/*Define data structure for CMD_802_11_PS_MODE*/
typedef PACK_START struct host_802_11PsMode_t
{
    UINT16 Action;
    UINT16 NullPktInterval;
    UINT16 numDtims;
    UINT16 BCNMissTimeOut;
    UINT16 LocalListenInterval;
    UINT16 AdhocAwakePeriod;        // for ADHOC_PS_BCN_TX_NAP
}
PACK_END host_802_11PsMode_t, *PDS_802_11_PS_MODE;

/*Define data structure for CMD_802_11_DATA_RATE*/
typedef PACK_START struct host_802_11DataRate_t
{
    UINT16 Action;
    UINT16 AutoRate;
    UINT8 DataRate[14];      /*Supported data reate list */
}
PACK_END host_802_11DataRate_t, *PDS_802_11_DATA_RATE;

/*Define data structure for CMD_802_11_DATA_RATE*/
typedef PACK_START struct host_802_11DataRateSet_t
{
    UINT16 Action;

#define HW_RATEDROP_DISABLED    0
#define HW_RATEDROP_AUTO_MODE   1
#define HW_RATEDROP_SINGLE_MODE 2
    UINT16 HWRateDropMode;

    UINT16 Bitmap;      /*Supported data reate list */
    UINT16 Threshold;
    UINT16 FinalRate;

}
PACK_END host_802_11DataRateSet_t, *PDS_802_11_DATA_RATE_SET;

/*Define data structure for CMD_MAC_CONTROL*/
typedef PACK_START struct host_MacControl_t
{
    UINT16 Control;
    UINT16 Reserverd;
}
PACK_END host_MacControl_t, *PDS_MacControl_t;

#ifdef GPIO_STATE
typedef PACK_START struct host_SetGPIOPinState_t
{
    UINT8 gpioNum;
    UINT8 State;
}
PACK_END host_SetGPIOPinState_t;
#endif

/*Define data structure for CMD_802_11_QUERY_RSN_OPTION*/
typedef PACK_START struct host_802_11QueryRSNOption
{
    UINT8 RSN_Capable;
    UINT8 Reserved[3];
}
PACK_END host_802_11QueryRSNOption_t, *PDS__802_11_QUERY_RSN_OPTION;

/*Define data structure for CMD_802_11_ENABLE_RSN*/
typedef PACK_START struct host_802_11EnableRSN
{
    UINT16 Action;
    UINT16 Enable;
}
PACK_END host_802_11EnableRSN_t, *PDS__802_11_ENABLE_RSN;
/*Define data structure for CMD_802_11_CONFIG_RSN*/
typedef PACK_START struct host_802_11ConfigRSN
{
    UINT16 Action;
    UINT8 Version;
    UINT8 PairWiseKeysSupported;
    UINT8 MultiCastCipher[4];
    UINT8 GroupKeyMethod;
    UINT32 GroupReKeyTime;
    UINT32 GroupReKeyPkts;
    UINT8 GroupReKeyStrict;
    UINT8 TsnEnabled;
    UINT32 GroupMasterRekeyTime;
    UINT32 GroupUpdateTmo;
    UINT32 GroupUpdateCnt;
    UINT32 PairWiseUpdateTmo;
    UINT32 PairWiseUpdateCnt;
}
PACK_END host_802_11ConfigRSN_t, *PDS__802_11_CONFIG_RSN;

/*Define data structure for CMD_802_11_UNICAST_CIPHER*/
typedef PACK_START struct host_802_11UnicastCipher
{
    UINT16 Action;
    UINT8 UnicastCipher[4];
    UINT16 Enabled;
}
PACK_END host_802_11UnicastCipher_t, *PDS__CMD_802_11_UNICAST_CIPHER;

typedef PACK_START struct host_802_11McastCipher
{
    UINT16 Action;
    UINT8 McastCipher[4];
}
PACK_END host_802_11McastCipher_t, *PDS__CMD_802_11_MCAST_CIPHER;

/*Define data structure for CMD_802_11_RSN_AUTH_SUITES*/
typedef PACK_START struct host_802_11RSNAuthSuites
{
    UINT16 Action;
    UINT8 AuthSuites[4];
    UINT16 Enabled;
}
PACK_END host_802_11RSNAuthSuites_t, *PDS__CMD_802_11_RSN_AUTH_SUITES;

/*Define data structure for CMD_802_11_RSN_STATS*/
typedef PACK_START struct host_802_11_RSNStats
{
    MIB_RSNSTATS RSNStats;
}
PACK_END host_802_11RSNStats_t, *PDS__CMD_802_11_RSN_STATS;

#define host_ACT_GET_AES      0x0002
#define host_ACT_SET_AES      0x0003
#define host_ACT_CLEAR		  0x0004

/*Define data structure for CMD_802_11_PWK_KEY*/
typedef PACK_START struct host_802_11PwkKey
{
    UINT16 Action;
    UINT8 TkipEncryptKey[16];
    UINT8 TkipTxMicKey[8];
    UINT8 TkipRxMicKey[8];
}
PACK_END host_802_11PwkKey_t, *PDS__CMD_802_11_PWK_KEY;
/*Define data structure for CMD_802_11_GRP_KEY*/
typedef PACK_START struct host_802_11GrpKey
{
    UINT16 Action;
    UINT8 TkipEncryptKey[16];
    UINT8 TkipTxMicKey[8];
    UINT8 TkipRxMicKey[8];
}
PACK_END host_802_11GrpKey_t, *PDS__CMD_802_11_GRP_KEY;
/*Define data structure for CMD_802_11_PAIRWISE_TSC*/
typedef PACK_START struct host_802_11PwTSC
{
    UINT16 Action;
    UINT32 TxIv32;
    UINT16 TxIv16;
}
PACK_END host_802_11PwTSC_t, *PDS__CMD_802_11_PAIRWISE_TSC;

/*Define data structure for CMD_802_11_GROUP_TSC*/
typedef PACK_START struct host_802_11GrpTSC
{
    UINT16 Action;
    UINT32 TxIv32;
    UINT16 TxIv16;
}
PACK_END host_802_11GrpTSC_t, *PDS__CMD_802_11_GROUP_TSC;

/*Define data structure for CMD_802_11_QUERY_TKIP_REPLY_CNTRS*/
typedef PACK_START struct host_802_11QueryTkipReplyCntrs
{
    UINT32 NumTkipCntrs;
}
PACK_END host_802_11QueryTkipReplyCntrs_t,
*PDS__CMD_802_11_QUERY_TKIP_REPLY_CNTR;

/* Define data structure for CMD_802_11_KEY_DECRYPT */
typedef PACK_START struct host_802_11_KEY_DECRYPT
{
    UINT16 Action;
    UINT16 DecType;
    UINT8 KeyDecKey[16];
    UINT8 KeyIV[16];
    UINT16 KeyDataLen;
    UINT8 KeyData[1];
}
PACK_END host_802_11_KEY_DECRYPT_t;

typedef PACK_START struct host_MRVL_AES_CRYPT
{
	UINT16 action; /* de/en-crypt */
	UINT16 algorithm;
	UINT16 keyIVLen;
	UINT8  keyIV[32];
	UINT16 keyLen;
	UINT8  key[32];
	MrvlIEAesCrypt_t aesTlv;
}
PACK_END host_MRVL_AES_CRYPT_t;

typedef PACK_START struct host_802_11_KEY_ENCRYPT
{
    UINT16 Action;
    UINT16 EncType;
    UINT8 KeyIV[16];
    UINT8 KeyEncKey[16];
    UINT16 KeyDataLen;
    UINT8 KeyData[1];
}
PACK_END host_802_11_KEY_ENCRYPT_t;

typedef PACK_START struct host_eepromAccess
{
	UINT16	Action;
	UINT16	Offset;
	UINT16	ByteCnt;
	UINT8	Value;
} PACK_END	host_eepromAccess_t;

// Customer specific command for disabling ATIM tx when knowing destination is
// always powered (e.g. printer).
//
typedef PACK_START struct
{
    UINT16 Action;
    UINT16 Enable;
}
PACK_END host_atim_tx_t;

// Set beacon aCWmin: small value means aggressive beacon tx.
//
typedef PACK_START struct
{
    UINT16 Action;
    UINT16 Value;
}
PACK_END host_bcn_cw_t;

#ifdef ADHOC_BCN_TUNE
typedef PACK_START struct host_802_11BncTurnOpt_t
{
    UINT16 Action;
    UINT16 enabled;     /* 1 to enable, 0 to disable */
    UINT16 delta;
    UINT16 cwBcn;
    UINT16 csTxNum;
    UINT16 csRxNum;
}
PACK_END host_802_11BncTurnOpt_t, *PDS_802_11_BCN_TUNE_OPT;
#endif /* ADHOC_BCN_TUNE */

typedef PACK_START struct host_802_11GPIO_CFG_t
{
    UINT16 Action;
    UINT8 gpioNum;      /* bitmap of gpio: 0-7 */
    UINT8 dir;          /* bitmap of gpio dir: 0=input, 1=output */
}
PACK_END host_802_11GPIO_CFG_t, *PDS_802_11_GPIO_CFG;

typedef PACK_START struct host_802_11GPIO_OP_t
{
    UINT16 Action;
    UINT8 gpioNum;      /* 0-7 */
    UINT8 state;
}
PACK_END host_802_11GPIO_OP_t, *PDS_802_11_GPIO_OP;

/* host command for GET/SET Key Material */
typedef PACK_START struct host_802_11Key_Material_t
{
    UINT16 Action;
    MrvlIEKeyParamSet_t  tlvKey;
}
PACK_END host_802_11Key_Material_t;

typedef PACK_START struct host_802_11BG_Scan_Config_t
{
	UINT16                   Action;
	IEEEtypes_BGScan_Info_t bgScanInfo;
}
PACK_END host_802_11BG_Scan_Config_t;

typedef PACK_START struct host_802_11BG_Scan_Query_Req_t
{
	UINT8                   Flush;
}
PACK_END host_802_11BG_Scan_Query_Req_t;

typedef PACK_START struct host_802_11BG_Scan_Query_Rsp_t
{
    /* Report Condition matched since last query */
    UINT32 ReportCondition;

    /* Normal Scan response */
    w81cbProcQ_ScanRsp_t scanResp;
}
PACK_END host_802_11BG_Scan_Query_Rsp_t;

/*  Define data structure for APCmd_CMD_802_11_QUERY_STATUS */
typedef struct _APCmd_DS_802_11_QUERY_STATUS
{

    UINT16 FWStatus;
    UINT16 MACStatus;
    UINT16 RFStatus;
    UINT16 EthernetPortStatus;
    UINT16 CurentChannel;   /* 1..99 */
    UINT8 APName[32];
    UINT8 APMACAdr[6];      /*  AP MAC address */
    UINT16 NumOfAssociatedSTA;
    UINT32 MaxLinkSpeed;    /* Allowable max.link speed in unit of 1000 bps */
}
APCmd_DS_802_11_QUERY_STATUS, *PAPCmd_DS_802_11_QUERY_STATUS;

/*  Define data structure for
 APCmd_CMD_802_11_GET_AUTH_STA_LIST and APCmd_CMD_802_11_GET_ASSOC_STA_LIST*/
typedef struct _APCmd_CMD_802_11_STA_LIST
{
    UINT16 Index;
    UINT16 AID;
    UINT8 StationName[32];  /* can be NULL */
    UINT8 MACAdr[6];        /*  MAC address of the station */
    struct _APCmd_CMD_802_11_STA_LIST *Next;    /* Points to next station in the list */
}
APCmd_CMD_802_11_STA_LIST, *PAPCmd_CMD_802_11_STA_LIST;

/*  Define data structure for APCmd_CMD_802_11_GET_AUTH_STAS */
typedef struct _APCmd_CMD_802_11_GET_AUTH_STA_LIST
{

    PAPCmd_CMD_802_11_STA_LIST pStationList;
}
APCmd_CMD_802_11_GET_AUTH_STA_LIST, *PAPCmd_CMD_802_11_GET_AUTH_STA_LIST;

/*  Define data structure for APCmd_CMD_802_11_GET_ASSOC_STAS */
typedef struct _APCmd_CMD_802_11_GET_ASSOC_STA_LIST
{

    PAPCmd_CMD_802_11_STA_LIST pStationList;
}
APCmd_CMD_802_11_GET_ASSOC_STA_LIST, *PAPCmd_CMD_802_11_GET_ASSOC_STA_LIST;

/*  Define data structure for APCmd_CMD_802_11_QUERY_STA_INFO */
typedef struct _APCmd_CMD_802_11_QUERY_STA_INFO
{

    UINT8 MACAdr[6];        /*  MAC address of the station to be queried */
    UINT8 IPAddress[6];     /*  Info returned by AP, currently use the first 4 bytes */
    UINT16 PowerSaveMode;   /*  Info returned by AP */
    UINT16 WepStatus;       /*  Info returned by AP, see WEP Encryption section */
}
APCmd_CMD_802_11_QUERY_STA_INFO, *PAPCmd_CMD_802_11_QUERY_STA_INFO;

/*  Define data structure for APCmd_CMD_802_11_SSID */
typedef struct _APCmd_CMD_802_11_SSID
{

    UINT16 Action;      /* see general purpose section */
    UINT16 SsidLength;
    UINT8 Ssid[32];
    UINT16 AcceptOpenSsis;  /* 1: Accept, 0: Do not accept */
}
APCmd_CMD_802_11_SSID, *PAPCmd_CMD_802_11_SSID;

/*  Define data structure for APCmd_CMD_802_11_PRIVACY_OPTION */
typedef struct _APCmd_CMD_802_11_PRIVACY_OPTION
{

    UINT16 Action;      /* see general purpose section */
    UINT16 DataEncryption;  /* see privacy option section */
}
APCmd_CMD_802_11_PRIVACY_OPTION, *PAPCmd_CMD_802_11_PRIVACY_OPTION;

/*  Define data structure for APCmd_CMD_802_11_AUTHENTICATION_MODE */
typedef struct _APCmd_CMD_802_11_AUTHENTICATION_MODE
{

    UINT16 Action;      /* see general purpose section */
    UINT16 AuthMode;        /* see authentication mode section */
}
APCmd_CMD_802_11_AUTHENTICATION_MODE, *PAPCmd_CMD_802_11_AUTHENTICATION_MODE;

/*  Define data structure for APCmd_CMD_802_11_FRAGMENTATION_THRESHOLD */
typedef struct _APCmd_CMD_802_11_FRAGMENTATION_THRESHOLD
{

    UINT16 Action;      /* see general purpose section */
    UINT16 FragmentationThreshold;
}
APCmd_CMD_802_11_FRAGMENTATION_THRESHOLD,
*PAPCmd_CMD_802_11_FRAGMENTATION_THRESHOLD;

/*  Define data structure for APCmd_CMD_802_11_RTS_THRESHOLD */
typedef struct _APCmd_CMD_802_11_RTS_THRESHOLD
{

    UINT16 Action;      /* see general purpose section */
    UINT16 RtsThreshold;
}
APCmd_CMD_802_11_RTS_THRESHOLD, *PAPCmd_CMD_802_11_RTS_THRESHOLD;

/*  Define data structure for APCmd_CMD_802_11_DATA_RETRY_LIMIT */
typedef struct _APCmd_CMD_802_11_DATA_RETRY_LIMIT
{

    UINT16 Action;      /* see general purpose section */
    UINT16 DataRetryLimit;
}
APCmd_CMD_802_11_DATA_RETRY_LIMIT, *PAPCmd_CMD_802_11_DATA_RETRY_LIMIT;

/*  Define data structure for APCmd_CMD_802_11_RTS_RETRY_LIMIT */
typedef struct _APCmd_CMD_802_11_RTS_RETRY_LIMIT
{

    UINT16 Action;      /* see general purpose section */
    UINT16 RtsRetryLimit;
}
APCmd_CMD_802_11_RTS_RETRY_LIMIT, *PAPCmd_CMD_802_11_RTS_RETRY_LIMIT;

/*  Define data structure for
 APCmd_CMD_802_11_TRANSMIT_RATES and APCmd_CMD_802_11_SUPPORTED_RATES*/
typedef struct _APCmd_DS_802_11_DATA_RATE
{

    UINT16 Action;      /* Ignore if the command is APCmd_CMD_802_11_SUPPORTED_RATE */
    UINT8 DataRate[8];      /* Transmit or Supported data rate list */
}
APCmd_DS_802_11_DATA_RATE, *PAPCmd_DS_802_11_DATA_RATE;

/*  Define data structure for APCmd_CMD_802_11_RADIO_PREAMBLE */
typedef struct _APCmd_DS_802_11_RADIO_PREAMBLE
{

    UINT16 Action;      /* see general purpose section */
    UINT16 RadioPreamble;   /* see radio preamble section */
}
APCmd_DS_802_11_RADIO_PREAMBLE, *PAPCmd_DS_802_11_RADIO_PREAMBLE;

/*  Define data structure for APCmd_CMD_802_11_BEACON_PERIOD */
typedef struct _APCmd_DS_802_11_BEACON_PERIOD
{

    UINT16 Action;      /* see general purpose section */
    UINT16 BeaconPeriod;    /* in unit of 1024 microseconds (us) */
}
APCmd_DS_802_11_BEACON_PERIOD, *PAPCmd_DS_802_11_BEACON_PERIOD;

/*  Define data structure for APCmd_CMD_802_11_DTIM_PERIOD */
typedef struct _APCmd_DS_802_11_DTIM_PERIOD
{

    UINT16 Action;      /* see general purpose section */
    UINT16 DtimPeriod;      /* in unit of beacon intervals */
}
APCmd_DS_802_11_DTIM_PERIOD, *PAPCmd_DS_802_11_DTIM_PERIOD;

/*  Define data structure for APCmd_CMD_802_3_GET_STAT */
typedef struct _APCmd_DS_802_3_GET_STAT
{
    UINT32 XmitOK;
    UINT32 RcvOK;
    UINT32 XmitError;
    UINT32 RcvError;
    UINT32 XmitMulticast;
    UINT32 RcvMulticast;
    UINT32 XmitUnicast;
    UINT32 RcvUnicast;
    UINT32 RcvNoBuffer;
    UINT32 RcvCRCError;
}
APCmd_DS_802_3_GET_STAT, *PAPCmd_DS_802_3_GET_STAT;

/*  Define data structure for APCmd_CMD_802_3_LINK_SPEED */
typedef struct _APCmd_DS_802_3_LINK_SPEED
{

    UINT32 LinkSpeed;       /* in unit of 1000 bps */
}
APCmd_DS_802_3_LINK_SPEED, *PAPCmd_DS_802_3_LINK_SPEED;

/*  Define data structure for APCmd_CMD_802_3_IP_ADDRESS */
/*  This command is for query only if DHCP is enabled */
typedef struct _APCmd_DS_802_3_IP_ADDRESS
{

    UINT16 Action;      /* see general purpose section */
    UINT8 IPAddress[6];     /* currently use the first 4 bytes */
}
APCmd_DS_802_3_IP_ADDRESS, *PAPCmd_DS_802_3_IP_ADDRESS;

/*  Define data structure for APCmd_CMD_802_3_SUBNET_MASK */
/*  This command is for query only if DHCP is enabled */
typedef struct _APCmd_DS_802_3_SUBNET_MASK
{

    UINT16 Action;      /* see general purpose section */
    UINT8 IPAddress[6];     /* currently use the first 4 bytes */
}
APCmd_DS_802_3_SUBNET_MASK, *PAPCmd_DS_802_3_SUBNET_MASK;

/*  Define data structure for APCmd_CMD_802_3_GATEWAY */
/*  This command is for query only if DHCP is enabled */
typedef struct _APCmd_DS_802_3_GATEWAY
{
    UINT16 Action;      /* see general purpose section */
    UINT8 IPAddress[6];     /* currently use the first 4 bytes */
}
APCmd_DS_802_3_GATEWAY, *PAPCmd_DS_802_3_GATEWAY;

/*  Define data structure for APCmd_CMD_802_3_DHCP */
typedef struct _APCmd_DS_802_3_DHCP
{
    UINT16 Action;      /* see general purpose section */
    UINT16 DHCPStatus;      /* see DHCP section */
}
APCmd_DS_802_3_DHCP, *PAPCmd_DS_802_3_DHCP;

/*  Define data structure for APCmd_CMD_802_3_ICMP */
typedef struct _APCmd_DS_802_3_ICMP
{
    UINT16 Action;      /* see general purpose section */
    UINT16 ICMPStatus;      /* see ICMP section */
}
APCmd_DS_802_3_ICMP, *PAPCmd_DS_802_3_ICMP;

/*  Define data structure for APCmd_CMD_802_11_LINK_TEST */
typedef struct _APCmd_CMD_802_11_LINK_TEST
{
    UINT8 IPAddress[6];     /* MAC Address of the target station currently use the first 4 bytes */
    UINT16 PktNum;      /* Packet number to be sent */
    UINT16 PktSize;     /* Size of the packet to be sent */
    UINT16 XmtPkt;      /* Test result, pkt transmitted that received an echo packet */
    UINT16 TotalErr;        /* Test result, pkt failure due to any reason */
    UINT16 WepErr;      /* Test result, pkt failure due to WEP error */
}
APCmd_CMD_802_11_LINK_TEST, *PAPCmd_CMD_802_11_LINK_TEST;

#define TX_MODE_NORMAL  0
#define TX_MODE_CONT    1
#define TX_MODE_CW      2

/*Define data structure for CMD_802_11_TX_MODE*/
typedef PACK_START struct host_txMode_t
{
    UINT16 mode;
}
PACK_END host_txMode_t, *PDS_TX_MODE;

#define RX_MODE_NORMAL  0
#define RX_MODE_RDONLY  1
/*Define data structure for CMD_802_11_RX_MODE*/
typedef PACK_START struct host_rxMode_t
{
    UINT16 mode;
}
PACK_END host_rxMode_t, *PDS_RX_MODE;

/*Define data structure for CMD_802_11_RGN_CODE*/
typedef PACK_START struct host_rgnCode_t
{
    UINT16 Action;
    UINT16 rgnCode;
}
PACK_END host_rgnCode_t;

/*Define data structure for CMD_802_11_MAC_ADDR*/
typedef struct host_macAddr_t
{
    UINT16 Action;
    UINT8 macAddr[6];
}
PACK_END host_macAddr_t;

#define HOSTSLEEP_WAKE_UP_BROADCAST_DATA   0x0001 // Wake host up on non-unicast data
#define HOSTSLEEP_WAKE_UP_UNICAST_DATA     0x0002 // Wake host up on any unicast packet
#define HOSTSLEEP_WAKE_UP_MAC_EVENT        0x0004 // Wake host up on any MAC event
#define HOSTSLEEP_WAKE_UP_MULTICAST_DATA   0x0008 // Wake host up on any matched packet

/*Define data structure for host_CMD_802_11_HOST_SLEEP_CFG*/
typedef PACK_START struct host_Sleep
{
    UINT32 conditions;
    SINT8  gpio;
    UINT8  gap;
} PACK_END host_Sleep, *PDS_WakeUp;

typedef PACK_START struct host_CalData_Ext_t
{
    UINT16 Action;
    UINT16 rev;
    UINT16 len;
    UINT8  data;
}
PACK_END host_CalData_Ext_t;

#define SELECT11B   0
#define SELECT11G   1
#define SELECT11A   2
#define SELECT11J   3

typedef PACK_START struct host_BandSel_t
{
   UINT16      Action;
   UINT16      band;
   UINT16      desiredChannel;
#ifdef DOT11J
   UINT16      chanBW;
#endif
}
PACK_END host_BandSel_t;


/*Define data structure for CMD_802_11_PRE_TBTT*/
typedef PACK_START struct host_PreTbtt_t
{
    UINT16 Action;
    UINT16 PreTbttTime;
}
PACK_END host_PreTbtt_t, *PDS_PreTbtt_t;

#ifdef PASSTHROUGH

typedef PACK_START struct host_PassThrough_t
{
    UINT16 Action;
    UINT8 Enable;       /* 0: diable Passthrough mode, 1 Enable Passthrough mode */
    UINT8 Channel;
    UINT32 Filter1;
    UINT32 Filter2;
    UINT8 MatchBssid[6];
    UINT8 MatchSrcAddr[6];
    UINT8 MatchDstAddr[6];
}
PACK_END host_PassThrough_t;

#endif

typedef PACK_START struct host_GSPI_Bus_config_t
{
    UINT16 Action;
    UINT16 BusDelayMode;
    UINT16 HostTimeDelayToReadPort;
    UINT16 HostTimeDelayToReadRegister;
}
PACK_END host_GSPI_Bus_config_t;

#ifdef DOT11D
typedef PACK_START struct host_802_11_D_Domain_t
{
    UINT16 Action;
}
PACK_END host_802_11D_Domain_t;
#endif

typedef PACK_START struct host_Wmm_Ack_Policy_t
{
    UINT16 Action;
    UINT8  AccessCategory;
    UINT8  AckPolicy;
}
PACK_END host_Wmm_Ack_Policy_t;


typedef PACK_START struct host_Wmm_Prio_Pkt_Avail_t
{
    UINT8 PacketPriority;
}
PACK_END host_Wmm_Prio_Pkt_Avail_t;


typedef PACK_START struct
{
    WmmQueueStatus_t QueueStatus[AC_MAX_TYPES];

    /*
    ** Size of the IEEE element + 2 more bytes for the uint16 extension of
    **  the element ID and length
    */
    UINT8 WmmParamMrvlTlv[sizeof(IEEEtypes_WMM_Parameter_t) + 2];
}
PACK_END host_Wmm_Get_Status_t;


/*!
 * Defines Parameters for a DelTS Request
 */
typedef PACK_START struct
{
    /*! Firmware command error/success result code */
    uint8 commandResult;

    /*! Dialog token to use for the request */
    uint8 dialogToken;

    /*! DELTS reason code */
    uint8 reasonCode;

    /*! TSPEC element to be deleted */
    IEEEtypes_WMM_TSPEC_t tspecIE;

} PACK_END host_Wmm_Del_Ts_Req_t;


/*!
 * Defines Parameters for a TSPEC Request
 */
typedef PACK_START struct
{
    /*! Firmware command error/success result code */
    uint8 commandResult;

    /*! Timeout in ms for ADDTS Msg */
    uint32 timeout_ms;

    /*! Dialog token to use for the request */
    uint8 dialogToken;

    /*! ADDTS Resp status code */
    uint8 statusCode;

    /*! TSPEC element to be added */
    IEEEtypes_WMM_TSPEC_t tspecIE;

    /*! Extra IEs that are to be sent after the TSPEC in an ADDTS */
    uint8 extraIE[256];

} PACK_END host_Wmm_Add_Ts_Req_t;


typedef PACK_START struct
{
    UINT8  Action;            /* On, Off, Get stats for an AC */
    UINT8  AccessCategory;    /* AC the command applies to */
    UINT16 PktCount;          /* Total packet count */
    UINT16 PktLoss;           /* Total packet loss */
    UINT32 AvgQueueDelay;     /* Avg queue delay in microseconds */
    UINT32 AvgTxDelay;        /* Avg transmission delay in microseconds */
    UINT32 UsedTime;          /* Used medium time calculation */

    UINT16 DelayHistogram[WMM_STATS_PKTS_HIST_BINS];  /* Hist of queue delay
                                                         in milliseconds */
    UINT16 RoamDelay;         /* Delay between packets before/after roam */
}
PACK_END host_Wmm_Stats_Queue_Stats_t;

typedef PACK_START struct
{
    UINT8  Action;             /* Set, Get, Default queue configuration */
    UINT8  AccessCategory;     /* AC the command applies to */
    UINT16 MsduLifetimeExpiry; /* MSDU lifetime expiry in TUs for the queue */
    UINT8  TLVBuffer[64];      /* TBD: Used for TLV RateSet */
}
PACK_END host_Wmm_Queue_Config_t;

/* Define data structure for host_CMD_BCA_CONFIG_TIMESHARE */
typedef PACK_START struct host_BcaTimeShare_t
{
    UINT16 Action;
    UINT16 TrafficType;
    UINT32 TimeshareInterval;
    UINT32 BTTime;
}
PACK_END host_BcaTimeShare_t;

/* Define data structure for host_CMD_802_11_SUBSCRIBE_EVENT */
typedef PACK_START struct host_SubscribeEvent_t
{
    UINT16 Action;
    UINT16 Events;   /* Bitmap of subscribed events */
}
PACK_END host_SubscribeEvent_t;

typedef PACK_START struct host_LED_Cfg_t
{
   UINT16 Action;
   UINT16  NumLed;
} PACK_END host_LED_Cfg_t;

#define MAX_KEEPALIVE_PKT_LEN	(0x60)
typedef PACK_START struct _host_AutoTx_Cfg_t
{
   UINT16 Action;
   UINT16 Type;
   UINT16 Len;
   UINT16 Interval; /* in seconds */
   UINT8  Priority;
   UINT8  Reserved;
   UINT16 EtherFrmLen;
   UINT8  DestAddr[6];
   UINT8  SrcAddr[6];
   UINT16 EtherFrmBodyLen;
   UINT8  EtherFrmBody[MAX_KEEPALIVE_PKT_LEN]; //Last 4 bytes are 32bit FCS
} PACK_END host_AutoTx_Cfg_t;

typedef PACK_START struct hostCmd_t
{
    host_MsgHdr_t header;
    PACK_START union
    {
        host_getHwSpec_t getHwSpec;
        host_eepromUpdate_T eepromUpdate;
        host_802_11QueryAp_t QueryAp;
        host_802_11QueryIbssSta_t QueryIbssSta;
        host_802_11QueryTraffic_t QueryTraffic;
        host_802_11QueryStatus_t QueryStatus;
        host_802_11GetLog_t GetLog;
        host_macTxEnable_t macTxEnable;
        host_macRxEnable_t macRxEnable;
        host_macLoopBackEnable_t macLoopBackEnable;
        host_macIntEnable_t macIntEnable;
        host_macMulticastAdr_t macMulticastAdr;
        host_802_11Authenticate_t Authenticate;
        host_802_11Associate_t Associate;
        host_802_11SetWep_t SetWep;
        host_802_3GetStat_t GetStat802_03;
        host_802_11GetStat_t GetStat802_11;
        host_802_11SnmpMib_t SnmpMib;
        host_macRegMap_t macRegMap;
        host_bbpRegMap_t bbpRegMap;
        host_rfRegMap_t rfRegMap;
        host_macRegAccess_t macRegAccess;
        host_bbpRegAccess_t bbpRegAccess;
        host_rfRegAccess_t rfRegAccess;
#ifdef EEPROM_ACCESS
        host_eepromAccess_t eepromAccess;
#endif
        host_802_11RadioCtl_t RadioCtl;
        host_802_11RfChnl_t RfChnl;
        host_802_11TxRate_t TxRate;
        host_802_11Rssi_t Rssi;
        host_802_11RfTxPower_t RfTxPower;
        host_802_11RfAntenna_t RfAntenna;
        host_802_11PsMode_t PsMode;
        host_802_11DataRate_t DataRate;
        host_MacControl_t MacControl;

		host_MRVL_AES_CRYPT_t aesCrypt;
        host_802_11QueryRSNOption_t RSNOption;
        host_802_11EnableRSN_t EnableRSN;
        //host_802_11ConfigRSN_t ConfigureRSN;
        //host_802_11UnicastCipher_t UnicastCipher;
        //host_802_11RSNAuthSuites_t RSNAuthSuites;
        //host_802_11RSNStats_t RSNStats;
        host_802_11PwkKey_t PwkKey;
        host_802_11GrpKey_t GrpKey;
        host_802_11PwTSC_t PwTSC;
        host_802_11GrpTSC_t GrpTSC;
        host_802_11QueryTkipReplyCntrs_t TkipReplyCntrs;

#ifdef GPIO_STATE
        host_SetGPIOPinState_t GPIOPinState;
#endif

#ifdef DOT11D
        host_802_11D_Domain_t Domain;
#endif
    } CmdBody;
}
PACK_END hostCmd_t;

#ifdef AFC
typedef PACK_START struct _SET_AFC_CMD
{
    UINT16 AFC_auto;
    PACK_START union
    {
        PACK_START struct
        {
            UINT16 AFC_offset_thr;
            UINT16 AFC_update_period;

        }
        PACK_END auto_mode;
        PACK_START struct
        {
            SINT16 AFC_timing_offset;
            SINT16 AFC_carrier_offset;
        }
        PACK_END manual_mode;
    } PACK_END body;
}
PACK_END AFC_CMD_t;
#endif

typedef PACK_START struct _SME_MEASUREMENT_REQ_t
{
	IEEEtypes_MacAddr_t	peerStaAddr;
	UINT8 DialogToken;
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
PACK_END SME_MEASUREMENT_REQ_t;

typedef PACK_START struct _SME_MEASUREMENT_REP_t
{
	IEEEtypes_MacAddr_t	peerStaAddr;
	UINT8 DialogToken;
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
PACK_END SME_MEASUREMENT_REP_t;

typedef PACK_START struct _SME_CH_SWITCH_t
{
	uint8 Ch_Sw_Mode;
	uint8 ChNum;
	uint8 ChSwCnt;
}
PACK_END SME_CH_SWITCH_t;

typedef PACK_START struct _DFS_ACTION_t
{
	UINT8 Action;
}
PACK_END DFS_ACTION_t;

//define the DFS Actions
#define DFS_START_RADAR_DETECTION 0x00
#define DFS_START_CCA_DETECTION 0x02
#define DFS_START_RPI_DETECTION 0x04

typedef PACK_START struct _DFS_QUIET_ELEMENT_t
{
	UINT8 Action;
	IEEEtypes_QUIET_ELEMENT_t Quiet_Elem;
}
PACK_END DFS_QUIET_ELEMENT_t;

#define QUIET_IE_ADD 1
#define QUIET_IE_DEL 0

///////////////
//TPC and Power Adaptation commands.
//////////////
typedef PACK_START struct host_MrvlTPC_Cfg_t
{
	UINT16 Action;
	UINT8  EnableTPC;
	SINT8  TPC_P0;
	SINT8  TPC_P1;
	SINT8  TPC_P2;
	UINT8  UseSNR;
} PACK_END host_MrvlTPC_Cfg_t;

typedef PACK_START struct host_MrvlPowerAdaptation_Cfg_t
{
	UINT16 Action;
	UINT8  EnablePA;
	SINT8  PA_P0;
	SINT8  PA_P1;
	SINT8  PA_P2;
} PACK_END host_MrvlPowerAdaptation_Cfg_t;

#define POWER_ADAPT_MAX_GROUPS  5

typedef PACK_START struct
{
    UINT16 PA_level;
    UINT16 RateBitMap;
    UINT32 Reserved;
} PACK_END power_adapt_group_t;

// Power Adaptation group configuration TLV
typedef PACK_START struct
{
    MrvlIEParamSet_t    IEParam;
    power_adapt_group_t group[POWER_ADAPT_MAX_GROUPS];
} PACK_END MrvlIETypes_PowerAdapt_Group_t;

typedef PACK_START struct
{
	UINT16                          Action;
	UINT16                          EnablePA;
} PACK_END host_power_adapt_cfg_t;

#define HOSTSLEEP_WAKE_THRU_NONE   0
#define HOSTSLEEP_WAKE_THRU_INTR   1
#define HOSTSLEEP_WAKE_THRU_GPIO   2

typedef PACK_START struct host_MrvlHostWakeMethod_t
{
	UINT16 Action;
	UINT16 Method;
} PACK_END host_MrvlHostWakeMethod_t;

#ifdef ADHOC_COALESCING
typedef PACK_START struct host_BSSID_Status_t
{
    UINT16               Action;
    UINT16               Enable;
    IEEEtypes_MacAddr_t  AdhocBSSID;
    UINT16               BeaconInterval;
    UINT16               ATIMWindow;
    UINT16               UseGRateProtection;
} PACK_END host_BSSID_Status_t;
#endif

#ifdef WSC
typedef PACK_START struct host_wscRegistrarConfig_t
{
    UINT16              Action;
    MrvlIEParamSet_t    wscTlv;
} PACK_END host_wscRegistrarConfig_t;
#endif

typedef PACK_START struct host_SDIO_IntCfg
{
    UINT16               GPIOPin;
    UINT16               GPIOInterruptEdge;
    UINT16               GPIOPulseWidth;
} PACK_END host_SDIO_IntCfg;

typedef PACK_START struct host_SDIO_Interrupt_Config_t
{
    UINT16               Action;
    host_SDIO_IntCfg     GPIOCfg;
} PACK_END host_SDIO_Interrupt_Config_t;

typedef PACK_START struct host_SDIO_PullCtrl
{
    UINT16               PullUpDelay;
    UINT16               PullDownDelay;
} PACK_END host_SDIO_PullCtrl;

typedef PACK_START struct host_SDIOPullCtrl_t
{
    UINT16               Action;
    host_SDIO_PullCtrl   PullCfg;
} PACK_END host_SDIOPullCtrl_t, *PDS_SDIOPullCtrl_t;

typedef PACK_START struct
{
    UINT16 Action;
    UINT16 freq_set;
}
PACK_END host_clock_cfg;

#define LDO_SRC_INTERNAL 0
#define LDO_SRC_EXTERNAL 1
typedef PACK_START struct
{
    UINT16 Action;
    UINT16 LDOSrc;
}
PACK_END host_LDO_cfg;

#endif /* _HOST_H_ */
