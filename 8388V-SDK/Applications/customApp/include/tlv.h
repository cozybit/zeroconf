/*****************************************************************************
 *
 * File: tlv.h
 *
 *        
 *                                                                          
 * Author(s):    Kapil Chhabra
 * Date:         2005-01-27
 * Description:  Definitions of the Marvell TLV and parsing functions.
 *
 *****************************************************************************/
#ifndef _TLV_H_
#define _TLV_H_

#include "tlv_id.h"
#include "dot11d.h"
#include "IEEE_types.h"
#include "dot11hTPC_types.h"

/*
** TLV IDs are assigned in tlv_id.h. 
*/

typedef PACK_START struct 
{
    UINT16 Type;
    UINT16 Length;
} PACK_END MrvlIEParamSet_t; 

typedef PACK_START struct
{
    MrvlIEParamSet_t hdr;
    UINT8 Value[1];
} PACK_END MrvlIEGeneric_t;

/* Key Material TLV */
typedef PACK_START struct MrvlIEKeyParamSet_t
{
    MrvlIEParamSet_t        hdr;
    UINT16                  keyMgtId;
}
    PACK_END MrvlIEKeyParamSet_t;

/* Marvell Power Constraint TLV */
typedef PACK_START struct MrvlIEPowerConstraint_t
{
    MrvlIEParamSet_t  IEParam;
    UINT8  channel;
    UINT8  dBm;
} 
    PACK_END  MrvlIEPowerConstraint_t;

/* Marvell WSC Selected Registar TLV */
typedef PACK_START struct MrvlIEWSCSelectedRegistrar_t
{
    MrvlIEParamSet_t  IEParam;
    UINT16            devPwdID;
} PACK_END  MrvlIEWSCSelectedRegistrar_t;


/* Marvell WSC Enrollee TMO TLV */
typedef PACK_START struct MrvlIEWSCEnrolleeTmo_t
{
    MrvlIEParamSet_t  IEParam;
    UINT16            tmo;
} PACK_END  MrvlIEWSCEnrolleeTmo_t;

/****************
 * AES CRYPTION FEATURE
 *
 * DEFINE STARTS --------------
 */
typedef PACK_START struct MrvlIEAesCrypt_t
{
	MrvlIEParamSet_t	hdr;
	UINT8			payload[32];
}
PACK_END MrvlIEAesCrypt_t;

/* DEFINE ENDS ----------------
 */

/* Marvell Power Capability TLV */
typedef PACK_START struct MrvlIEPowerCapability_t
{
    MrvlIEParamSet_t  IEParam;
    UINT8  minPwr;
    UINT8  maxPwr;
} 
    PACK_END  MrvlIEPowerCapability_t;

/* Marvell TLV for OMNI Serial Number and Hw Revision Information. */
typedef PACK_START struct MrvlIE_OMNI_t
{
    MrvlIEParamSet_t  IEParam;
    UINT8  SerialNumber[16];
    UINT8  HWRev;
    UINT8  Reserved[3];
} 
    PACK_END  MrvlIE_OMNI_t;

/* Marvell LED Behavior TLV */
typedef PACK_START struct MrvlIELedBehavior_t
{
    MrvlIEParamSet_t  IEParam;
    UINT8  FirmwareState;
    UINT8  LedNumber;
    UINT8  LedState;
    UINT8  LedArgs;  
}
    PACK_END  MrvlIELedBehavior_t;

/* Marvell LED Behavior TLV */

#ifdef SDK_FEATURE_XX
typedef PACK_START struct MrvlIEStaMac_t
{
    UINT32  uOUI;
	IEEEtypes_MacAddr_t Mac;
} PACK_END MrvlIEStaMac_t;

typedef PACK_START struct VendorIEMac_t
{
	IEEEtypes_ElementId_e ElementId;
	IEEEtypes_Len_t       Len;	
	MrvlIEStaMac_t        Payload;
} PACK_END VendorIEMac_t;

#define MRVL_MAC_ADDR_OUI (6<<16 | MRVL_STA_MAC_TLV_ID)
#endif

typedef PACK_START struct 
{
    MrvlIEParamSet_t  IEParam;

    /*
    ** Set a place holder for the TSF values.  Sized to max BSS for message
    **   allocation. The TLV will return a variable number of TSF values.
    */
    UINT64 TSFValue[IEEEtypes_MAX_BSS_DESCRIPTS];

} PACK_END MrvlIETsfArray_t;

/* Marvell AutoTx TLV */
#define MAX_KEEPALIVE_PKT_LEN	(0x60)
typedef PACK_START struct _MrvlAutoTx_t
{
    MrvlIEParamSet_t hdr;
    UINT16 Interval; /* in seconds */
    UINT8  Priority; 
    UINT8  Reserved;
    UINT16 EtherFrmLen;
    UINT8  DestAddr[6];
    UINT8  SrcAddr[6];
    UINT8  EtherFrmBody[MAX_KEEPALIVE_PKT_LEN]; //Last 4 bytes are 32bit FCS	
} PACK_END MrvlAutoTx_t;

extern UINT16 parse_MrvlIE(uint16 type, uint8 *srcBuf, 
                           uint8 *destBuf, uint16 destBufLen);
extern UINT16 parse_MrvlIE_ChannelList_Set(uint8 *srcBuf, 
                                           channelInfo_t * channelList,
                                           uint32* pScanSingleChan);
extern UINT16 parse_MrvlIE_KeyMaterial_Set_all(UINT8 *tlvParam_p, 
                                               UINT32 endCmdPtr);
extern UINT16 build_MrvlIE_KeyMaterial_all(UINT8 *tlvParam_p);
extern UINT16 build_MrvlIE_Terminator(UINT8 *tlvParam_p);
extern UINT16 parse_MrvlIE_SubscribeEvents_Set(uint8 *src, uint8 *dest, 
                                               uint8 *dest1);
extern UINT16 parse_MrvlIE_BeaconMissed_Set(uint8 *src, BOOLEAN (*fn)(),
                                            uint8 *dest);
extern UINT16 parse_MrvlIE_LEDBehavior(UINT8 *srcBuf, UINT8 len,
                                       UINT8 *destBuf);
extern UINT16 parse_MrvlIE_LEDGpio(UINT8 *srcBuf, UINT16 len, UINT8 *destBuf,
                                   void (*gpioInput)());
extern UINT16 build_MrvlIE_LED_all(UINT8 *tlvParam_p, UINT8 *srcBuf);

extern 
MrvlIEParamSet_t* parse_MrvlIE_RateChannelPwrTbl(MrvlIEParamSet_t *pbuffer,
                                                 UINT16 buffer_length,
                                                 UINT16 TLV_type);

extern BOOLEAN check_MrvlIE_ValidTLVBufferLength(MrvlIEParamSet_t *pStartTLV,
                                                 UINT16 total_buffer_length);

extern UINT16 parse_MrvlIE_PwrCapability(UINT8 *srcBuf,
                                         StaPowerCapability_t *PwrCap);

extern UINT16 parse_MrvlIE_NumProbes_Set(uint8 *src, uint8 *dest);

extern UINT16 parse_MrvlIE_NumSsidProbe_Set(uint8 *src, uint8 *dest);
extern UINT16 parse_MrvlIE_BcastProbe_Set(uint8 *src, uint8 *dest);

extern UINT16 parse_MrvlIE_wscEnrolleeSelRegistrar (UINT8 *srcBuf);
extern UINT16 parse_MrvlIE_wscEnrolleeTmo (UINT8 *srcBuf);
extern UINT16 parse_MrvlIE_wscEnrolleeProbeReq (UINT8 *srcBuf);

extern UINT16 parse_MrvlIE_LocalPwrConstraint(UINT8 *srcBuf,
                                  LocalPowerConstraint_t *locPwrConstraint);



#endif //_TLV_H_
