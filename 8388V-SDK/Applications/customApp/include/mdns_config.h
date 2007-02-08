/*! \file mdns_config.h
 *  \brief mDNS responder settings */

#ifndef MDNS_CONGIF_H
#define MDNS_CONFIG_H

/*! \def SERVICE_DOMAIN
 *  \brief this should usually be '.local' */
#define SERVICE_DOMAIN	"\5local"
/*! \def SERVICE_NAME
 *  \brief the device name as it will appear in Zeroconf browsers */
#define SERVICE_NAME	"\5""8388V"
/*! \def SERVICE_TARGET
 *  \brief the complete service domain name */
#define SERVICE_TARGET	"\xA""http-8388V" SERVICE_DOMAIN
/*! \def SERVICE_TYPE
 *  \brief service type identifier, for example '._http._tcp' for an HTTP 
 *  	   server */
#define SERVICE_TYPE	"\5_http\4_tcp"

#endif
