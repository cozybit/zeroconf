/*! \file mdns_responder.h
 *  \brief mDNS resoponder thread
 ******************************************************************************/

#ifndef _MDNS_RESPONDER_H_
#define _MDNS_RESPONDER_H_

#include "system.h"

sys_status mdns_responder_init(void);
sys_status mdns_responder_shutdown(void);

#endif
