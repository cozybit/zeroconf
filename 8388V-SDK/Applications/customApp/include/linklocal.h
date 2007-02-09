/*! \file linklocal.h
 *  \brief link-local thread
 ******************************************************************************/

#ifndef _LINKLOCAL_H_
#define _LINKLOCAL_H_

#include "system.h"

/** launches link-local address thread.  
 *
 * If link-local addressing is to be used, link-local manager should be called 
 * prior to treck stack initialization.  Otherwise, the Treck tcpip handler 
 * will consume ARP packets before the link-local manager ever sees them.  
 * Applications should poll the state variable treck_ready to know if the 
 * treck stack is up.
 *
 * \retval sys_status SYS_SUCCESS for success, other for failure.
 */
sys_status ll_init(void);
/** shuts down link-local thread
 *
 * \retval sys_status SYS_SUCCESS for success, other for failure. */
sys_status ll_shutdown(void);

#endif
