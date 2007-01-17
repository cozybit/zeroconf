/******************** (c) Marvell Semiconductor, Inc., 2006 *******************
 *
 * $Header:$
 *
 * Purpose:
 *    User Interface
 *
 * Public Procedures:
 *
 * Private Procedures:
 *
 * Notes:
 *    None.
 *
 *****************************************************************************/

#include "wltypes.h"
#include "dbg_Facility.h"
#include "wl_msgdefs.h"
#include "os_if.h"
#include "hostMsgHdr.h"
#include "sysinfo.h"
#include "mli.h"
#include "tx_api.h"
#include "sysinfo.h"
#include "ecl_hostcmd.h"
#include "tlv.h"
#include "tlv_id.h"
#include "eping.h"
#include "hostsme_sm.h"
#include "peersta_api.h"
#include "userif.h"
#ifdef EMBEDDED_TCPIP

#include "tcpip_socki.h"
#endif
/*  Configuration used for Treck ETCP stack/Raw Data configuration*/
char ip_addr[4] ;  
char net_mask[4] ; 
char def_gtwy[4] ; 


/* Events received from the firmware to the UserIF task*/
#ifndef sysinfo_NO_HOST_EVENT
#define sysinfo_NO_HOST_EVENT            	10
#endif

#define NO_HOST_EVENT		  				(1<<1)
#define FW_READY_EVENT                      (1<<2)
#define MAC_EVENT                           (1<<3)
#define REASSOC_TIMEOUT_EVENT               (1<<4)
#define NOHOST_EVENT_TRIGGERS               NO_HOST_EVENT | FW_READY_EVENT | MAC_EVENT | REASSOC_TIMEOUT_EVENT


/* Msg to trigger the state machine*/
uint32      HostSmeMsg_evt;  

/* Initialization constants for WLAN Driver*/
#define G_SUPPORTED_RATES		    14
#define	WLAN_SUPPORTED_RATES		14

/**
 * the rates supported by the card
 */
char WlanDataRates[WLAN_SUPPORTED_RATES] =
    { 0x02, 0x04, 0x0B, 0x16, 0x00, 0x0C, 0x12,
    0x18, 0x24, 0x30, 0x48, 0x60, 0x6C, 0x00
};

/**
 * the G rates supported
 */
char SupportedRates[G_SUPPORTED_RATES] =
    { 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, 0x30, 0x48, 0x60, 0x6c,
0 };

/* Buffer pointers to send commands and receive results*/
uint8 *cmd_buffer;
uint8 *cmd_res_buffer;

/* Number of b/g channels to scan*/
#define NUM_CHANNELS  11

/* Task information for the User Interface task*/
TX_THREAD				stUserIFThread;
static UINT8 			UserIFstack[4096];


#define MAX_BSS_HANDLES  20
BSSDescriptor_t     BSS_List[MAX_BSS_HANDLES];
int num_bss;
char curr_bss;
IEEEtypes_Bss_e  bss_type;
IEEEtypes_Bss_e  currbss_type;
UINT8  STAMACAddr[6];
int send_ping;
IEEEtypes_SsIdElement_t specificSSID;
int send_auth;
int link_present;
int cmd_in_progress;
IEEEtypes_MacAddr_t specificBSSID;
extern int use_peer_sta_api;

extern void cmd_parser(void);
extern void print_usage(void);
void parse_scan_result(w81cbProcQ_ScanRsp_t *res);

/**
 * Sends an event to the no host task if reassoc
 * timeout occurs
 * 
 * @param data
 * @return 
 */
SINT32 userif_ReAssocActTimeOut(UINT8 *data)
{
	os_EventTrigger(sysinfo_NO_HOST_EVENT, REASSOC_TIMEOUT_EVENT);
    return 0;
}

/**
 * Compares two SSIDs for match
 * 
 * @param ssid1
 * @param ssid2
 * @return 
 */
int SSIDcmp(IEEEtypes_SsIdElement_t * ssid1, IEEEtypes_SsIdElement_t * ssid2)
{
    if (!ssid1 || !ssid2)
        return -1;

    if (ssid1->Len != ssid2->Len)
        return -1;

    return memcmp((void *)ssid1->SsId, (void *)ssid2->SsId, ssid1->Len);
}

/**
 * Finds the SSID in the BSS_List
 * 
 * @return 
 */
int	FindSSIDinList(void)
{
  int i;

  for (i = 1 ; i <= num_bss; i++) {
	 if(SSIDcmp(&specificSSID,&BSS_List[i].ssid_element) == 0)
	 {
       curr_bss = i;
	   if(BSS_List[i].cap_info.Ibss)
	    currbss_type = BSS_INDEPENDENT;
	   else
		currbss_type = BSS_INFRASTRUCTURE;
	   return i;
	 }
  }
  return 0;
}

/**
 * Finds a BSSID in the BSS_List
 * 
 * @return 
 */
int	FindBSSIDinList(void)
{
  int i;

  for (i = 1 ; i <= num_bss; i++) {
	 if(memcmp(&specificBSSID,(char *)BSS_List[i].BssId,6) == 0)
	 {
       curr_bss = i;
	   return i;
	 }
  }
  return 0;
}

/******************************************************************************
 *
 * Name: userif_prepare_scan_cmd
 *
 * Description:
 * 	 This routine is used to prepare the scan command
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

void userif_prepare_scan_cmd(int ssid_scan)
{
    MrvlIEGeneric_t *   gentlv;
	uint8 * pos;
    uint32 size;
	IEEEtypes_ScanCmd_t * scancmd;
	channelInfo_t   * chan_info;
    int i;
    cmd_in_progress = 1;
    ((host_MsgHdr_t*)cmd_buffer)->Msg = host_CMD_802_11_SCAN;
	pos = (uint8 *)cmd_buffer;
    pos += sizeof(host_MsgHdr_t);
    size = sizeof(host_MsgHdr_t);

    scancmd = (IEEEtypes_ScanCmd_t *) pos;
    scancmd->BssType = bss_type;
	memset((void *)scancmd->BssId, 0, sizeof(IEEEtypes_MacAddr_t));

	pos += sizeof(IEEEtypes_Bss_e) + sizeof(IEEEtypes_MacAddr_t);
	size += sizeof(IEEEtypes_Bss_e) + sizeof(IEEEtypes_MacAddr_t);
    send_auth = ssid_scan;

	if(send_auth == 1) {
      gentlv = (MrvlIEGeneric_t *) pos;
      gentlv->hdr.Type = MRVL_SSID_TLV_ID;
      gentlv->hdr.Length = specificSSID.Len;

	  pos += sizeof(gentlv->hdr);
      memcpy(pos, (void *)specificSSID.SsId, gentlv->hdr.Length);
      pos += gentlv->hdr.Length;
      size +=  sizeof(gentlv->hdr) + gentlv->hdr.Length;
	}
 
    gentlv = (MrvlIEGeneric_t *) pos;
    gentlv->hdr.Type = MRVL_CHANNELLIST_TLV_ID;
    gentlv->hdr.Length = NUM_CHANNELS*sizeof(channelInfo_t);
	pos += sizeof(gentlv->hdr);
	for(i = 0; i < NUM_CHANNELS; i++) {
	chan_info = (channelInfo_t *)pos;
    chan_info->radioType = 0;
    chan_info->chanNum = i + 1; 
	chan_info->scanMode.passiveScan = 0;
	chan_info->scanMode.disableChanFilt = 1;
	chan_info->minScanTime = 0x20;
	chan_info->maxScanTime = 100;
    pos += sizeof(channelInfo_t);
	}

    size +=  sizeof(gentlv->hdr) + gentlv->hdr.Length;

    ((host_MsgHdr_t*)cmd_buffer)->Size = size;
	os_EventTrigger(sysinfo_CB_PROC_Q_EVENT, CBP_EV_HOST_OPC_RECEIVED);    
}

/******************************************************************************
 *
 * Name: userif_prepare_auth_cmd
 *
 * Description:
 * 	 This routine is used to prepare the authentication command
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

void userif_prepare_auth_cmd(void)
{
    host_802_11Authenticate_t      *auth_p;
    cmd_in_progress = 1;

	((host_MsgHdr_t*)cmd_buffer)->Msg = host_CMD_802_11_AUTHENTICATE;
	((host_MsgHdr_t*)cmd_buffer)->Size = sizeof(host_MsgHdr_t) + sizeof(host_802_11Authenticate_t);
	auth_p =  (host_802_11Authenticate_t *)((uint32)cmd_buffer + sizeof(host_MsgHdr_t));
	auth_p->AuthType = AUTH_OPEN_SYSTEM;
	auth_p->TimeOut  = 0;
	send_auth = 1;
	os_EventTrigger(sysinfo_CB_PROC_Q_EVENT, CBP_EV_HOST_OPC_RECEIVED);

}

/**
 * Prepares the deauth command
 */
void userif_prepare_deauth_cmd(void)
{
    IEEEtypes_DeauthCmd_t    *deauth_p;
    cmd_in_progress = 1;
    
	((host_MsgHdr_t*)cmd_buffer)->Msg = host_CMD_802_11_DEAUTHENTICATE;
	((host_MsgHdr_t*)cmd_buffer)->Size = sizeof(host_MsgHdr_t) + sizeof(IEEEtypes_DeauthCmd_t);
	deauth_p =  (IEEEtypes_DeauthCmd_t *)((uint32)cmd_buffer + sizeof(host_MsgHdr_t));
	deauth_p->Reason = 3;
    memcpy((void *)deauth_p->PeerStaAddr, (void *)BSS_List[curr_bss].BssId, sizeof(IEEEtypes_MacAddr_t));
	os_EventTrigger(sysinfo_CB_PROC_Q_EVENT, CBP_EV_HOST_OPC_RECEIVED);

}

/**
 * Prepares the adhoc stop command
 */
void userif_prepare_adhoc_stop_cmd(void)
{
    cmd_in_progress = 1;
    
	((host_MsgHdr_t*)cmd_buffer)->Msg = host_CMD_802_11_AD_HOC_STOP;
	((host_MsgHdr_t*)cmd_buffer)->Size = sizeof(host_MsgHdr_t);
	os_EventTrigger(sysinfo_CB_PROC_Q_EVENT, CBP_EV_HOST_OPC_RECEIVED);

}
/******************************************************************************
 *
 * Name: userif_prepare_assoc_cmd
 *
 * Description:
 * 	 This routine is used to prepare the association command
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *  None.
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

void userif_prepare_assoc_cmd(void)
{
    IEEEtypes_AssocCmd_ext_t           *assoc_p;
    MrvlIEGeneric_t *   gentlv;
	channelInfo_t   * chan_info;
	uint8 * pos;
    uint32 size;
    cmd_in_progress = 1;

    ((host_MsgHdr_t*)cmd_buffer)->Msg = host_CMD_802_11_ASSOCIATE;
    assoc_p =  (IEEEtypes_AssocCmd_ext_t *)((uint32)cmd_buffer + sizeof(host_MsgHdr_t));
    memcpy((void *)assoc_p->PeerStaAddr, (void *)BSS_List[curr_bss].BssId, sizeof(IEEEtypes_MacAddr_t));
	memcpy((void *)&assoc_p->CapInfo,(void *)&BSS_List[curr_bss].cap_info,sizeof(IEEEtypes_CapInfo_t));
    assoc_p->ListenInterval = 0x100;
	assoc_p->BcnPeriod   = BSS_List[curr_bss].bcn_interval;
	assoc_p->DtimPeriod  = 0x10;
	pos = (uint8 *)cmd_buffer;
    size = sizeof(host_MsgHdr_t) +
		   sizeof(IEEEtypes_MacAddr_t) +
		   sizeof(IEEEtypes_CapInfo_t) +
		   sizeof(IEEEtypes_ListenInterval_t) +
		   sizeof(IEEEtypes_BcnInterval_t) +
		   sizeof(IEEEtypes_DtimPeriod_t);
    pos += size;

    gentlv = (MrvlIEGeneric_t *) pos;
    gentlv->hdr.Type = MRVL_SSID_TLV_ID;
    gentlv->hdr.Length = BSS_List[curr_bss].ssid_element.Len;
	pos += sizeof(gentlv->hdr);
    memcpy(pos, (void *)BSS_List[curr_bss].ssid_element.SsId, gentlv->hdr.Length);
    pos += gentlv->hdr.Length;
    size +=  sizeof(gentlv->hdr) + gentlv->hdr.Length;

    gentlv = (MrvlIEGeneric_t *) pos;
    gentlv->hdr.Type = MRVL_PHYPARAMDSSET_TLV_ID;
    gentlv->hdr.Length = 1;
	pos += sizeof(gentlv->hdr);
    pos[0] = BSS_List[curr_bss].Channel;
    pos += gentlv->hdr.Length;
    size +=  sizeof(gentlv->hdr) + gentlv->hdr.Length;


    gentlv = (MrvlIEGeneric_t *) pos;
    gentlv->hdr.Type = MRVL_RATES_TLV_ID;
    gentlv->hdr.Length = G_SUPPORTED_RATES;
	pos += sizeof(gentlv->hdr);
	memcpy(pos, SupportedRates, G_SUPPORTED_RATES);
	pos += gentlv->hdr.Length;
	size +=  sizeof(gentlv->hdr) + gentlv->hdr.Length;
	 
	gentlv = (MrvlIEGeneric_t *) pos;
	gentlv->hdr.Type = MRVL_CHANNELLIST_TLV_ID;
	gentlv->hdr.Length = sizeof(channelInfo_t);
	pos += sizeof(gentlv->hdr);
	chan_info = (channelInfo_t *)pos;
	memset((void *)chan_info, 0x00, sizeof(channelInfo_t));
	chan_info->chanNum = BSS_List[curr_bss].Channel; 
 	pos += gentlv->hdr.Length;
	size +=  sizeof(gentlv->hdr) + gentlv->hdr.Length;

	((host_MsgHdr_t*)cmd_buffer)->Size = size;
	os_EventTrigger(sysinfo_CB_PROC_Q_EVENT, CBP_EV_HOST_OPC_RECEIVED);
}

/******************************************************************************
 *
 * Name: userif_prepare_adhoc_join_cmd
 *
 * Description:
 * 	 This routine is used to prepare the adhoc join command
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *  None.
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

void userif_prepare_adhoc_join_cmd(void)
{
    IEEEtypes_BssDesc_t           *bssd_p;
    cmd_in_progress = 1;

    ((host_MsgHdr_t*)cmd_buffer)->Msg = host_CMD_802_11_AD_HOC_JOIN;
    bssd_p =  (IEEEtypes_BssDesc_t *)((uint32)cmd_buffer + sizeof(host_MsgHdr_t));
    memcpy((void *)bssd_p->BssId, (void *)BSS_List[curr_bss].BssId, sizeof(IEEEtypes_MacAddr_t));
    bssd_p->BssType = BSS_INDEPENDENT;
	bssd_p->BcnPeriod   = BSS_List[curr_bss].bcn_interval;
	memcpy((void *)&bssd_p->Cap,(void *)&BSS_List[curr_bss].cap_info,sizeof(IEEEtypes_CapInfo_t));
	bssd_p->PhyParamSet.DsParamSet.ElementId = DS_PARAM_SET;    
	bssd_p->PhyParamSet.DsParamSet.Len = 1;    
	bssd_p->PhyParamSet.DsParamSet.CurrentChan = BSS_List[curr_bss].Channel;
    bssd_p->SsParamSet.IbssParamSet.ElementId = IBSS_PARAM_SET;
    bssd_p->SsParamSet.IbssParamSet.Len = 2;
	if(BSS_List[curr_bss].AtimWindow)
      bssd_p->SsParamSet.IbssParamSet.AtimWindow = BSS_List[curr_bss].AtimWindow;
	else
	  bssd_p->SsParamSet.IbssParamSet.AtimWindow = 0x10;
	
	memset((void *)bssd_p->SsId,0,32);
    memcpy((void *)bssd_p->SsId,(void *)BSS_List[curr_bss].ssid_element.SsId,BSS_List[curr_bss].ssid_element.Len);
    memcpy((void *)bssd_p->DataRates, SupportedRates, G_SUPPORTED_RATES);

	((host_MsgHdr_t*)cmd_buffer)->Size = sizeof(host_MsgHdr_t) + sizeof(IEEEtypes_BssDesc_t);
	os_EventTrigger(sysinfo_CB_PROC_Q_EVENT, CBP_EV_HOST_OPC_RECEIVED);
}

/******************************************************************************
 *
 * Name: userif_prepare_adhoc_start_cmd
 *
 * Description:
 * 	 This routine is used to prepare the adhoc start command
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *  None.
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

void userif_prepare_adhoc_start_cmd(void)
{
    smeQ_AdhocStartCmd_t           *start_p;
    cmd_in_progress = 1;

    ((host_MsgHdr_t*)cmd_buffer)->Msg = host_CMD_802_11_AD_HOC_START;
    start_p =  (smeQ_AdhocStartCmd_t *)((uint32)cmd_buffer + sizeof(host_MsgHdr_t));
    start_p->BssType = BSS_INDEPENDENT;
	start_p->BcnPeriod   = 0;
	start_p->DtimPeriod  = 0x10;
	memset((void *)&start_p->CapInfo,0,sizeof(IEEEtypes_CapInfo_t));
	start_p->CapInfo.Ess    = 0x0;
    start_p->CapInfo.Ibss    = 0x1;
	start_p->CapInfo.ShortPreamble = 0x0;
    start_p->CapInfo.ShortSlotTime = 0x0;
	start_p->PhyParamSet.DsParamSet.ElementId = DS_PARAM_SET;
	start_p->PhyParamSet.DsParamSet.Len = 1;    
	start_p->PhyParamSet.DsParamSet.CurrentChan = 11;
	memset((void *)start_p->SsId,0,32);
    memcpy((void *)start_p->SsId,(void *)specificSSID.SsId,specificSSID.Len);
    start_p->OpRateSet[0] = 0;
    start_p->SsParamSet.IbssParamSet.ElementId = IBSS_PARAM_SET;
    start_p->SsParamSet.IbssParamSet.Len = 2;
    start_p->SsParamSet.IbssParamSet.AtimWindow = 0x10;
    currbss_type = BSS_INDEPENDENT;
	((host_MsgHdr_t*)cmd_buffer)->Size = sizeof(host_MsgHdr_t) + sizeof(IEEEtypes_BssDesc_t);
	os_EventTrigger(sysinfo_CB_PROC_Q_EVENT, CBP_EV_HOST_OPC_RECEIVED);
}

#ifdef EMBEDDED_TCPIP
/******************************************************************************
 *
 * Name: userif_prepare_config_etcp
 *
 * Description:
 * 	 This routine is used to prepare the command to configure the ETCP
 * stack.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

void userif_prepare_config_etcp(void) 
{
    TCP_CNFG  *config;
    cmd_in_progress = 1;

	((host_MsgHdr_t*)cmd_buffer)->Msg = host_CMD_TCP;
	((host_MsgHdr_t*)cmd_buffer)->Size = sizeof(host_MsgHdr_t) + sizeof(TCP_CNFG);

	/* Initialize Treck stack with the econfig command */
    config = (TCP_CNFG  *) ((uint32)cmd_buffer + sizeof(host_MsgHdr_t));
	config->uSubType = E_TCP_CMD_Configure;
    config->subCmd.stConfigure.tTreckConfig = 0;
  	config->subCmd.stConfigure.uUseDHCP = 0;
    memcpy((void *)config->subCmd.stConfigure.aIPaddr, (void *)ip_addr, 4);
    memcpy((void *)config->subCmd.stConfigure.aNetMask, (void *)net_mask, 4);
	memcpy((void *)config->subCmd.stConfigure.aDefaultGateway, (void *)def_gtwy, 4);
	config->subCmd.stConfigure.uMultiHomeIndex = 0;
	HostSmeMsg_evt = ETCP_INIT_RESP;
	os_EventTrigger(sysinfo_CB_PROC_Q_EVENT, CBP_EV_HOST_OPC_RECEIVED);
}

/******************************************************************************
 *
 * Name: userif_prepare_open_raw_socket
 *
 * Description:
 * 	 This routine is used to prepare the command to open a raw socket with the
 *  ETCP stack
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

void userif_prepare_open_raw_socket(void) 
{
    TCP_CNFG  *config;
    cmd_in_progress = 1;
	((host_MsgHdr_t*)cmd_buffer)->Msg = host_CMD_TCP;
	((host_MsgHdr_t*)cmd_buffer)->Size = sizeof(host_MsgHdr_t) + sizeof(TCP_CNFG);

	/* Open raw socket for eping */
    config = (TCP_CNFG  *) ((uint32)cmd_buffer + sizeof(host_MsgHdr_t));
    config->uSubType = E_TCP_CMD_Socket;
    config->subCmd.stSocket.nSocketType = SOCK_RAW;
	config->subCmd.stSocket.nProtocol = IPPROTO_ICMP;
	HostSmeMsg_evt = RAW_SOCK_OPEN_RESP;
	os_EventTrigger(sysinfo_CB_PROC_Q_EVENT, CBP_EV_HOST_OPC_RECEIVED);
}

#endif
/******************************************************************************
 *
 * Name: userif_prepare_macaddr_get_cmd
 *
 * Description:
 *
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

void userif_prepare_macaddr_get_cmd(void) 
{
	uint16 *macaddr_cmd;
    cmd_in_progress = 1;
	((host_MsgHdr_t*)cmd_buffer)->Msg = host_CMD_802_11_MAC_ADDR;
	((host_MsgHdr_t*)cmd_buffer)->Size = sizeof(host_MsgHdr_t) +sizeof(UINT16)+ sizeof(IEEEtypes_MacAddr_t);

	/* Fill MacAddress Get Command */
    macaddr_cmd = (uint16  *) ((uint32)cmd_buffer + sizeof(host_MsgHdr_t));
    *(macaddr_cmd) = host_ACT_READ;
	HostSmeMsg_evt = GET_MACADDR_RESP;
	os_EventTrigger(sysinfo_CB_PROC_Q_EVENT, CBP_EV_HOST_OPC_RECEIVED);
}

/******************************************************************************
 *
 * Name: process_cbproc_response
 *
 * Description:
 *
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

void process_cbproc_response(void) {
    w81cbProcQ_ScanRsp_t * scan_rsp;
	uint8 * macaddr;
	uint16 assoc_resp;
    int MsgType;

	MsgType = ((host_MsgHdr_t *)cmd_res_buffer)->Msg;

	switch (MsgType) {
	case host_RET_802_11_SCAN:
	 scan_rsp = (w81cbProcQ_ScanRsp_t *)((uint32)cmd_res_buffer + sizeof(host_MsgHdr_t));
	 if (scan_rsp->NumSets) {
		parse_scan_result(scan_rsp);
	 } else {
		 curr_bss = 0;
	 }
	 HostSmeMsg_evt = SCAN_RESP;
	 if(!send_auth) {
		 cmd_in_progress = 0;
	 }

	 break;
	case host_RET_802_11_MAC_ADDR:
		macaddr = (uint8 *) ((uint32)cmd_buffer + sizeof(host_MsgHdr_t) + sizeof(UINT16));
		memcpy((void *)STAMACAddr , (void *)macaddr, 6);
		cmd_in_progress = 0;
        break;
	case host_RET_802_11_ASSOCIATE:
		assoc_resp =  *(UINT16 *)((uint32)cmd_res_buffer + sizeof(host_MsgHdr_t) + sizeof(IEEEtypes_CapInfo_t));
    	if(assoc_resp){
		 HostSmeMsg_evt = ASSOC_RESP_FAILED;
		} else {
			HostSmeMsg_evt = ASSOC_RESP;
			if(use_peer_sta_api) {
			memcpy((void *)specificBSSID, (void *)BSS_List[curr_bss].BssId, sizeof(IEEEtypes_MacAddr_t));
			peersta_AddSTAEntry(specificBSSID,2,eping_process_pkt);
			}
			link_present = 1;
		}
		cmd_in_progress = 0;
		break;
	case host_RET_802_11_AD_HOC_START:
		HostSmeMsg_evt = ADHOC_START_RESP;
	case host_RET_802_11_AD_HOC_JOIN:
		HostSmeMsg_evt = ADHOC_JOIN_RESP;        
		if(use_peer_sta_api) {
 		 memcpy((void *)specificBSSID, (void *)BSS_List[curr_bss].BssId, sizeof(IEEEtypes_MacAddr_t));
		 peersta_AddSTAEntry(specificBSSID,2,eping_process_pkt);
		}
		link_present = 1;
		cmd_in_progress = 0;
		break;
	case host_RET_802_11_DEAUTHENTICATE:
	case host_RET_802_11_AD_HOC_STOP:
		userif_prepare_scan_cmd(1);
		HostSmeMsg_evt = DEAUTH_RESP;		
//		cmd_in_progress = 0;
		break;
	case host_RET_802_11_AUTHENTICATE:
		HostSmeMsg_evt = AUTH_RESP;
		break;
	default:
		cmd_in_progress = 0;
		break;
	}
}



/******************************************************************************
 *
 * Name: UserIF_Main
 *
 * Description:
 * 	 This is the main routine which sends command to the wlan driver to
 *   establish a connection, it also configures the ETCP stack. Upon receiving the
 *   user input it sends ping packets to the given ip address
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

void UserIF_Main(ULONG data)
{
	UINT32	 Events;
	print_usage();
    /* Main Loop*/
    while(1) {
		  /* First time also Wait till FW is ready*/
        Events = os_EventWait(sysinfo_NO_HOST_EVENT,
							NOHOST_EVENT_TRIGGERS,
							os_EVENT_WAITMODE_CLR_OR);
 
		if(Events & FW_READY_EVENT)
		{
			HostSmeMsg_evt = FW_READY;	
		}

		if(Events & MAC_EVENT)
		{
			link_present = 0;
			HostSmeMsg_evt = LINK_LOST;			
		}

		if(Events & REASSOC_TIMEOUT_EVENT)
		{
			HostSmeMsg_evt = REASSOC_TIMEOUT;			
		}
		if(Events & NO_HOST_EVENT)
		{
		  /* In case bad response redo everything*/
		  if (((host_MsgHdr_t *)cmd_res_buffer)->Result != host_RESULT_OK)
		  {
			  cmd_in_progress = 0;
			  HostSmeMsg_evt = RESTART;			  
		  } else {
			  process_cbproc_response();
		  }
		}
		HostSmeState_Handler(HostSmeMsg_evt);
  }
}


/******************************************************************************
 *
 * Name: userif_init
 *
 * Description:
 * 	 This routine is used to initialize the eping parameters and the process
 *   which interacts with the user.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

Status_e  userif_init(void)
{

	int i;

	os_EventInit((os_Id_t)sysinfo_NO_HOST_EVENT);

	// initialise stack buffer for Eping task
	for (i = 0; i < sizeof(UserIFstack); UserIFstack[i++] = 0xff);

	// create Eping task
	if (tx_thread_create(&stUserIFThread,
			 	"UserIFTask",
				UserIF_Main,
				(UINT32)NULL,
				(VOID *)&UserIFstack[0],
				sizeof(UserIFstack),
				9,
				9,
				29,
				TX_AUTO_START) != TX_SUCCESS) {
		return FAIL;
	}

	/* Get the buffer used to send commands*/
    cmd_buffer = (uint8*) mli_GetCmdReqBuffer();
	/* Get the buffer used to receive commands*/
	cmd_res_buffer = (uint8*) mli_GetCmdResBuffer();
    /* Change print level to 0*/
	dbg_ChangePrintLevel(0);
	/* Initialize Host SME state machine */
	HostSMESmCtor();
	/* Install the eping periodic handler*/ 
    send_ping = 0;
	link_present = 0;
    use_peer_sta_api = 0;
	cmd_in_progress = 0;
	bss_type = BSS_ANY;
#ifdef EMBEDDED_TCPIP
	mli_installTickFunction(frameICMPPacket);
#else
	mli_installTickFunction(send_raw_pkt);
#endif
	mli_installTickFunction(cmd_parser); 	
    return SUCCESS;
}

/******************************************************************************
 *
 * Name: parse_beacon
 *
 * Description:
 * 	 parses the BSS beacon
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   beacon
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

void parse_beacon(char *bptr, int bsize)
{
  IEEEtypes_IE_Param_t * gentlv;
  IEEEtypes_SsIdElement_t * ssid_element;
  IEEEtypes_DsParamSet_t  *pDS;
  IEEEtypes_IbssParamSet_t  *pIbss;
  IEEEtypes_RSN_IE_t *pIe;
  const uint8 oui01[4] = { 0x00, 0x50, 0xf2, 0x01 };
  int privacy;
  int element_size;
  privacy = 0;
  if (!send_auth) DBG_P(( DBG_L0 "%2x. ", num_bss));
  /* Parse the fixed length fields */
  memcpy((void *)(BSS_List[num_bss].BssId),bptr,sizeof(IEEEtypes_MacAddr_t));
  if (!send_auth) DBG_P(( DBG_L0 "BSSID %02x:%02x:%02x:%02x:%02x:%02x ",bptr[0],bptr[1],bptr[2],bptr[3],bptr[4],bptr[5]));
  bptr += sizeof(IEEEtypes_MacAddr_t);
  bsize -= sizeof(IEEEtypes_MacAddr_t);
  if (!send_auth) DBG_P(( DBG_L0 "RSSI %d ",*(uint8 *)bptr));
  bptr += sizeof(uint8) + sizeof(IEEEtypes_TimeStamp_t);
  BSS_List[num_bss].bcn_interval = *(uint16 *)bptr;
  bptr += sizeof(IEEEtypes_BcnInterval_t);
  bsize -= sizeof(IEEEtypes_BcnInterval_t);

  memcpy((void *)&BSS_List[num_bss].cap_info, (void *)bptr, sizeof(IEEEtypes_CapInfo_t));
  bptr += sizeof(IEEEtypes_CapInfo_t) ;
  bsize += sizeof(IEEEtypes_CapInfo_t);
  if(BSS_List[num_bss].cap_info.Privacy == 1) {
	  privacy = 1;
  }
  ssid_element = (IEEEtypes_SsIdElement_t *)bptr;
  BSS_List[num_bss].ssid_element.Len = ssid_element->Len;
  memcpy((void *)BSS_List[num_bss].ssid_element.SsId, (void *)(ssid_element->SsId), ssid_element->Len);
  BSS_List[num_bss].ssid_element.SsId[ssid_element->Len] = '\0';
  if (!send_auth) DBG_P(( DBG_L0 "SSID %20s\t", BSS_List[num_bss].ssid_element.SsId));

  bptr += 2 + ssid_element->Len;
  bsize -= 2 + ssid_element->Len;

  /* Parse the variable fields*/
  while(bsize > 2) {
   gentlv =  (IEEEtypes_IE_Param_t *)bptr;
   element_size = gentlv->Len;
   switch(gentlv->ElementId) {
        case MRVL_SSID_TLV_ID:
            break;

        case MRVL_RATES_TLV_ID:
            break;

        case MRVL_PHYPARAMFHSET_TLV_ID:
            break;

        case MRVL_PHYPARAMDSSET_TLV_ID:
            pDS = (IEEEtypes_DsParamSet_t *) bptr;
            BSS_List[num_bss].Channel = pDS->CurrentChan;
            break;

        case MRVL_CFPARAMSET_TLV_ID:
            break;

        case MRVL_IBSSPARAMSET_TLV_ID:
            pIbss = (IEEEtypes_IbssParamSet_t *) bptr;
            BSS_List[num_bss].AtimWindow = pIbss->AtimWindow;
            break;

        case MRVL_COUNTRY_TLV_ID:
            break;

		case VENDOR_SPECIFIC_221:
            pIe = (IEEEtypes_RSN_IE_t *) bptr;
            if (!memcmp((char *)pIe->OuiType, oui01, sizeof(oui01))) {
				privacy = 2;
			}
#ifdef SDK_FEATURE_XX
			else {
				VendorIEMac_t * pTempMac;
                UINT32 uTemp = MRVL_MAC_ADDR_OUI;

				pTempMac = (VendorIEMac_t *) bptr;
				if( !memcmp((void*)&pTempMac->Payload.uOUI, (void*)&uTemp,4))
					DBG_P(( DBG_L0 "MAC ADDR %02x:%02x:%02x:%02x:%02x:%02x ",
											pTempMac->Payload.Mac[0], pTempMac->Payload.Mac[1],
											pTempMac->Payload.Mac[2], pTempMac->Payload.Mac[3],
											pTempMac->Payload.Mac[4], pTempMac->Payload.Mac[5]));
			}
#endif
			break;

		case RSN_IEWPA2:
            privacy = 3;
            break;
        default:
            break;
        }

   bsize -= (element_size + 2);
   bptr  += (element_size + 2);
  }
  if (!send_auth) {
   if (privacy == 3) {
      DBG_P(( DBG_L0 "Security: WPA2\r\n"));
   } else if (privacy == 2) {
	  DBG_P(( DBG_L0 "Security: WPA\r\n"));
   } else if (privacy  == 1) {
	  DBG_P(( DBG_L0 "Security: WEP\r\n"));
   } else {
	  DBG_P(( DBG_L0 "Security: None\r\n"));
   }
  }

}

/******************************************************************************
 *
 * Name: parse_scan_result
 *
 * Description:
 * 	 Parses the BSSs one by one
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   scan response
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

void parse_scan_result(w81cbProcQ_ScanRsp_t *scan_rsp)
{
	int i;
	unsigned short beacon_size;
	char * bptr;
	num_bss = 0;
#ifdef UART_DRV
	         DBG_P(( DBG_L0 "\r\n")); 
#endif

	bptr = (char *)scan_rsp->BssDescAndTlvBuffer;
	for(i = 0 ; (i < scan_rsp->NumSets) && (i < MAX_BSS_HANDLES); i++) {
        beacon_size = *bptr;
		bptr += 2;
		/* Process here */
		num_bss++;
		parse_beacon(bptr, beacon_size);
        bptr += beacon_size;
	}
}

