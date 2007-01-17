/***************************************************************************
*
* File: wl_msgdefs.h
*
*
*
* Author(s):
* Date:
* Description:
*
***************************************************************************/

#ifndef WL_MSGDEFS_H
#define WL_MSGDEFS_H

#include "wltypes.h"
#include "IEEE_types.h"
#include "tlv.h"  /* MrvlIETsfArray_t */
#include "ecl_hostcmd.h"

/*Define data structure for CMD_802_11_AUTHENTICATE*/
typedef PACK_START struct host_802_11Authenticate_t
{
    UINT8 MacAddr[host_SIZE_MAC_ADR];
    UINT8 AuthType;
    UINT16 TimeOut;
    UINT8 Reserved[2];
}
PACK_END host_802_11Authenticate_t, *PDS_802_11_AUTHENTICATE;

typedef PACK_START struct w81cbProcQ_ScanRsp_t
{
    UINT16 BufSize;
    UINT8  NumSets;
    /* Place holder for the variable length fields.  Do not allocate here
    **   since it increases the memory requirements.  The field is filled
    **   in w81cbproc into the final interface buffer; therefore, the memory
    **   is not needed in the interprocess messaging.
    **
    **  BssDescAndTlvBuffer[MAX_SCAN_BUF_SIZE + sizeof(MrvlIETsfArray_t)];
    */
    UINT8  BssDescAndTlvBuffer[1];
}
PACK_END w81cbProcQ_ScanRsp_t;

typedef PACK_START struct smeQ_AdhocStartCmd_t
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

    /*
    ** Fixed binary fields for the host/driver command are above this point
    **  the IEBuffer[1] is left as a place holder for TLV arguments.
    */
	UINT8 IEBuffer[1];
}
PACK_END smeQ_AdhocStartCmd_t;
/* */
/* Start request message sent from the SME to start a new BSS; the BSS */
/* may be either an infrastructure BSS (with the MAC entity acting as the */
/* AP) or an independent BSS (with the MAC entity acting as the first */
/* station in the IBSS) */
/* */

#endif
