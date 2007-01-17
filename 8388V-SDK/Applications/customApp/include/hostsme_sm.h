/*******************************************************************
*
*    DESCRIPTION: hostsme_sm.h
*
*    AUTHOR:
*
*    HISTORY:
*
*    DATE:9/26/2006
*
*******************************************************************/
#ifndef HOST_SME_H
#define HOST_SME_H
 /** include files **/
#include "wltypes.h"
 #include "IEEE_types.h"
 /** local definitions **/
 /* SME statemachine events*/
enum HostSMESmEvents
{
	FW_READY,
	RESTART,
	SCAN_RESP,
	AUTH_RESP,
	ASSOC_RESP,    
	ASSOC_RESP_FAILED,
	DEAUTH_RESP,
    LINK_LOST,
    ADHOC_START_RESP,
	ADHOC_JOIN_RESP,
	GET_MACADDR_RESP,
	ETCP_INIT_RESP,
	RAW_SOCK_OPEN_RESP,
	REASSOC_TIMEOUT
};

/* BSS descriptor for storing the BSS information after scan*/
typedef struct _BSSDescriptor_t {
  IEEEtypes_MacAddr_t  BssId;
  IEEEtypes_BcnInterval_t bcn_interval;
  IEEEtypes_CapInfo_t     cap_info;
  IEEEtypes_SsIdElement_t ssid_element;
  UINT16                  AtimWindow;
  UINT8                   Channel;
} BSSDescriptor_t;

 /* default settings */
 
 /** external functions **/
 
 /** external data **/
 
 /** internal functions **/
 
 /** public data **/
typedef enum  {
   DISASSOC = 0,
   CONNECTED    
} HostSMESmState;
 
 /** private data **/
 
 /** public functions **/
void HostSMESmCtor(void);
void HostSmeState_Handler(uint32 evt);

 /** private functions **/
#endif
