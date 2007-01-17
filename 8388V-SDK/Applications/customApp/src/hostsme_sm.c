/******************** (c) Marvell Semiconductor, Inc., 2006 *******************
 *
 * $Header:$
 *
 * Purpose:
 *    Host SME statemachine Implementation
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
#include "os_if.h"
#include "mac_events.h"
#include "timer.h"
#include "hostsme_sm.h"
#include "dbg_Facility.h"
#include "IEEE_types.h"
#include "userif.h"
#define assert(x)

extern IEEEtypes_Bss_e  bss_type;
extern BSSDescriptor_t     BSS_List[20];
extern int send_auth;
extern int cmd_in_progress;
/**
 */
extern char curr_bss;

HostSMESmState hostsme_state;

/*  Timer for ReAssociation */
static Timer reassocTimer = {NULL, 0};
#define REASSOC_TIME       500

void HostSmeState_Handler(uint32 evt)
{
  switch(hostsme_state){
   case DISASSOC:
    switch (evt)
    {
	case FW_READY: 	
	case RESTART:
        return ;
	case SCAN_RESP:
		if(send_auth){
		  if (FindSSIDinList()) {
			 if (BSS_List[curr_bss].cap_info.Ibss) 
	           userif_prepare_adhoc_join_cmd();
		     else
		       userif_prepare_auth_cmd();
		  } else {
			if(bss_type == BSS_INDEPENDENT)
			  userif_prepare_adhoc_start_cmd();
			else
			  cmd_in_progress = 0;
		  }
		}
        return ;
	case AUTH_RESP:
	case REASSOC_TIMEOUT:	  		
		if (BSS_List[curr_bss].cap_info.Ibss) 
		  userif_prepare_adhoc_join_cmd();
		else
		  userif_prepare_assoc_cmd();	  
        return ;
	case ADHOC_START_RESP:
	case ADHOC_JOIN_RESP:
	case ASSOC_RESP:
        hostsme_state = CONNECTED;
		return ;
	case ASSOC_RESP_FAILED:
		if(reassocTimer.active)
		{
			TimerRearm(&reassocTimer, REASSOC_TIME);
		}
		else
		{
			TimerInit(&reassocTimer);
			TimerFireIn(&reassocTimer, 1, (void *)&userif_ReAssocActTimeOut, NULL, REASSOC_TIME );
		}
		return ;
    default:
        return ;
    }
   case CONNECTED:
    switch (evt)
    {
    case LINK_LOST:
		/* Start the ReAssoc timer for Timeout period */
		if(reassocTimer.active)
		{
			TimerRearm(&reassocTimer, REASSOC_TIME);
		}
		else
		{
			TimerInit(&reassocTimer);
			TimerFireIn(&reassocTimer, 1, (void *)&userif_ReAssocActTimeOut, NULL, REASSOC_TIME );
		}
        hostsme_state = DISASSOC;
        return ;
	case RESTART:
	case DEAUTH_RESP:
        hostsme_state = DISASSOC; 
		return ;
    default:
        return ;
    }
  }
}

/**
 * Constructor for the Host SME statemachine
 * 
 * @param me
 */
void HostSMESmCtor(void)
{
  hostsme_state = DISASSOC;
}

