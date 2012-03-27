/*
 * Copyright (C) 2007-2011 cozybit Inc.
 *
 * This file is part of libmdns.
 *
 * libmdns is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * libmdns is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with libmdns. If not, see <http://www.gnu.org/licenses/>.
 *
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
 * MDNS_SYSTEM_*: You must define the target system on which mdns will run.
 * For example, specify "MDNS_SYSTEM_LINUX" for a linux target.  Be sure to
 * ONLY define one MDNS_SYSTEM.
 *
 * MDNS_ENDIAN_BIG|MDNS_ENDIAN_LITTLE: You must define exactly one of these to
 * specify the endianness of your target.
 *
 * MDNS_QUERY_API: Developers who require the ability to query for services as
 * opposed to just responding to such queries should define MDNS_QUERY_API.
 * This enables the mdns_query_* functions described below.  These functions
 * will return MDNS_NOIMPL if the mdns library was built without MDNS_QUERY_API
 * defined.
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
 * MDNS_SERVICE_CACHE_SIZE: This is the maximum number of service instances
 * that can be monitored.  It defaults to 16.  See the section below on "mdns
 * query callback" for details on how to tune this value, and field
 * implications of the value of this value.
 *
 * MDNS_MAX_SERVICE_MONITORS: This is the maximum number of service types that
 * the system can monitor.  If an application only needs to monitor one type of
 * service, this should be 1 to save memory.  The default is 4.
 *
 * MDNS_TESTS: define this to compile the internal tests.  In this case, you
 * can invoke mdns_tests() to run the tests.  Expect test results to be printed
 * using mdns_log().
 */

/* mdns control socket ports
 *
 * mdns uses two control sockets to communicate between the mdns threads and
 * any API calls.  This control socket is actually a UDP socket on the loopback
 * interface.  Developers who wish to specify certain ports for this control
 * socket can do so by changing MDNS_CTRL_RESPONDER and MDNS_CTRL_QUERIER.
 */
#ifndef MDNS_CTRL_RESPONDER
#define MDNS_CTRL_RESPONDER 12345
#endif

#ifndef MDNS_CTRL_QUERIER
#define MDNS_CTRL_QUERIER  (MDNS_CTRL_RESPONDER + 1)
#endif

#ifndef MDNS_SERVICE_CACHE_SIZE
#define MDNS_SERVICE_CACHE_SIZE 16
#endif

#ifndef MDNS_MAX_SERVICE_MONITORS
#define MDNS_MAX_SERVICE_MONITORS 4
#endif

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

/* Maximum length of key/value pair
 *
 * TXT records associated with a service are populated with key/value pairs.
 * These key/value pairs must not exceed this length.
 */
#define MDNS_MAX_KEYVAL_LEN	255	/* defined by the standard */

/* return codes */
#define MDNS_SUCCESS	0
#define MDNS_INVAL		1	/* invalid argument */
#define MDNS_BADSRV		2	/* bad service descriptor */
#define MDNS_TOOBIG		3	/* not enough room for everything */
#define MDNS_NOIMPL		4	/* unimplemented feature */
#define MDNS_NOMEM		5	/* insufficient memory */
#define MDNS_INUSE		6	/* requested resource is in use */
#define MDNS_NORESP		7	/* no response */

/* service descriptor
 *
 * Central to mdns is the notion of a service.  Hosts advertise service types
 * such as a website, a printer, some custom service, etc.  Network users can
 * use an mdns browser to discover services on the network.  Internally, this
 * mdns implementation uses the following struct to describe a service.  These
 * structs can be created by a user, populated, and passed to mdns_launch to
 * specify services that are to be advertised.  When a user starts a query for
 * services, the discovered services are passed back to the user in this
 * struct.
 *
 * The members include:
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
 * domain: string that represents the domain.  This field is ignored by the
 * responder (i.e., in those mdns_services passed to mdns_launch).  In this
 * case, the domain passed to mdns_launch is used.  However, the domain is
 * valid for mdns_services that are passed to query callbacks.
 *
 * port: the tcp or udp port on which the service named servname is available
 * in network byte order.
 *
 * proto: Either MDNS_PROTO_TCP or MDNS_PROTO_UDP depending on what protocol
 * clients should use to connect to the service servtype.
 *
 * keyvals: NULL-terminated string of colon-separated key=value pairs.  These
 * are the key/value pairs for the TXT record associated with a service type.
 * For example, the servtype "http" defines the TXT keys "u", "p", and "path"
 * for the username, password, and path to a document.  If you supplied all of
 * these, the keyvals string would be:
 *
 * "u=myusername:p=mypassword:path=/index.html"
 *
 * If keyvals is NULL, no TXT record will be advertised.  If keyvals is ":", a
 * TXT record will appear, but it will not contain any key/value pairs.  The
 * key must be present (i.e., two contiguous ':' characters should never appear
 * in the keyvals string.)  A key may appear with no value.  The interpretation
 * of this depends on the nature of the service.  The length of a single
 * key/value pair cannot exceed MDNS_MAX_KEYVAL_LEN bytes.
 *
 * Note that the keyvals string of any struct mdns_services passed to
 * mdns_launch WILL be modified by mdns, and therefore must not be const (e.g.,
 * in ROM).  Further, the keyvals member should not be dereferenced after being
 * passed to mdns_launch.  On the contrary, struct mdns_services generated in
 * response to queries may be dereferenced and will have the afore-described
 * syntax.
 *
 * ipaddr: The IP address on which the service is available in network byte
 * order.  Note that this member need not be populated for services passed to
 * mdns_launch.  It is implied that these services are offered at the ip
 * address supplied as an explicit argument to the mdns_launch call.  However,
 * when a struct mdns_service is generated in response to a query, this member
 * will be populated.
 */
struct mdns_service
{
    char *servname;
    char *servtype;
	char *domain;
    uint16_t port;
    int proto;
    char *keyvals;
	uint32_t ipaddr;

	/* The following members are for internal use only and should not be
	 * dereferenced by the user.
	 */
	uint8_t fqsn[MDNS_MAX_NAME_LEN];
	uint8_t *ptrname;
	uint16_t kvlen;
	uint32_t flags;
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
 * in "foo.local", for example.  The hostname must not exceed
 * MDNS_MAX_LABEL_LEN bytes.  If hostname is NULL, the responder capability
 * will not be launched, and the services array will be NULL; only query
 * support will be enabled.  This is useful if only the query functionality is
 * desired.
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
 * Perhaps hostname was NULL and the query capability is not compiled.
 *
 * MDNS_BADSRV: one of the service descriptors in the services list was
 * invalid.  Perhaps one of the key/val pairs exceeded MDNS_MAX_KEYVAL_LEN.
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
 *
 * Any services being monitored will be unmonitored.
 */
void mdns_halt(void);

/* mdns query callback
 *
 * A user initiates a query for services by calling the mdns_query_monitor
 * function with a fully-qualified service type, an mdns_query_cb, and an
 * opaque argument.  When a service instance is discovered, the query callback
 * will be invoked with following arguments:
 *
 * data: a void * that was passed to mdns_query_monitor().  This can be
 * anything that the user wants, such as pointer to a custom internal data
 * structure.
 *
 * s: A pointer to the struct mdns_service that was discovered.  The struct
 * mdns_service is only valid until the callback returns.  So if attributes of
 * the service (such as IP address and port) are required by the user for later
 * use, they must be copied and preserved elsewhere.
 *
 * status: A code that reports the status of the query.  It takes one of the
 * following values:
 *
 * MDNS_DISCOVERED: The mdns_service s has just been discovered on the network
 * and will be monitored by the mdns stack.
 *
 * MDNS_UPDATED: The mdns_service s, which is being monitored, has been updated
 * in some way (e.g., it's IP address has changed, it's key/value pairs have
 * changed.)
 *
 * MDNS_DISAPPEARED: The mdns_service has left the network.  This usually
 * happens when a service has shut down, or when it has stopped responding
 * queries.  Applications may also detect this condition by some means outside
 * of mdns, such as a closed TCP connection.
 *
 * MDNS_CACHE_FULL: The mdns_service has been discovered.  However, the number
 * of monitored service instances has exceeded MDNS_SERVICE_CACHE_SIZE.  So the
 * returned mdns_service may not be complete.  See NOTES below on other
 * implications of an MDNS_CACHE_FULL status.
 *
 * NOTES:
 *
 * The query callback should always return MDNS_SUCCESS.  In the future, other
 * return codes may be developed to specify other conditions.
 *
 * Callback implementers must take care to not make any blocking calls, nor to
 * call any mdns API functions from within callbacks.
 *
 * Suppose MDNS_SERVICE_CACHE_SIZE is 16 and that a user has invoked
 * mdns_query_monitor to monitor services of type _http._tcp.local.  Further,
 * suppose that this particular domain has 17 instances of this type.  The
 * first 16 instances to be discovered will result in 16 callbacks with the
 * status MDNS_DISCOVERED.  These instances will be cached and monitored for
 * updates, disappearance, etc.  When the 17th instance is discovered, the
 * callback will be called as usual, but the status will be MDNS_CACHE_FULL,
 * and the service will not be monitored.  While a best effort is made to
 * deliver all of the service information, the mdns_service may be incomplete.
 * Specifically, the ipaddr may be 0 and the service name may be "".  Further,
 * the callback may be called again if the 17th instance of the service
 * announces itself on the network again.  If one of the other services
 * disappears, the next announcement from the 17th instance will result in a
 * callback with status MDNS_DISCOVERED, and from that point forward it will be
 * monitored.
 *
 * So what's the "best" value for MDNS_SERVICE_CACHE_SIZE?  This depends on the
 * application and on the field in which the application is deployed.  If a
 * particular application knows that it will never see more than 6 instances of
 * a service, then 6 is a fine value for MDNS_SERVICE_CACHE_SIZE.  In this
 * case, callbacks with a status of MDNS_CACHE_FULL would represent a warning
 * or error condition.  Similarly, if an application cannot handle any more
 * than 10 instances of a service, then MDNS_SERVICE_CACHE_SIZE should be 10
 * and callbacks with a status of MDNS_CACHE_FULL can be ignored.  If the
 * maximum number of service instances is not known, and the application
 * retains its own state for each instance of a service, it may be able to use
 * that state to do the right thing when the callback status is
 * MDNS_CACHE_FULL.
 */
typedef int (* mdns_query_cb)(void *data,
							  const struct mdns_service *s,
							  int status);

#define MDNS_DISCOVERED		1
#define MDNS_UPDATED		2
#define MDNS_DISAPPEARED	3
#define MDNS_CACHE_FULL		4

/* mdns_query_monitor: query for and monitor instances of a service
 *
 * When instances of the specified service are discovered, the specified
 * query callback is called as described above.
 *
 * fqst: Pointer to a null-terminated string specifying the fully-qualified
 * service type.  For example, "_http._tcp.local" would query for all http
 * servers in the ".local" domain.
 *
 * cb: an mdns_query_cb to be called when services matching the specified fqst
 * are discovered, are updated, or disappear.  cb will be passed the opaque
 * data argument described below, a struct mdns_service that represents the
 * discovered service, and a status code.
 *
 * data: a void * that will passed to cb when services are discovered, are
 * updated, or disappear.  This can be anything that the user wants, such as
 * pointer to a custom internal data structure.
 *
 * Returns one of the following codes:
 *
 * MDNS_SUCCESS: the query was successfully launched.  The caller should expect
 * the mdns_query_cb to be invoked as instances of the specified service are
 * discovered.
 *
 * MDNS_INVAL: cb was NULL or fqst was not valid.
 *
 * MDNS_NOMEM: MDNS_MAX_SERVICE_MONITORS is already being monitored.  Either
 * this value must be increased, or a service must be unmonitored by calling
 * mdns_query_unmonitor.
 *
 * MDNS_INUSE: The specified service type is already being monitored by another
 * callback, and multiple callbacks per service are not supported.
 *
 * MDNS_NORESP: No response from the querier.  Perhaps it was not launched or
 * it has crashed.
 *
 * Note: multiple calls to mdns_query_service_start are allowed.  This enables
 * the caller to query for more than just one service type.
 */
int mdns_query_monitor(char *fqst, mdns_query_cb cb, void *data);

/* mdns_query_unmonitor: stop monitoring a particular service
 *
 * fqst: The service type to stop monitoring, or NULL to unmonitor all
 * services.
 *
 * Note: Suppose a service has just been discovered and is being processed
 * while the call to mdns_query_monitor is underway.  A callback may be
 * generated before the service is unmonitored.
 */
void mdns_query_unmonitor(char *fqst);

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
