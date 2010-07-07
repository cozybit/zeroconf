#ifndef MDNS_H
#define MDNS_H

#include <stdint.h>

/* Build options
 *
 * A number of build options are available in order to optimize footprint, etc.
 * These are conventially provided from the makefile as opposed to being
 * defined directly in a header file.  They include:
 *
 * MDNS_CHECK_ARGS: define this to enable various error checking on the input.
 * Developers may wish to enable this during development to ensure that various
 * inputs such as host names are legal.  Then, if the inputs are not going to
 * change, this option can be turned off to save code space.
 *
 * MDNS_LOG: define this to include logging.
 *
 * MDNS_DBG: define this to include debug-level logging.  This has no effect if
 * MDNS_LOG is not defined.
 */

#define MDNS_CTRL_PORT 12345

/* error codes */
#define MDNS_INVAL		2	/* invalid argument */

/* mdns_launch: start the mdns thread
 *
 * ipaddr: ip address to bind to and to associate with all names and services.
 * The address must be in network byte order.
 *
 * domain: pointer to null-terminated domain.  Most users will want to set this
 * argument to "local".
 *
 * hostname: pointer to a null-terminated string that is the hostname to
 * resolve.
 *
 */
int mdns_launch(uint32_t ipaddr, char *domain, char *hostname);

void mdns_halt(void);

#endif
