/*
 * Copyright (C) cozybit, Inc. 2007-2010. All Rights Reserved.
 *
 * Use and redistribution subject to licensing terms.
 */
#ifndef MDNS_H
#define MDNS_H

#include <mdns_port.h>

/* Build options
 *
 * A number of build options are available in order to optimize footprint,
 * specify the target system, etc.  These are conventially provided from the
 * makefile as opposed to being defined directly in a header file.  They
 * include:
 *
 * MDNS_SYSTEM: This must be set to the target system for which mdns is being
 * compiled.  For example, specify "MDNS_SYSTEM=LINUX" for a linux target.
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
 *
 * MDNS_TESTS: define this to compile the internal tests.  In this case, you
 * can invoke mdns_tests() to run the tests.  Expect test results to be printed
 * using mdns_log().
 */

/* mdns control scoket port
 *
 * mdns uses a control socket to communicate between the mdns thread and any
 * API calls.  This control socket is actually a UDP socket on the loopback
 * interface.  Developers who wish to specify a certain port for this control
 * socket can do so by changing MDNS_CTRL_PORT.
 */
#define MDNS_CTRL_PORT 12345

/* Maximum length of labels
 *
 * A label is one segment of a DNS name.  For example, "foo" is a label in the
 * name "foo.local.".  RFC 1035 requires that labels do not exceed 63 bytes.
 */
#define MDNS_MAX_LABEL_LEN	63	/* defined by the standard */

/* Maximum length of names
 *
 * A name is a list of labels such as "My Webserver.foo.local" or
 * mydevice.local.  RFC 1035 requires that names do not exceed 255 bytes.
 */
#define MDNS_MAX_NAME_LEN	255	/* defined by the standard */

/* return codes */
#define MDNS_SUCCESS	0
#define MDNS_INVAL		1	/* invalid argument */
#define MDNS_BADSRV		2	/* bad service descriptor */
#define MDNS_TOOBIG		3	/* not enough room for everything */

/* service descriptor
 *
 * Central to mdns is the notion of a service.  Hosts advertise service types
 * such as a website, a printer, some custom service, etc.  Network users can
 * use an mdns browser to discover services on the network.  Internally, this
 * mdns implementation uses the following struct to describe a service.  It has
 * the following members:
 *
 * servname: string that is the service instance name that will be advertised.
 * It could be something like "Brian's Website" or "Special Service on Device
 * #123".  This is the name that is typically presented to users browsing for
 * your service.  The servname must not exceed MDNS_MAX_LABEL_LEN bytes.  The
 * MDNS specification allows servname to be a UTF8 string.  However, only the
 * ascii subset of UTF-8 has been tested.
 *
 * servtype: string that represents the service type.  This should be a type
 * registered at http://dns-sd.org/ServiceTypes.html.  For example, "http" is
 * the service type for a web server and "ssh" is for an ssh server.  You may
 * use an unregisterd service type during development, but not in released
 * products.  Consider registering any new service types at the aforementioned
 * webpage.  servtype must be non-NULL.
 *
 * port: the tcp or udp port on which the service named servname is available.
 *
 * proto: Either MDNS_PROTO_TCP or MDNS_PROTO_UDP depending on what protocol
 * clients should use to connect to the service servtype.
 *
 * keys: NULL-terminated array of key strings.  These are the keys of the TXT
 * key/value pairs for a service type.  For example, the servtype is "http"
 * defines the TXT keys "u", "p", and "path" for the username, password, and
 * path to a document.  If you supplied all of these, the keys array would be
 * {"u", "p", "path", NULL}.  For each element of keys, there must be a value
 * in values.  If keys is NULL, no TXT keys will be advertised.  If keys is
 * {NULL}, a TXT record will appear, but it will not contain any key/value
 * pairs.
 *
 * values: NULL-terminated array of value strings associated with keys.  In the
 * example above, an array something like {"myusername", "mypassword",
 * "/index.html", NULL} would be passed in.
 */
struct mdns_service
{
    char *servname;
    char *servtype;
    uint16_t port;
    int proto;
    char **keys;
    char **values;
};

/* protocol values for the proto member of the mdns_service descriptor */
#define MDNS_PROTO_TCP 0
#define MDNS_PROTO_UDP 1

/* mdns_launch: start the mdns thread
 *
 * ipaddr: ip address to bind to and to associate with all names and services.
 * The address must be in network byte order.
 *
 * domain: domain name string.  If this value is NULL, the domain ".local" will
 * be used.  The domain must not exceed MDNS_MAX_LABEL_LEN bytes.
 *
 * hostname: string that is the hostname to resolve.  This would be the "foo"
 * in "foo.local", for example.  This value must not be NULL.  The hostname
 * must not exceed MDNS_MAX_LABEL_LEN bytes.
 *
 * services: A NULL-terminated list of pointers to services to advertise.  If
 * this value is NULL, no service will be advertized.  Only name resolution
 * will be provided.
 *
 * mdns_launch returns one of the following return codes:
 *
 * MDNS_SUCCESS: success
 *
 * MDNS_INVAL: input was invalid.  Perhaps a label exceeded MDNS_MAX_LABEL_LEN,
 * or a name composed of the supplied labels exceeded MDNS_MAX_NAME_LEN.
 *
 * MDNS_BADSRV: one of the service descriptors in the services list was
 * invalid.
 *
 * MDNS_TOOBIG: The combination of name information and service descriptors
 * does not fit in a single packet, which is required by this implementation.
 *
 * Notes:
 *
 * The domain, hostname, and struct mdns_service elements must persist and
 * remain unchanged between calls to mdns_launch and mdns_halt.
 *
 * While mdns_launch returns immediately, the hostname and any servnames may
 * not be unique on the network.  In the event of a conflict, the names will
 * appended with an integer.  For example, if the hostname "foo.local" is
 * taken, mdns will attempt to claim "foo-2.local", then foo-3.local, and so on
 * until the conflicts cease.  If mdns gets all the way to foo-9.local and
 * still fail, it waits for 5 seconds (per the mDSN specification) and then
 * starts back at foo.local.  If a developer anticipates a network to have many
 * of her devices, she should devise a sensible scheme outside of mdns to
 * ensure that the names are unique.
 */
int mdns_launch(uint32_t ipaddr, char *domain, char *hostname,
                struct mdns_service **services);

/* halt the mdns thread
 */
void mdns_halt(void);

/* mdns_tests: run internal mdns tests
 *
 * This function is useful to verify that various internal mdns functionality
 * is properly working after a port, or after adding new features.  If
 * MDNS_TEST is undefined, it is an empty function.  It is not meant to be
 * compiled or run in a production system.  Test output is written using
 * mdns_log().
 */
void mdns_tests(void);

#endif
