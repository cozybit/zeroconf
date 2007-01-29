/******************************************************************************
 * mdns_responder.h
 *
 * 
 ******************************************************************************/

#ifndef _MDNS_RESPONDER_H_
#define _MDNS_RESPONDER_H_

#include "system.h"

/******************************************************************************
 * Public Functions
 ******************************************************************************/

sys_status mdns_responder_init(void);
sys_status mdns_responder_shutdown(void);

#endif
