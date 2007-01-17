/*
 * Copyright Notice:
 * Copyright Treck Incorporated  1997 - 2004
 * Copyright Elmic Systems Japan 1997 - 2004
 * No portions or the complete contents of this file may be copied or
 * distributed in any form (including but not limited to printed or electronic
 * forms) without the expressed written consent of Treck Incorporated OR
 * Elmic Systems.  Copyright laws and International Treaties protect the
 * contents of this file.  Unauthorized use is prohibited.
 * All rights reserved.
 *
 * Description:     Sockets & Extensions API
 * This file is intended for the user to include in their applications.
 * This file SHOULD NOT be modified.
 * Filename:        trsocket.h
 * Author:          Odile & Paul
 * Date Created:    9/22/97
 * $Source: include/trsocket.h $
 *
 * Modification History
 * $Revision: 4.1 $
 * $Date: 2004/11/07 21:05:22PST $
 * $Author: xwang $
 * $ProjectName: /home/mks/proj/tcpip.pj $
 * $ProjectRevision: 4.1.2.1 $
 */

/*
 * NOTICE:
 * Since the Sockets interface was designed and developed at the University of
 * California, the following copyright is included.  Portions of the following
 * definitions are protected by that copyright
 */

/*-
 * Copyright (c) 1982, 1986, 1991, 1993, 1994
 *  The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *  This product includes software developed by the University of
 *  California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef _TRSOCKET_H_

#define _TRSOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <trsystem.h>

#define TM_PROJECT_REVISION "$ProjectRevision: 4.1.2.1 $"

#ifdef TM_USE_TEST_POINTS
#ifndef TM_USE_NETSTAT
/* some of the test point code requires netstat */
#define TM_USE_NETSTAT
#endif /* ! TM_USE_NETSTAT */
#endif /* TM_USE_TEST_POINTS */

#ifdef TM_BSD_TYPES_CLASH
#include <sys/types.h>
#endif /* TM_BSD_TYPES_CLASH */

#ifdef TM_BSD_CLASH
/*
 * undef our standard macros, rename our data type, and functions names
 * that conflict with those in LINUX and WIND32 with the same names
 */
#include <trbsd.h>
#endif /* TM_BSD_CLASH */

#ifdef _TRMACRO_H_
/* Make sure trmacro.h is not included before trsocket.h */
#error "You must not #include trmacro.h before trsocket.h"
#endif /* _TRMACRO_H_ */

#ifdef TM_PC_LINT
/*
 * PC-LINT error messages suppression
 */

/* Suspicious use of ; */
#define LINT_SUSP_SEMICOLON_MACRO(x)    /*lint -emacro( (722), x ) */

/* Constant evaluate to zero and excessive shift value */
#define LINT_ZERO_EXSH_MACRO(x)          /*lint -emacro( (778,572), x) */

#else

#define LINT_SUSP_SEMICOLON_MACRO(x)
#define LINT_ZERO_EXSH_MACRO(x)

#endif /* TM_PC_LINT */

LINT_SUSP_SEMICOLON_MACRO(tm_thread_stop)
LINT_ZERO_EXSH_MACRO(ntohs)


/*
 * Compiler/linker macros
 */
/*
 * Turbo Treck TCP/IP version
 */
/* Current version */
#define TM_VERSION         40   /* 4.0  */

#ifndef TM_FAR
#define TM_FAR
#endif /* TM_FAR */

#ifndef TM_CODE_FAR
#define TM_CODE_FAR
#endif /* TM_CODE_FAR */

#ifndef TM_PROTO_EXTERN
/*
 * Prototypes in trsocket.h and trmacro.h will not be declared as extern.
 */
#define TM_PROTO_EXTERN
#endif /* TM_PROTO_EXTERN */

#ifdef TM_DEV_SEND_OFFLOAD
#ifndef TM_USE_DRV_ONE_SCAT_SEND
#define TM_USE_DRV_ONE_SCAT_SEND
#endif /* TM_USE_DRV_ONE_SCAT_SEND */
#endif /* TM_DEV_SEND_OFFLOAD */





/*
 * Error macros (added TM_ERR_BASE to BSD 4.4 sys\types.h definitions)
 */
#define TM_ERR_BASE        200
/* Operation not permitted */
#define TM_EPERM           (TM_ERR_BASE+1)
/* No such file or directory */
#define TM_ENOENT          (TM_ERR_BASE+2)
/* No such process */
#define TM_ESRCH           (TM_ERR_BASE+3)
/* Interrupted system call */
#define TM_EINTR           (TM_ERR_BASE+4)
/* Input/output error */
#define TM_EIO             (TM_ERR_BASE+5)
/* Device not configured */
#define TM_ENXIO           (TM_ERR_BASE+6)
/* Bad file descriptor */
#define TM_EBADF           (TM_ERR_BASE+9)
/* No child processes */
#define TM_ECHILD          (TM_ERR_BASE+10)
/* Cannot allocate memory */
#define TM_ENOMEM          (TM_ERR_BASE+12)
/* Permission denied */
#define TM_EACCES          (TM_ERR_BASE+13)
/* Bad address */
#define TM_EFAULT          (TM_ERR_BASE+14)
/* File exists */
#define TM_EEXIST          (TM_ERR_BASE+17)
/* Operation not supported by device */
#define TM_ENODEV          (TM_ERR_BASE+19)
/* Not a directory */
#define TM_ENOTDIR         (TM_ERR_BASE+20)
/* Is a directory */
#define TM_EISDIR          (TM_ERR_BASE+21)
/* Invalid argument */
#define TM_EINVAL          (TM_ERR_BASE+22)
/* Too many open files */
#define TM_EMFILE          (TM_ERR_BASE+24)
/* No space left on device */
#define TM_ENOSPC          (TM_ERR_BASE+28)

/* Last error code of IO errors */
#define TM_ERR_IO_LAST        TM_ENOSPC

/*
 * Network errors.
 */
/*
 * non-blocking and interrupt i/o
 */
/* Operation would block */
#define TM_EWOULDBLOCK     (TM_ERR_BASE+35)
/* Resource temporarily unavailable */
#define TM_EAGAIN          TM_EWOULDBLOCK
/* Operation now in progress */
#define TM_EINPROGRESS     (TM_ERR_BASE+36)
/* Operation already in progress*/
#define TM_EALREADY        (TM_ERR_BASE+37)

/* Socket operation on non-socket */
#define TM_ENOTSOCK        (TM_ERR_BASE+38)
/* Destination address required */
#define TM_EDESTADDRREQ    (TM_ERR_BASE+39)
/* Message too long */
#define TM_EMSGSIZE        (TM_ERR_BASE+40)
/* Protocol wrong type for socket */
#define TM_EPROTOTYPE      (TM_ERR_BASE+41)
/* Protocol not available */
#define TM_ENOPROTOOPT     (TM_ERR_BASE+42)
/* Protocol not supported */
#define TM_EPROTONOSUPPORT (TM_ERR_BASE+43)
/* Socket type not supported */
#define TM_ESOCKTNOSUPPORT (TM_ERR_BASE+44)
/* Operation not supported */
#define TM_EOPNOTSUPP      (TM_ERR_BASE+45)
/* Protocol family not supported */
#define TM_EPFNOSUPPORT    (TM_ERR_BASE+46)
/* Address family not supported by protocol family */
#define TM_EAFNOSUPPORT    (TM_ERR_BASE+47)
/* Address already in use */
#define TM_EADDRINUSE      (TM_ERR_BASE+48)
/* Can't assign requested address */
#define TM_EADDRNOTAVAIL   (TM_ERR_BASE+49)

/* Network is down */
#define TM_ENETDOWN        (TM_ERR_BASE+50)
/* Network is unreachable */
#define TM_ENETUNREACH     (TM_ERR_BASE+51)
/* Network dropped connection on reset */
#define TM_ENETRESET       (TM_ERR_BASE+52)
/* Software caused connection abort */
#define TM_ECONNABORTED    (TM_ERR_BASE+53)
/* Connection reset by peer */
#define TM_ECONNRESET      (TM_ERR_BASE+54)
/* No buffer space available */
#define TM_ENOBUFS         (TM_ERR_BASE+55)
/* Socket is already connected */
#define TM_EISCONN         (TM_ERR_BASE+56)
/* Socket is not connected */
#define TM_ENOTCONN        (TM_ERR_BASE+57)
/* Can't send after socket shutdown */
#define TM_ESHUTDOWN       (TM_ERR_BASE+58)
/* Too many references: can't splice */
#define TM_ETOOMANYREFS    (TM_ERR_BASE+59)
/* Operation timed out */
#define TM_ETIMEDOUT       (TM_ERR_BASE+60)
/* Connection refused */
#define TM_ECONNREFUSED    (TM_ERR_BASE+61)
/* Host is down */
#define TM_EHOSTDOWN       (TM_ERR_BASE+64)
/* No route to host */
#define TM_EHOSTUNREACH    (TM_ERR_BASE+65)

#define TM_ERR_BASE_LAST   TM_EHOSTUNREACH

/* socket call failed */
#define TM_SOCKET_ERROR     -1
/* No error */
#define TM_ENOERROR         0

/* Error extensions to handle ICMP errors */
#define TM_EXT_ERROR_BASE   300
/* ICMP type unreach with code >= 13 */
#define TM_EXT_EHOSTUNREACH (TM_EXT_ERROR_BASE+1)
/* ICMP source quench */
#define TM_EXT_SRCQNCH      (TM_EXT_ERROR_BASE+2)
/* ICMP time xceeded in transit */
#define TM_EXT_TTLTRANSIT   (TM_EXT_ERROR_BASE+3)
/* ICMP time xceeded in reassemly */
#define TM_EXT_TTLREASS     (TM_EXT_ERROR_BASE+4)

/* Additional error codes for FTP application program interface */

/* Command requires user to be loggedin, and user is not. */
#define TM_ENOTLOGIN        (TM_EXT_ERROR_BASE+5)
/* Temporary server error. Re-sending the same command could succeed */
#define TM_ESERVERTEMP      (TM_EXT_ERROR_BASE+6)
/* Permanent server error. Re-sending the same command will always fail */
#define TM_ESERVERPERM      (TM_EXT_ERROR_BASE+7)
/* server replies are out of SYNC. FTP user should call tfFtpQuit() */
#define TM_ESERVERSYNC      (TM_EXT_ERROR_BASE+8)

/* Socket error code used to report ICMPv6 errors of unknown type.
   ([RFC2463].R2.4:10) */
#define TM_EXT_UNKNOWN_ERR_TYPE (TM_EXT_ERROR_BASE+9)

#define TM_EXT_ERROR_LAST   TM_EXT_UNKNOWN_ERR_TYPE

/* Error extensions to handle IPv6 errors. */
#define TM_IPV6_ERROR_BASE      400
/* IPv6 functionality currently not implemented. */
#define TM_6_ENOTIMPLEMENTED    (TM_IPV6_ERROR_BASE+1)

/*
 * BSD Socket Structures
 */

/*
 * BSD Macro Definitions
 */
#define AF_UNSPEC  0
#define AF_INET    2
#define AF_INET6   28
#define PF_UNSPEC  AF_UNSPEC
#define PF_INET    AF_INET
#define PF_INET6   AF_INET6

/* IPv6: we implement the 4.4BSD version of sockaddr_in6. */
#define SIN6_LEN

/*
 * Defined per BSD 4.4
 * Changing these value may affect operation through a firewall
 * If these value is changed, the stack should be recompiled to
 * reflect the change.
 *
 * It is provided here for BSD compatibility
 */
#define IPPORT_RESERVEDSTART (int)600
#define IPPORT_RESERVED      (int)1024

/*
 * Protocol Numbers
 * (OLD BSD WAY)
 */
#define IP_PROTOIP        0
#define IP_PROTOICMP      1
#define IP_PROTOIGMP      2
#define IP_PROTOIPV4      4 /* IP-in-IP encapsulation */
#define IP_PROTOTCP       6
#define IP_PROTOUDP      17
#define IP_PROTORDP      46
#define IP_PROTOOSPF     89
#define IP_PROTOTPACKET 127

/*
 * Protocol Numbers
 * (New BSD way)
 */
#define IPPROTO_IP      IP_PROTOIP
#define IPPROTO_ICMP    IP_PROTOICMP
#define IPPROTO_IGMP    IP_PROTOIGMP
#define IPPROTO_IPV4    IP_PROTOIPV4 /* IP-in-IP encapsulation */
#define IPPROTO_TCP     IP_PROTOTCP
#define IPPROTO_UDP     IP_PROTOUDP
#define IPPROTO_RDP     IP_PROTORDP
#define IPPROTO_OSPF    IP_PROTOOSPF
#define IPPROTO_TPACKET IP_PROTOTPACKET

/* IPv6 protocol IDs, refer to [RFC2292]. */
#define IPPROTO_HOPOPTS     0
#define IPPROTO_IPV6        41
#define IPPROTO_ROUTING     43
#define IPPROTO_FRAGMENT    44
#define IPPROTO_ESP         50
#define IPPROTO_AH          51
#define IPPROTO_ICMPV6      58
#define IPPROTO_NONE        59
#define IPPROTO_DSTOPTS     60

#ifdef TM_6_MIP_DRAFT24
/* IPv6 Mobility Header protocol */
#define IPPROTO_MH          62
#else /* ! TM_6_MIP_DRAFT24 */
/* assigned by IANA */
#define IPPROTO_MH          135
#endif /* ! TM_6_MIP_DRAFT24 */

#define SOCK_STREAM     1       /* stream socket */
#define SOCK_DGRAM      2       /* datagram socket */
#define SOCK_RAW        3       /* raw-protocol interface */
#define SOCK_RDM        4       /* reliably-delivered message */
#define SOCK_SEQPACKET  5       /* sequenced packet stream */

/*
 * Option flags per-socket.
 */
#define SO_DEBUG        0x0001      /* turn on debugging info recording */
#define SO_ACCEPTCONN   0x0002      /* socket has had listen() */
#define SO_REUSEADDR    0x0004      /* allow local address reuse */
#define SO_KEEPALIVE    0x0008      /* keep connections alive */
#define SO_DONTROUTE    0x0010      /* just use interface addresses */
#define SO_BROADCAST    0x0020      /* permit sending of broadcast msgs */
#define SO_USELOOPBACK  0x0040      /* bypass hardware when possible */
#define SO_LINGER       0x0080      /* linger on close if data present */
#define SO_OOBINLINE    0x0100      /* leave received OOB data in line */
#define SO_REUSEPORT    0x0200      /* allow local address & port reuse */
#define SO_TIMESTAMP    0x0400      /* timestamp received dgram traffic */
#define SO_IPHDRINCL    0x0800      /* IP Header included on the socket */
#define TM_6_IPO_MN_USE_HA_TUNNEL 0x2000 /* disable MIPv6 route optimization */
#define SO_UNPACKEDDATA 0x4000      /* DSP only: user data is unpacked */

/*
 * Additional options, not kept in so_options.
 */
#define SO_SNDBUF          0x1001   /* send buffer size */
#define SO_RCVBUF          0x1002   /* receive buffer size */
#define SO_SNDLOWAT        0x1003   /* send low-water mark */
#define SO_RCVLOWAT        0x1004   /* receive low-water mark */
#define SO_SNDTIMEO        0x1005   /* send timeout */
#define SO_RCVTIMEO        0x1006   /* receive timeout */
#define SO_ERROR           0x1007   /* get error status and clear */
#define SO_TYPE            0x1008   /* get socket type */
#define TM_SO_SNDAPPEND    0x1009   /* send buffer copy threshold in bytes */
#define TM_SO_RCVCOPY      0x100A   /* recv buffer copy fraction */
#define TM_SO_SND_DGRAMS   0x100B   /* send datagram count (non-TCP) */
#define TM_SO_RCV_DGRAMS   0x100C   /* receive datagram count (non-TCP) */

#define TM_SOC_PROTO_DEFAULT         0 /* unspecified protocol by the user */
/* Socket protocols currently supported by Treck in the socket() API call. */
#define tm_treck_sock_protocols(protocol)  \
       (   (protocol == IPPROTO_ICMP)     \
        || (protocol == IPPROTO_IGMP)     \
        || (protocol == IPPROTO_TCP)      \
        || (protocol == IPPROTO_UDP)      \
        || (protocol == IPPROTO_ICMPV6)   \
        || (protocol == TM_SOC_PROTO_DEFAULT) /* default */ )

/*
 * Unsigned long and signed long literal specifiers.  These can be redefined
 * in trsystem.h
 */
#ifndef TM_UL
#define TM_UL(x) x##UL
#endif /* TM_UL */

#ifndef TM_L
#define TM_L(x) x##L
#endif /* TM_L */

#ifndef TM_UNREF_IN_ARG 
/* reference unused input argument in function to avoid compiler warning */
#define TM_UNREF_IN_ARG(x) ((void)(x))
#endif /* ! TM_UNREF_IN_ARG */

/*
 * Unix ioctl requests
 */
/* set/clear nonblocking I/O */
#define FIONBIO         TM_UL(0x01)
/* Get #bytes to read */
#define FIONREAD        TM_UL(0x02)
/* At out-of-band mark? */
#define SIOCATMARK      TM_UL(0x04)

/*
 * TCP Options.
 * options below 0x4000 are settable flags,
 * options 0x4000 and beyond, set values.
 */
/*
 * Turn ON/OFF disabling Nagle Algorithm (default OFF, Nagle algoritm
 * enabled: delay sending non full-size segments when it does not empty
 * the queue.)
 */
#define TCP_NODELAY           0x0001
#define TCP_MAXSEG            0x0002 /* Set Maximum TCP segment */
/*
 * Turn ON/OFF non usage of push bit when send queue is emptied.
 * (Default OFF: Push bit set when send queue is emptied)
 */
#define TCP_NOPUSH            0x0004
/*
 * Make urgent pointer point to last byte of urgent data (RFC1122)
 * (Default on)
 */
#define TCP_STDURG            0x0008
/*
 * Set amount of time before a connection is broken once TCP starts
 * retransmission, or probing a zero window. 0 means use default value,
 * -1 means retransmit forever. If a positive value is specified, it may be
 * rounded up to the implementation's next retrasmission time.
 * (Note set both a value and a flag.)
 */
#define TCP_MAXRT              0x0010
/* 0x0020 is reserved */

/* Set keep alive timeout (default 7200 seconds) */
#define TCP_KEEPALIVE          0x4001

/*
 * Treck Extended
 */
/* Turn ON/OFF Selective Acknolwlegements (default off) */
#define TM_TCP_SEL_ACK         0x0040
/* Turn ON/OFF Big Window Scaling (default off) */
#define TM_TCP_WND_SCALE       0x0080
/* Turn ON/OFF Time stamp (default off) */
#define TM_TCP_TS              0x0100
/* Turn ON/OFF Slow Start (defaul on) */
#define TM_TCP_SLOW_START      0x0200
/*
 * Set PUSH on every packet even when not emptying the send queue! Use
 * sparingly. TCP_NODELAY option is preferable to this option.
 * OL 8/27/01 change: Also preserve packet boundaries when sending data.
 *                    (See TM_TCP_PACKET flag below.)
 */
#define TM_TCP_PUSH_ALL        0x0400
/*
 * Preserve user packet boundary when sending data:
 * . Set PUSH on every packet even when not emptying the send queue.
 *   (TM_TCP_PUSH_ALL)
 * . Preserve packet boundaries even on retransmissions.
 */
#define TM_TCP_PACKET          0x0400
/* 0x0800, and beyond are reserved */

/* Set Delay Ack time (default 200ms) */
#define TM_TCP_DELAY_ACK       0x4002
/* Set Maximum number of retransmissions (default 12) */
#define TM_TCP_MAX_REXMIT      0x4003
/* Set keep alive interval probes (default 75 seconds) */
#define TM_TCP_KEEPALIVE_INTV  0x4004
/* Set keep alive probe count (default 8). */
#define TM_TCP_KEEPALIVE_CNT   0x4005
/*
 * Set amount of time to stay in FIN WAIT 2 when socket has been closed
 * (default 600 seconds).
 */
#define TM_TCP_FINWT2TIME      0x4006
/*
 * set TIME WAIT time
 * (Default 2*Maximum segment life time (2*30 seconds, or 60 seconds))
 */
#define TM_TCP_2MSLTIME        0x4007
/*
 * Set default retransmission timeout value in milliseconds
 * (default 3,000 milliseconds)
 */
#define TM_TCP_RTO_DEF         0x4008
/*
 * Set minimum retransmission timeout value in milliseconds
 * (default 100 milliseconds)
 */
#define TM_TCP_RTO_MIN         0x4009
/*
 * Set maximum retransmission timeout value in milliseconds
 * (default 64,000 milliseconds)
 */
#define TM_TCP_RTO_MAX         0x400A
/*
 * Set minimum zero window probe timeout value in milliseconds
 * (default 500 milliseconds)
 */
#define TM_TCP_PROBE_MIN      0x400B
/*
 * Set maximum zero window probe timeout value in milliseconds
 * (default 60,000 milliseconds)
 */
#define TM_TCP_PROBE_MAX      0x400C

/* SSL options*/
/*
 * Set this option to enable SSL client negotiation on this socket,
 * optionLength must be sizeof(int), any non-zero value will enable SSL client
 */
#define TM_TCP_SSL_CLIENT                  0x400D
/*
 * Set this option to enable SSL server negotiation on this socket,
 * optionLength must be sizeof(int), any non-zero value will enable SSL server
 */
#define TM_TCP_SSL_SERVER                  0x400E
/*
 * Set the SSL session number for this socket. The option length must be
 * sizeof(int)
 */
#define TM_TCP_SSLSESSION                  0x400F
/*
 * Set the SSL connection negotiation timeout in seconds. (Default is 10
 * seconds.)
 */
#define TM_TCP_SSL_CONNECT_TIMEOUT         0x4010
/*
 * Set the SSL send minimum size. If user's send data is less
 * than this value, user data will be queued. Option length
 * must be sizeof(int), and option value can not greater than 0xffff.
 */
#define TM_TCP_SSL_SEND_MIN_SIZE           0x4011
/*
 * Set the SSL record maximum size. Each record will at most have
 * that much user data encapsulated. User data bigger than this
 * size limit will be cut into two records, Option length
 * must be sizeof(int), and option value can not greater than 0xffff.
 */
#define TM_TCP_SSL_SEND_MAX_SIZE           0x4012
/*
 * Set the window size of the listening socket to be different than
 * specified by SO_RCVBUF flag. This will allow listening socket's
 * window size to be different than the active socket spawned when
 * accept() is called.
*/
#define TM_TCP_PEND_ACCEPT_RECV_WND        0x4013

/*
 * IP Options
 */
#define IPO_HDRINCL           0x0002
#define IPO_TOS               0x0003
#define IPO_TTL               0x0004
#define IPO_MULTICAST_IF      0x0009
#define IPO_MULTICAST_TTL     0x000A
#define IPO_ADD_MEMBERSHIP    0x000C
#define IPO_DROP_MEMBERSHIP   0x000D

#define IPO_SRCADDR           0x1000

#define IP_TTL                IPO_TTL
#define IP_TOS                IPO_TOS
#define IP_MULTICAST_TTL      IPO_MULTICAST_TTL
#define IP_MULTICAST_IF       IPO_MULTICAST_IF
#define IP_HDRINCL            IPO_HDRINCL
#define IP_ADD_MEMBERSHIP     IPO_ADD_MEMBERSHIP
#define IP_DROP_MEMBERSHIP    IPO_DROP_MEMBERSHIP

/* IP TOS option values */
#define IPTOS_MINCOST         0x02
#define IPTOS_RELIABILITY     0x04
#define IPTOS_THROUGHPUT      0x08
#define IPTOS_LOWDELAY        0x10



/* other IPv6 BSD socket API stuff */
/* Refer to [RFC2553]. Max length of interface name. This should correspond to
   the value of the internal API macro TM_MAX_DEVICE_NAME. */
#define IF_NAMESIZE 14

/* Refer to [RFC2553]. Length of IPv4 address string. */
#define INET_ADDRSTRLEN 16


/* BSD address constants used for IPv4, in network byte order */
#define INADDR_ANY              TM_UL(0x00000000)
#ifdef TM_LITTLE_ENDIAN
#define INADDR_LOOPBACK         TM_UL(0x0100007F)
#else /* TM_BIG_ENDIAN */
#define INADDR_LOOPBACK         TM_UL(0x7F000001)
#endif /* TM_BIG_ENDIAN */

#define MSG_OOB               0x0001  /* process out-of-band data */
#define MSG_PEEK              0x0002  /* peek at incoming message */
#define MSG_DONTROUTE         0x0004  /* send without using routing tables */
#define MSG_EOR               0x0008  /* data completes record */
#define MSG_TRUNC             0x0010  /* data discarded before delivery */
#define MSG_CTRUNC            0x0020  /* control data lost before delivery */
#define MSG_WAITALL           0x0040  /* wait for full request or error */
#define MSG_DONTWAIT          0x0080  /* this message should be nonblocking */
#define MSG_EOF               0x0100  /* data completes connection */
#define MSG_BROADCAST         0x0200  /* Broadcast Packet */
/* User send data or non TCP recv data is scattered: */
#define MSG_SCATTERED         0x0400
#define MSG_IPHDRINCL         0x0800  /* IP header included in data */

#define MSG_MASK              0x0fff  /* Upper Nibble is reserved */

/* Allowed flags for send call */
#define TM_SEND_FLAGS             (   MSG_DONTWAIT | MSG_DONTROUTE | MSG_OOB \
                                    | MSG_WAITALL )
/* Allowed flags for sendto call */
#define TM_SENDTO_FLAGS           (MSG_DONTWAIT | MSG_DONTROUTE)
/* Allowed flags for tfZeroCopySend call (same as send) */
#define TM_ZEROCOPYSEND_FLAGS     (   MSG_DONTWAIT | MSG_DONTROUTE | MSG_OOB \
                                    | MSG_WAITALL  | MSG_SCATTERED )
/* Allowed flags for tfZeroCopySendTo call (same as sendto) */
#define TM_ZEROCOPYSENDTO_FLAGS   (   MSG_DONTWAIT | MSG_DONTROUTE           \
                                    | MSG_SCATTERED )

/* Allowed flags for recv call */
#define TM_RECV_FLAGS                                                        \
    ( MSG_DONTWAIT | MSG_PEEK | MSG_OOB | MSG_WAITALL )
/* Allowed flags for recvfrom call */
#define TM_RECVFROM_FLAGS         (MSG_DONTWAIT | MSG_PEEK)
/*
 * Allowed flags for tfZeroCopyRecv call: same as recv, but allow
 * MSG_SCATTERED for non TCP recv, and do not allow MSG_OOB, and do
 * not allow MSG_WAITALL
 */
#define TM_ZEROCOPYRECV_FLAGS     (MSG_DONTWAIT | MSG_PEEK | MSG_SCATTERED)
/*
 * Allowed flags for tfZeroCopyRecvFrom call: same as recvfrom, but allow
 * MSG_SCATTERED.
 */
#define TM_ZEROCOPYRECVFROM_FLAGS (MSG_DONTWAIT | MSG_PEEK | MSG_SCATTERED)


/* Arp entry or Routing entry infinite time to live */
#define TM_RTE_INF        TM_UL(0x7FFFFFFF) /* Infinite time to live */

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define SOL_SOCKET      ((int)0xFFFF)   /* options for socket level */


#ifndef TM_USER_32BIT_TYPE
/*
 * If the user hasn't defined their own 32-bit type, use default
 */
#define TM_USER_32BIT_TYPE
typedef unsigned long ttUser32Bit;
typedef signed long   ttUserS32Bit;
#endif /* !TM_USER_32BIT_TYPE */

#ifndef TM_USER_16BIT_TYPE
#define TM_USER_16BIT_TYPE
typedef unsigned short ttUser16Bit;
#endif /* !TM_USER_16BIT_TYPE */

#ifndef TM_USER_8BIT_TYPE
#define TM_USER_8BIT_TYPE
typedef unsigned char ttUser8Bit;
#endif /* !TM_USER_8BIT_TYPE */

typedef char            TM_FAR * ttUserCharPtr;
typedef int             TM_FAR * ttUserIntPtr;
typedef const char      TM_FAR * ttUserConstCharPtr;
typedef ttUser8Bit      TM_FAR * ttUser8BitPtr;
typedef ttUser16Bit     TM_FAR * ttUser16BitPtr;
typedef ttUser32Bit     TM_FAR * ttUser32BitPtr;
typedef ttUserCharPtr   TM_FAR * ttUserCharPtrPtr;

typedef unsigned char   TM_FAR * ttUnsignedCharPtr;


#ifndef TM_USER_KERNEL_CRITICAL_STATUS_TYPE
/*
 * If the user hasn't defined their own kernel critical status type,
 * use default
 */
#define TM_USER_KERNEL_CRITICAL_STATUS_TYPE
typedef unsigned long ttUserKernelCriticalStatus;
#endif /* TM_USER_KERNEL_CRITICAL_STATUS_TYPE */





/*
 * BSD Type Definitions
 */
#ifndef TM_BSD_TYPES_CLASH

typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef ttUser32Bit     u_long;
typedef char TM_FAR *   caddr_t;
typedef unsigned int    socklen_t;

#ifndef FD_SETSIZE
/*
 * Select uses bit masks of file descriptors in longs.  These macros
 * manipulate such bit fields (the filesystem macros use chars).
 * FD_SETSIZE may be defined by the user, but the default here should
 * be enough for most uses.
 */
#ifdef TM_SOC_INDEX_MAX
#define FD_SETSIZE  TM_SOC_INDEX_MAX
#else /* !TM_SOC_INDEX_MAX */
#define FD_SETSIZE  256
#endif /* TM_SOC_INDEX_MAX */
#endif /* FD_SETSIZE */

#define NBBY 8 /* Number of bits in a byte */

typedef ttUserS32Bit    fd_mask;
#define NFDBITS (sizeof(fd_mask) * NBBY)    /* bits per mask */

#ifndef howmany
#define howmany(x, y)   (((x) + ((y) - 1)) / (y))
#endif

typedef struct fd_set
{
    fd_mask fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;

#define FD_SET(n, p)                                                        \
    ((p)->fds_bits[(n)/(int)NFDBITS] |=                                     \
                           (ttUserS32Bit)((TM_UL(1) << ((n) % (int)NFDBITS))))
#define FD_CLR(n, p)                                                        \
    ((p)->fds_bits[(n)/(int)NFDBITS] &=                                     \
                           (ttUserS32Bit)(~(TM_UL(1) << ((n) % (int)NFDBITS))))
#define FD_ISSET(n, p) \
    (   (ttUser32Bit)((p)->fds_bits[(n)/(int)NFDBITS]) \
      & (TM_UL(1) << ((n) % (int)NFDBITS))  )

#ifdef TM_USE_CLIB
#define FD_ZERO(p)     (void)memset((ttUserVoidPtr)(p), 0, sizeof(*(p)))
#define FD_COPY(f, t)  \
            memcpy((ttUserVoidPtr)t, (ttUserConstVoidPtr)f, sizeof(*(f)))
#else /* !TM_USE_CLIB */
#define FD_ZERO(p)     tfBzero(p, sizeof(*(p)))
#define FD_COPY(f, t)  tfBcopy(f, t, sizeof(*(f)))
#endif /* !TM_USE_CLIB */

/* move timeval definition inside ifndef TM_BSD_TYPES_CLASH */
struct timeval
{
    u_long tv_sec;    /* Number of Seconds */
    u_long tv_usec;   /* Number of micro seconds */
};

#endif /* TM_BSD_TYPES_CLASH */

typedef u_char  TM_FAR * ttUcharPtr;

/*
 * BSD Structure Definitions
 */
#ifndef TM_PROTOCOL_MAX_ADDR_SIZE
#define TM_PROTOCOL_MAX_ADDR_SIZE          14
#endif /* !TM_PROTOCOL_MAX_ADDR_SIZE */

struct sockaddr
{
    u_char  sa_len;      /* Total Length */
    u_char  sa_family;   /* Address Family AF_xxx */
/* up to TM_PROTOCOL_MAX_ADDR_SIZE bytes of protocol specific address */
    char    sa_data[TM_PROTOCOL_MAX_ADDR_SIZE];
};

/* Null sockaddr pointer */
#define TM_SOCKADDR_NULL_PTR        (struct sockaddr TM_FAR *)0

/* Bind to this port to allow listening on all the ports */
#define TM_WILD_PORT                (u_short)0xffff


/* Structure used to store an IPv4 address */
struct in_addr
{
    u_long s_addr; /* 32bit netid/hostid address in network byte order */
};



/* Structure used for joining or leaving a group multicast IP address */
struct ip_mreq
{
    struct in_addr imr_multiaddr; /* IPv4 Class D multicast address */
    struct in_addr imr_interface; /* IPv4 address of local interface */
};


struct sockaddr_in
{
    u_char          sin_len;    /* length of struct sockaddr_in */
    u_char          sin_family; /* AF_INET */
    u_short         sin_port;   /* 16bit Port Number in network byte order */
    struct  in_addr sin_addr;   /* 32bit netid/hostid in network byte order */
    char            sin_zero[8];/* unused */
};


/* Refer to [RFC2553]. Because the sockaddr type isn't big enough to hold an
   IPv6 address, the application instead uses the sockaddr_storage type to
   allocate a large enough buffer for an IPv6 address returned by
   getsockname(). Additionally, all of the Elmic tfNg "Next Generation" APIs
   expect IP address parameters to be of type sockaddr_storage, which
   can hold either an IPv4 or an IPv6 address, the ss_family field of
   sockaddr_storage being used to differentiate between the two address
   formats. */
struct sockaddr_storage
{
    union
    {
        struct sockaddr_in      ipv4;
        struct sockaddr         ipSockAddr; /* For PC-LINT */
    } addr;
};
#define ss_len      addr.ipv4.sin_len
#define ss_family   addr.ipv4.sin_family
#define ss_port     addr.ipv4.sin_port

typedef struct sockaddr_storage TM_FAR *         ttSockAddrStoragePtr;
typedef const struct sockaddr_storage TM_FAR *   ttConstSockAddrStoragePtr;

/* Null sockaddr_storage pointer */
#define TM_SOCKADDR_STORAGE_NULL_PTR  (ttSockAddrStoragePtr)0

struct iovec
{
    caddr_t iov_base;
    int     iov_len;
};

/*
 * Structure used for manipulating linger option.
 */
struct  linger
{
    int l_onoff;        /* option on/off */
    int l_linger;       /* linger time */
};

/*
 * Message header for recvmsg and sendmsg calls.
 * Used value-result for recvmsg, value only for sendmsg.
 * any number of ancillary data objects can be specified by the msg_control and
 * msg_controllen members of the msghdr structure
 */
struct msghdr
{
    caddr_t                msg_name;         /* optional address */
    u_int                  msg_namelen;      /* size of address */
    struct  iovec TM_FAR * msg_iov;          /* scatter/gather array */
    u_int                  msg_iovlen;       /* # elements in msg_iov */
    caddr_t                msg_control;      /* UNUSED for embedded system */
    u_int                  msg_controllen;   /* UNUSED for embedded system  */
    int                    msg_flags;        /* flags on received message */
};

/* structure used for ancillary data objects */
struct cmsghdr
{
    socklen_t  cmsg_len;   /* #bytes, including this header */
    int        cmsg_level; /* originating protocol */
    int        cmsg_type;  /* protocol-specific type */
/* followed by unsigned char cmsg_data[]; */
};

typedef struct cmsghdr TM_FAR * ttAncDataPtr;
typedef struct cmsghdr TM_FAR * ttCmsgHdrPtr;

#ifndef TM_ALIGN_BYTES
/* If the alignment bytes is not defined , use default 4 bytes */
#define TM_ALIGN_BYTES      4
#endif /* TM_ALIGN_BYTES */

/* given a length, return the length with required alignment */
#define CMSG_ALIGN(len) \
    (((len + TM_ALIGN_BYTES - 1) / TM_ALIGN_BYTES) * TM_ALIGN_BYTES)
#define tm_align(len)      CMSG_ALIGN(len)


/*
 * returns a pointer to the first cmsghdr in the ancillary data
 * buffer associated with the passed msghdr.
 */
#define CMSG_FIRSTHDR(mhdr) \
        ( (mhdr)->msg_controllen >= sizeof(struct cmsghdr) ? \
          (ttCmsgHdrPtr)(mhdr)->msg_control : \
          (ttCmsgHdrPtr)NULL )

/* given pointer to struct cmsghdr, return pointer to next cmsghdr */
#define CMSG_NXTHDR(mhdr, cmsg) \
        ( ((cmsg) == NULL) ? CMSG_FIRSTHDR(mhdr) : \
          (((ttUcharPtr)(cmsg) + CMSG_ALIGN((cmsg)->cmsg_len) \
                             + CMSG_ALIGN(sizeof(struct cmsghdr)) > \
            (ttUcharPtr)((mhdr)->msg_control) + (mhdr)->msg_controllen) ? \
           (ttCmsgHdrPtr)NULL : \
           (ttCmsgHdrPtr)((   ttUcharPtr)(cmsg) \
                                + CMSG_ALIGN((cmsg)->cmsg_len))) )

/* given pointer to struct cmsghdr, return pointer to data */
#define CMSG_DATA(cmsg) \
            ((ttUnsignedCharPtr)(cmsg) + CMSG_ALIGN(sizeof(struct cmsghdr)))

/* RFC 2292 additions */
/*
 * returns the number of bytes that an ancillary element with payload of the
 * passed data length occupies. This is a constant expression.
 * DSP note: CMSG_SPACE returns the number of required words (not bytes), so
 *           'len' should always be specified in words.
 */
#define CMSG_SPACE(len) (CMSG_ALIGN(sizeof(struct cmsghdr)) + CMSG_ALIGN(len))
/*
 * CMSG_LEN returns the value to store in the cmsg_len member
 * of  the  cmsghdr structure, taking into account any necessary
 * alignment. It takes the data length as  an  argument.
 * This is a constant expression.
 */
#define CMSG_LEN(len)   (CMSG_ALIGN(sizeof(struct cmsghdr)) + (len))

/*
 * Convert a long to a string. Returns pointer to the begining of the
 * converted number.
 */
TM_PROTO_EXTERN char TM_FAR * tfLongToString(ttUserS32Bit  number,
                                             char TM_FAR * string,
                                             unsigned int  stringLength,
                                             unsigned int  base);


/*
 * BSD Sockets Interface Function Prototypes
 */
TM_PROTO_EXTERN ttUser32Bit inet_addr(char TM_FAR * ipAddressDottedStringPtr);
TM_PROTO_EXTERN ttUser32Bit inet_aton(char TM_FAR * ipAddressDottedStringPtr);
TM_PROTO_EXTERN char TM_FAR * inet_ntoa(struct in_addr  inAddr);

TM_PROTO_EXTERN int socket(
    int addressFamily,
    int socketType,
    int protocol);

TM_PROTO_EXTERN int rresvport(int TM_FAR * portToReservePtr);

TM_PROTO_EXTERN int listen(
    int socketDescriptor,
    int backLog);

TM_PROTO_EXTERN int bind(
    int                             socketDescriptor,
    const struct sockaddr TM_FAR *  localAddressPtr,
    int                             localAddressLength);
/*
 * Same as bind, but does not check that the IP address is a configured
 * IP address on an interface.
 */
TM_PROTO_EXTERN int tfBindNoCheck(
    int                             socketDescriptor,
    const struct sockaddr TM_FAR *  localAddressPtr,
    int                             localAddressLength );

TM_PROTO_EXTERN int send(
    int                 socketDescriptor,
    const char TM_FAR * bufferPtr,
    int                 bufferLength,
    int                 flags);

TM_PROTO_EXTERN int recv(
    int           socketDescriptor,
    char TM_FAR * bufferPtr,
    int           bufferLength,
    int           flags );

TM_PROTO_EXTERN int sendto(
    int                             socketDescriptor,
    const char TM_FAR *             bufferPtr,
    int                             bufferLength,
    int                             flags,
    const struct sockaddr TM_FAR *  toAddressPtr,
    int                             addressLength );

TM_PROTO_EXTERN int recvfrom(
    int                      socketDescriptor,
    char            TM_FAR * bufferPtr,
    int                      bufferLength,
    int                      flags,
    struct sockaddr TM_FAR * fromAddressPtr,
    int             TM_FAR * addressLengthPtr );

TM_PROTO_EXTERN int readv(
    int                   socketDescriptor,
    struct iovec TM_FAR * iovecPtr,
    int                   iocount );

TM_PROTO_EXTERN int writev(
    int                   socketDescriptor,
    struct iovec TM_FAR * iovecPtr,
    int                   iocount );


TM_PROTO_EXTERN int accept(
    int                      socketDescriptor,
    struct sockaddr TM_FAR * peerAddressPtr,
    int             TM_FAR * addressLengthPtr );

TM_PROTO_EXTERN int setsockopt(
    int                 socketDescriptor,
    int                 levelOfLayer,
    int                 optionName,
    const char TM_FAR * optionValuePtr,
    int                 optionLength );

TM_PROTO_EXTERN int getsockopt(
    int           socketDescriptor,
    int           levelOfLayer,
    int           optionName,
    char TM_FAR * optionValuePtr,
    int  TM_FAR * optionLengthPtr );

TM_PROTO_EXTERN int socketpair(
    int addressFamily,
    int socketType,
    int protocol,
    int TM_FAR * socketVector );

TM_PROTO_EXTERN int getpeername(
    int                      socketDescriptor,
    struct sockaddr TM_FAR * fromAddressPtr,
    int             TM_FAR * addressLengthPtr );

TM_PROTO_EXTERN int getsockname(
    int                      socketDescriptor,
    struct sockaddr TM_FAR * myAddressPtr,
    int             TM_FAR * addressLengthPtr );

/*
 * values for shutdown second argument
 * howToShutdown == 0       shutdown the read-half of the connection
 * howToShutdown == 1       shutdown the write-half of the connection
 * howToShutdown == 2       shutdown both halves of the connection
 */
TM_PROTO_EXTERN int shutdown(
    int socketDescriptor,
    int howToShutdown);

TM_PROTO_EXTERN int select(
    int                     maxSocketDescriptor,
    fd_set         TM_FAR * readSocketSetPtr,
    fd_set         TM_FAR * writeSocketSetPtr,
    fd_set         TM_FAR * exceptionSocketSetPtr,
    struct timeval TM_FAR * timeOut );

TM_PROTO_EXTERN int connect(
    int                             socketDescriptor,
    const struct sockaddr TM_FAR *  addressPtr,
    int                             addressLength );

/* Refer to [RFC2553]. Maps an interface name into its corresponding interface
   index. */
TM_PROTO_EXTERN unsigned int if_nametoindex(
    const char TM_FAR * ifname );

/* Refer to [RFC2553]. Maps an interface index into its corresponding interface
   name. */
TM_PROTO_EXTERN char TM_FAR * if_indextoname(
    unsigned int    ifindex,
    char TM_FAR *   ifname );

/* Refer to [RFC2553]. Converts an IP address from the standard text
   presentation format to the binary format. */
TM_PROTO_EXTERN int inet_pton(
    int                 af,
    const char TM_FAR * src,
    void TM_FAR *       dst );

/* Refer to [RFC2553]. Converts an IP address from the binary format to the
   standard text presentation format. */
TM_PROTO_EXTERN const char TM_FAR * inet_ntop(
    int                 af,
    const void TM_FAR * src,
    char TM_FAR *       dst,
    int                 size );

/* to read hexidecimail byte string, such as "112233445566778899aabb"
 * to hex byte array 0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb...
 */
TM_PROTO_EXTERN int tfHexByteStr2HexByteArr(const char TM_FAR * src,
                                            int                 len,
                                            char TM_FAR       * dst);


/*
 * Treck Extensions to BSD Sockets
 * ntohl and ntohs are macro's not functions here
 */


/*
 * Treck Extension Typedefs
 */
typedef ttUser32Bit              ttUserIpAddress;
typedef unsigned short           ttUserIpPort;
typedef void            TM_FAR * ttUserInterface;
typedef void            TM_FAR * ttUserMessage;
typedef void            TM_FAR * ttUserBuffer;
typedef ttUserBuffer    TM_FAR * ttUserBufferPtr;
typedef void            TM_FAR * ttUserVoidPtr;
typedef const void      TM_FAR * ttUserConstVoidPtr;
typedef ttUserIpAddress TM_FAR * ttUserIpAddressPtr;
typedef ttUserIpPort    TM_FAR * ttUserIpPortPtr;
typedef void            TM_FAR * ttUserLinkLayer;
typedef void (TM_CODE_FAR      * ttUserIsrHandlerPtr)(void);
typedef void            TM_FAR * ttUserContext;

/*
 * Generic Parameter Passing Union Typedef
 */
typedef union tuUserGenericUnion
{
    ttUser32Bit     gen32bitParm;
    ttUserS32Bit    genSlongParm;
    void TM_FAR    *genVoidParmPtr;
    unsigned short  gen16BitParm;
    short           genS16BitParm;
    unsigned int    genUintParm;
    int             genIntParm;
    unsigned char   gen8BitParm;
    char            genCharParm;
} ttUserGenericUnion;

typedef ttUserGenericUnion TM_FAR * ttUserGenericUnionPtr;


/*
 * Treck Sockets Extension Macros
 */

/* Blocking States for tfBlockingState() function */
#define TM_BLOCKING_OFF     0
#define TM_BLOCKING_ON      1

/* Address Lengths */
#define TM_ETHERNET_PHY_ADDR_LEN 6

/*
 * Link Layers
 */
#define TM_LINK_TUNNEL_MASK             0x800

/* Link layer Broadcast */
#define TM_LINK_BROADCAST_MASK          0x400

/* LAN Link Layer Mask */
#define TM_LINK_LAN_MASK                0x200

/* Pt2Pt Link Layer mask */
#define TM_LINK_PT2PT_MASK              0x100

/* Ethernet link layer */
#define TM_LINK_LAYER_ETHERNET          \
                                    (TM_LINK_LAN_MASK | TM_LINK_BROADCAST_MASK)

/* VLAN Tagged Ethernet link layer */
#define TM_LINK_LAYER_ETHER_TAG          \
                                    (TM_LINK_LAN_MASK | TM_LINK_BROADCAST_MASK)

/* PPP link layer */
#define TM_LINK_LAYER_PPP           (0x01 | TM_LINK_PT2PT_MASK)

/* SLIP link layer */
#define TM_LINK_LAYER_SLIP          (0x02 | TM_LINK_PT2PT_MASK)

/* TRANSP link layer */
#define TM_LINK_LAYER_TRANSP        (0x03 | TM_LINK_PT2PT_MASK)

/* Non LAN, non PT2PT (bit 0x100, and 0x200 not sets) */
#define TM_LINK_LAYER_NULL          (0x04 | TM_LINK_BROADCAST_MASK)

/* IPv6 tunnel virtual link layer type */
#define TM_LINK_LAYER_V6_TUNNEL     (0x05 | TM_LINK_TUNNEL_MASK)

/* Movile IPv6 MN node virtual link layer type */
#define TM_LINK_LAYER_MIPV6_MN_VIRTUAL \
                                    (0x06 | TM_LINK_TUNNEL_MASK)


/*
 * Link Layer Flags
 */
#define TM_LL_OKAY                  0x0000
#define TM_LL_OPEN_STARTED          0x0001
#define TM_LL_OPEN_COMPLETE         0x0002
#define TM_LL_OPEN_FAILED           0x0004
#define TM_LL_CLOSE_STARTED         0x0008
#define TM_LL_CLOSE_COMPLETE        0x0010
#define TM_LL_LCP_UP                0x0020
#define TM_LL_PAP_UP                0x0040
#define TM_LL_CHAP_UP               0x0080
#define TM_LL_IP4_OPEN_FAILED       0x0100
#define TM_LL_IP6_OPEN_FAILED       0x0200
#define TM_LL_IP4_OPEN_COMPLETE     0x0400
#define TM_LL_IP6_OPEN_COMPLETE     0x0800
#define TM_LL_EAP_UP                0x1000

/*
 * Flags for device configuration tfConfigInterface()
 * NOTE flags 0x1, 0x2, 0x4, 0x8, and 0x10 are reserved.
 */

/* This device can support multiple buffers per frame */
#define TM_DEV_SCATTER_SEND_ENB     0x0020
/* Ethernet multicast enabled on interface */
#define TM_DEV_MCAST_ENB            0x0040
/* IP forwarding enabled */
#define TM_DEV_IP_FORW_ENB          0x0080
 /* IP forwarding of directed broadcast enabled */
#define TM_DEV_IP_FORW_DBROAD_ENB   0x0100
/* User will finish configuration */
#define TM_DEV_IP_USER_BOOT         0x0200
/* Configure IP address via BOOTP protocol */
#define TM_DEV_IP_BOOTP             0x0400
/* Configure IP address via DHCP protocol */
#define TM_DEV_IP_DHCP              0x0800
/* Do not check that Destination IP address is on an interface */
#define TM_DEV_IP_NO_CHECK          0x1000
/* IP forwarding of multicast enabled */
#define TM_DEV_IP_FORW_MCAST_ENB    0x2000
/* Allow mcast destination send only through the routing table */
#define TM_DEV_IP_MCAST_ROUTE       0x4000
/* Device flag (i.e. ttDeviceEntry.devFlag) that indicates that the IP routing
   table must always be consulted when sending a solicited reply packet (i.e.
   ICMP Echo Reply) to a unicast destination IP address in response to a
   request packet (i.e. ICMP Echo Request) received on this device, since the
   outgoing device that the reply must be sent on may be different from the
   incoming device that the request was received on. */
#define TM_DEV_IP_ASYMMETRICAL      0x8000

/* Flags user is allowed to set in the flags field of tfConfigInterface() */
#define TM_DEV_USER_ALLOWED ( TM_DEV_SCATTER_SEND_ENB|TM_DEV_MCAST_ENB | \
                              TM_DEV_IP_FORW_ENB |                       \
                              TM_DEV_IP_FORW_DBROAD_ENB|                 \
                              TM_DEV_IP_USER_BOOT |                      \
                              TM_DEV_IP_BOOTP|TM_DEV_IP_DHCP |           \
                              TM_DEV_IP_NO_CHECK |                       \
                              TM_DEV_IP_FORW_MCAST_ENB |                 \
                              TM_DEV_IP_MCAST_ROUTE )

#define TM_DEV_BOOT_FLAGS   ( TM_DEV_IP_USER_BOOT |                      \
                              TM_DEV_IP_BOOTP     |                      \
                              TM_DEV_IP_DHCP        )

/*
 * tfSendCompleteInterface flag value when it is called from either
 * a send task, or the main loop.
 */
#define TM_DEV_SEND_COMPLETE_APP        0x00
/*
 * tfSendCompleteInterface flag value when it is called from the driver
 */
#define TM_DEV_SEND_COMPLETE_DRIVER     0x01
/*
 * tfSendCompleteInterface flag value when it is called from a device
 * driver send which was called by GHS INDRT.
 * (Always ORed with TM_DEV_SEND_COMPLETE_DRIVER, since it is always
 *  called from the driver.)
 */
#define TM_DEV_SEND_COMPLETE_INDRT      0x02
/*
 * tfSendCompleteInterface() flag value when it is called because a
 * previous call had been postponed, as indicated by a non zero
 * devSendCompltCount. (Always ORed with TM_DEV_SEND_COMPLETE_APP, since
 * it is always called from an application.)
 */
#define TM_DEV_SEND_COMPLETE_DELAYED    0x04

/* Defines for scattered data sending */
#define TM_USER_BUFFER_MORE             0x00
#define TM_USER_BUFFER_LAST             0x01
/*
 * Additional flag when call is made from GHS INDRT, i.e. ISR level. Has
 * to match TM_DEV_SEND_COMPLETE_INDRT. Always ORed with TM_USER_BUFFER_LAST,
 * as GHS INDRT data is never scattered.
 */
#define TM_USER_BUFFER_INDRT            0x02

/* Offset to start Ethernet header, to allow 4-byte alignment of IP header */
#define TM_ETHER_IP_ALIGN               2


/* No op on non DSP processor */
#define tm_packed_byte_count(bytes) (bytes)
/* No op on non DSP processor */
#define tm_byte_count(packedBytes)  (packedBytes)
/* No op on non DSP processor */
#define tm_packed_byte_len(bytes) (bytes)


/*
 * Returns a single 'byte' from an array of long (32-bit) words.
 * Primarily used when manipulating IPv6 addresses, currently.  While the
 * expression may look complicated, since the arguments will mostly be
 * constant, the preprocessor will simplify this.
 */
#define tm_dsp_get_byte_larr(arr, byteIdx) \
( ((arr)[byteIdx >> 2] >> ((3 - (byteIdx % 4)) << 3)) & 0xff)


#define TM_ETHER_HEADER_SIZE        14
#define TM_TAGGED_ETHER_HEADER_SIZE 18
#ifndef TM_VLAN_MHOME_INDEX
/* default mHome index for storing VLAN tags (1 per context) */
#define TM_VLAN_MHOME_INDEX 0 
#endif /* TM_VLAN_MHOME_INDEX */
/* Maximum ethernet packet size (including CRC) */
#define TM_ETHER_MAX_PACKET_CRC     1518
#define TM_TAGGED_ETHER_MAX_PACKET_CRC     1518

/*
 * String Compare Values
 */
#define TM_STRCMP_EQUAL   0
/* Used only to set in memcmp of similar function */
#define TM_STRCMP_LESS   -1
/* Used only to set in memcmp of similar function */
#define TM_STRCMP_GREATER 1

#ifdef TM_OPTIMIZE_SPEED
/* Read and write compatible functions as listed in the documentation */
#define tfRead(a,b,c)   recv(a, b, c, 0)
#define tfWrite(a,b,c)  send(a, b, c, 0)
#else /* !TM_OPTIMIZE_SPEED */

TM_PROTO_EXTERN int tfRead(
    int           socketDescriptor,
    char TM_FAR * bufferPtr,
    int           bufferLength );

TM_PROTO_EXTERN int tfWrite(
    int                 socketDescriptor,
    const char TM_FAR * bufferPtr,
    int                 bufferLength );
#endif /* TM_OPTIMIZE_SPEED */

/* Backward compatibility of read and write compatible functions */
#define tfread          tfRead
#define tfwrite         tfWrite

/*
 * Kernel Macros
 */
#define TM_KERN_ERROR     -1
#define TM_KERN_OKAY       0
#ifndef tm_thread_stop
#define tm_thread_stop     for(;;)
#endif /*tm_thread_stop */

/*
 * Device macros
 */
#define TM_DEV_ERROR       -1
#define TM_DEV_OKAY         0


/*
 * IP addresses macros
 * NOTE: ALl IP addresses macros assume that IP addresses are in
 * network byte order. All IP addresses comparison are
 * done on 32-bit quantities. (ttUserIpAddress is a 32-bit long.)
 */

#ifndef ntohl
#ifdef TM_LITTLE_ENDIAN

#define ntohl(x) (ttUserIpAddress)((ttUserIpAddress)(((x)>>24)&TM_UL(0xff))\
                              |(ttUserIpAddress)(((x)>>8)&TM_UL(0xff00))\
                              |(ttUserIpAddress)(((x)<<8)&TM_UL(0xff0000))\
                              |(ttUserIpAddress)(((x)<<24)&TM_UL(0xff000000)))

#else /* TM_LITTLE_ENDIAN */
#define ntohl(x) (ttUserIpAddress)(x)
#endif /* TM_LITTLE_ENDIAN */
#endif /* ntohl */

#ifndef ntohs
#ifdef TM_LITTLE_ENDIAN
#define ntohs(x)    (unsigned short)((unsigned short)(((x)>>8)&0xffU)\
                  | (unsigned short)(((x)<<8)&0xff00U) )


#else /* TM_LITTLE_ENDIAN */
#define ntohs(x) (unsigned short)(x)
#endif /* TM_LITTLE_ENDIAN */
#endif /* ntohs */

#ifndef htonl
#define htonl(x) ntohl(x)
#endif /* htonl */

#ifndef htons
#define htons(x) ntohs(x)
#endif /* htons */

/*
 * PPP Macros
 */

#define TM_PPP_OPT_WANT             0
#define TM_PPP_OPT_ALLOW            1

/*
 * LCP Options
 */
/* The maximum receive unit that we support RFC1661*/
#define TM_LCP_MAX_RECV_UNIT        1
/* The Async Control Character Map RFC1662*/
#define TM_LCP_ACCM                 2
/* The authentication protocol that we require to us RFC1661*/
#define TM_LCP_AUTH_PROTOCOL        3
/* The link quality protocol we wish to use RFC1661*/
#define TM_LCP_QUALITY_PROTOCOL     4
/* Magic Number for loopback Detection RFC1661*/
#define TM_LCP_MAGIC_NUMBER         5
/* Turn on protocol field compression RFC1661*/
#define TM_LCP_PROTOCOL_COMP        7
/* Turn on address/control field compression RFC1661*/
#define TM_LCP_ADDRCONTROL_COMP     8
/* LCP timeout, retry options */
#define TM_LCP_TERM_RETRY           9
#define TM_LCP_CONFIG_RETRY         10
#define TM_LCP_TIMEOUT              11
/* LCP maximum failure option */
#define TM_LCP_MAX_FAILURES         12

/*
 * IPCP Options
 */
/* The compression protocol that the requesting system wishes to use */
#define TM_IPCP_COMP_PROTOCOL       2
/* The IP Address the requesting system wishes to use */
#define TM_IPCP_IP_ADDRESS          3
/* The number of VJ compression slots to use */
#define TM_IPCP_VJ_SLOTS            4
/* IPCP Timeout, retry options */
#define TM_IPCP_RETRY               5
#define TM_IPCP_TIMEOUT             6
/* LCP maximum failure option */
#define TM_IPCP_MAX_FAILURES        7
/* RFC-2507 IPv4 header compression parameters. */
#define TM_IPCP_IPHC_TCP_SPACE      8
#define TM_IPCP_IPHC_NON_TCP_SPACE  9
#define TM_IPCP_IPHC_MAX_PERIOD     10
#define TM_IPCP_IPHC_MAX_TIME       11
#define TM_IPCP_IPHC_MAX_HEADER     12


/*
 * IPV6CP Options
 */
#define TM_IPV6CP_COMP_PROTOCOL      2
#define TM_IPV6CP_RETRY              3
#define TM_IPV6CP_TIMEOUT            4
#define TM_IPV6CP_MAX_FAILURES       5
/* RFC-2507 IPv6 header compression parameters. */
#define TM_IPV6CP_IPHC_TCP_SPACE     6
#define TM_IPV6CP_IPHC_NON_TCP_SPACE 7
#define TM_IPV6CP_IPHC_MAX_PERIOD    8
#define TM_IPV6CP_IPHC_MAX_TIME      9
#define TM_IPV6CP_IPHC_MAX_HEADER    10


/*
 * We overlap these with the option numbers since 29/31 are
 * unused by the stack to save space
 * We simple add 100 or subtract 100 when sending an receiving
 */
#define TM_IPCP_DNS_PRI     29
#define TM_IPCP_DNS_SEC     31

/*
 * LQM (Link Quality Monitoring) macros
 */

/* values for reasonCode passed to user-defined link quality
   monitoring function (refer to tfLqmRegisterMonitor) */
#define TM_LQM_MONITOR_LQR          0
#define TM_LQM_MONITOR_TIMEOUT      1

/* [RFC1989].R2.9:40
   max retransmission count for retransmission of Link-Quality-Report if we
   expect a LQR response from the peer and we receive no response. */
#define TM_LQM_RETX_MAX_RETRIES     1

/* PPP callback flag indicates that LQM has been enabled on the link */
#define TM_LL_LQM_UP        0x0100

/* PPP callback flag indicates that LQM has been disabled on the link */
#define TM_LL_LQM_DISABLED  0x0200

/* PPP callback flag indicates that link quality is bad and recovery
   should be attempted */
#define TM_LL_LQM_LINK_BAD  0x0400

/*
 * DNS Flags
 */
#define TM_DNS_PRIMARY          1
#define TM_DNS_SECONDARY        2


/*
 * PAP Options
 */
/* The local username to authenticate with */
#define TM_PAP_USERNAME 1
/* The local password to authenticate with */
#define TM_PAP_PASSWORD 2
/* PAP timeout, retry options */
#define TM_PAP_RETRY    3
#define TM_PAP_TIMEOUT  4


/* Authentication methods, user may add more methods (we call it methods
 * here not protocol, because we don't want confuse with macro like
 * TM_PPP_CHAP_PROTOCOL),
 * however, the value needs to be an integer between 1 and 8
 */
#define TM_PPP_AUTHMETHOD_MIN        (ttUser8Bit)0
#define TM_PPP_AUTHMETHOD_NULL       (ttUser8Bit)0
#define TM_PPP_AUTHMETHOD_EAP        (ttUser8Bit)1
#define TM_PPP_AUTHMETHOD_CHAP       (ttUser8Bit)2
#define TM_PPP_AUTHMETHOD_PAP        (ttUser8Bit)3
#define TM_PPP_AUTHMETHOD_MSCHAP_V1  (ttUser8Bit)4
#define TM_PPP_AUTHMETHOD_MAX        (ttUser8Bit)8

#define TM_PPP_AUTHPRIORITY_MIN      (ttUser8Bit)0x0
#define TM_PPP_AUTHPRIORITY_MAX      (ttUser8Bit)0x0f


/*
 * CHAP options
 */
/* The local username to authenticate with */
#define TM_CHAP_USERNAME 1
/* The local secret to authenticate with */
#define TM_CHAP_SECRET   2
/* CHAP timeout, retry options */
#define TM_CHAP_RETRY    3
#define TM_CHAP_TIMEOUT  4
/* Add CHAP auth algorihtms */
#define TM_CHAP_ALG_ADD  5
/* Delete CHAP auth algorihtms */
#define TM_CHAP_ALG_DEL  6
/* MS-chap password */
#define TM_CHAP_MSSECRET 7

/* CHAP encryption types (MD5,DES,etc) */
#define TM_CHAP_MD5          0x05
#define TM_CHAP_MSV1         0x80
#define TM_CHAP_MSV2         0x81

/* When DSP processes unpacked data, we don't need to apply
 * the method designed for packed data. For example, "AB" in
 * packed format is '4142 00xx', we need use tm_md5_get_char_next
 * to get the next position. HOwever, if unpacked format,
 * say 16bit dsp, '0041 0042', we just call 'v=pointer++'
 */
#define TM_DSP_UNPACKED_CHAR    0x09
/*
 * General PPP options
 */
#define TM_PPP_SEND_BUFFER_SIZE 1
#define TM_PPP_OPEN_TIMEOUT     2

/*
 * PPP Protocols
 */
#ifndef TM_LITTLE_ENDIAN
#define TM_PPP_PROTOCOL                 (unsigned short)(0x0000)
#define TM_PPP_LCP_PROTOCOL             (unsigned short)(0xc021)
#define TM_PPP_IPCP_PROTOCOL            (unsigned short)(0x8021)
#define TM_PPP_IPV6CP_PROTOCOL          (unsigned short)(0x8057)
#define TM_PPP_PAP_PROTOCOL             (unsigned short)(0xc023)
#define TM_PPP_CHAP_PROTOCOL            (unsigned short)(0xc223)
#define TM_PPP_COMP_TCP_PROTOCOL        (unsigned short)(0x002d)
#define TM_PPP_UNCOMP_TCP_PROTOCOL      (unsigned short)(0x002f)
#define TM_PPP_IP_PROTOCOL              (unsigned short)(0x0021)
#define TM_PPP_IPV6_PROTOCOL            (unsigned short)(0x0057)
#define TM_PPP_LQR_PROTOCOL             (unsigned short)(0xc025)
#define TM_PPP_IPHC_PROTOCOL            (unsigned short)(0x0061)
#define TM_PPP_EAP_PROTOCOL             (unsigned short)(0xc227)
#else /* TM_LITTLE_ENDIAN */
#define TM_PPP_PROTOCOL                 (unsigned short)(0x0000)
#define TM_PPP_LCP_PROTOCOL             (unsigned short)(0x21c0)
#define TM_PPP_IPCP_PROTOCOL            (unsigned short)(0x2180)
#define TM_PPP_IPV6CP_PROTOCOL          (unsigned short)(0x5780)
#define TM_PPP_PAP_PROTOCOL             (unsigned short)(0x23c0)
#define TM_PPP_CHAP_PROTOCOL            (unsigned short)(0x23c2)
#define TM_PPP_COMP_TCP_PROTOCOL        (unsigned short)(0x2d00)
#define TM_PPP_UNCOMP_TCP_PROTOCOL      (unsigned short)(0x2f00)
#define TM_PPP_IP_PROTOCOL              (unsigned short)(0x2100)
#define TM_PPP_IPV6_PROTOCOL            (unsigned short)(0x5700)
#define TM_PPP_LQR_PROTOCOL             (unsigned short)(0x25c0)
#define TM_PPP_IPHC_PROTOCOL            (unsigned short)(0x6100)
#define TM_PPP_EAP_PROTOCOL             (unsigned short)(0x27c2)
#endif /* TM_LITTLE_ENDIAN */

/*
 * Treck TCP/IP Stack Options
 */
/*
 * Options that can only be used with tfInitTreckOptions()
 */
/* Option to set Tick length */
#define TM_OPTION_TICK_LENGTH            1
/* Option to set Maximum number of sockets on the system */
#define TM_OPTION_SOCKETS_MAX            2
/*
 * Option to set maximum of user DHCP entries per interface (not including
 * automatic tfConfigInterface DHCP configuration entries).
 */
#define TM_OPTION_DHCP_MAX_ENTRIES       3

/*
 * Options that can be used with tfInitTreckOptions() or tfSetTreckOptions()
 */
#define TM_OPTION_ARP_MAX_RETRY          4
#define TM_OPTION_ARP_TIMEOUT_TIME       5
#define TM_OPTION_ARP_QUIET_TIME         6
#define TM_OPTION_ARP_TTL                7
#define TM_OPTION_ROUTER_ID              8
#define TM_OPTION_ROUTER_AGE_LIMIT       9
#define TM_OPTION_RIP_ENABLE            10
#define TM_OPTION_RIP_SEND_MODE         11
#define TM_OPTION_RIP_RECV_MODE         12
#define TM_OPTION_IP_FORWARDING         13
#define TM_OPTION_IP_DBCAST_FORWARD     14
#define TM_OPTION_IP_FRAGMENT           15
#define TM_OPTION_IP_TTL                16
#define TM_OPTION_IP_TOS                17
#define TM_OPTION_IP_FRAG_TTL           18
#define TM_OPTION_ICMP_ADDR_MASK_AGENT  19
#define TM_OPTION_UDP_CHECKSUM          20
#define TM_OPTION_ARP_MAX_ENTRIES       21
#define TM_OPTION_ROUTE_MAX_ENTRIES     22
#define TM_OPTION_DECREASED_PMTU_TTL    23
#define TM_OPTION_LARGER_PMTU_TTL       24
#define TM_OPTION_ARP_SMART             25
/* Option to change the default maximum number of NAT routing entries */
#define TM_OPTION_NAT_MAX_ENTRIES       26
#define TM_OPTION_SEND_TRAILER_SIZE     27
#define TM_OPTION_TIMER_MAX_EXECUTE     28
/*
 * Option to change the maximum number of IP datagrams waiting to be
 * reassembled. If any fragment of a new datagram arrives when the
 * maximum number of IP datagrams waiting to be reassembled has been reached,
 * it is dropped. Default value is 5.
 */
#define TM_OPTION_IP_FRAG_MAX_Q_SIZE     29
/*
 * Option to change the maximum size of an IP datagram waiting to be
 * reassembled. Beyond that size, we drop the entire IP datagram. Default value
 * is 8200 bytes.
 */
#define TM_OPTION_IP_FRAG_ENTRY_MAX_SIZE 30

/* resolution (in milliseconds) of timer used to age ARP cache entries */
#define TM_OPTION_RT_TIMER_RES_MSEC      31

/* Option to set Maximum number of TCP sockets on the system */
#define TM_OPTION_TCP_SOCKETS_MAX        32




/*
 * socket call back function flag
 */
#define TM_CB_CONNECT_COMPLT            0x0001
#define TM_CB_ACCEPT                    0x0002
#define TM_CB_RECV                      0x0004
#define TM_CB_RECV_OOB                  0x0008
#define TM_CB_SEND_COMPLT               0x0010
#define TM_CB_REMOTE_CLOSE              0x0020
#define TM_CB_SOCKET_ERROR              0x0040
#define TM_CB_RESET                     0x0080
#define TM_CB_CLOSE_COMPLT              0x0100
#define TM_CB_WRITE_READY               0x0200
#define TM_CB_SSL_ESTABLISHED           0x0400
#define TM_CB_SSL_SEND_CLOSE            0x0800
#define TM_CB_SSL_RECV_CLOSE            0x1000

/*
 * Treck Sockets Extension Function Prototypes
 */

/* Initialize and Start the Treck TCP/IP Protocol Stack*/
/*
 * This is the main routine that must be called prior to any other
 * call into the stack.  This routine will initialize the stack variables,
 * socket table etc.
 */
TM_PROTO_EXTERN int tfStartTreck(void);

/*
 * Set various options for the TCP/IP stack
 * AFTER a call to tfStartTreck()
 */
TM_PROTO_EXTERN int tfSetTreckOptions(
    int         optionName,
    ttUser32Bit optionValue );

/*
 * Set various options for the TCP/IP stack
 * BEFORE a call to tfStartTreck()
 */
TM_PROTO_EXTERN int tfInitTreckOptions( int         optionName,
                                        ttUser32Bit optionValue );

/*
 * Free all memory allocated dynamically by the Treck internal memory
 * management system. Called by the user, when the user does not
 * want to use the treck stack anymore, and wants all currently unused
 * memory to be returned to the user's system.
 */
TM_PROTO_EXTERN int tfFreeDynamicMemory(void);

/*
 * Free memory allocated dynamically by the Treck internal memory
 * management system. Only free memory queues whose sizes are within
 * minSize, and maxSize.
 * Note that the user can set -1 for maxSize to not specify an upper bound.
 * Note that calling tfFreeRangeDynamicMemory(0, -1) will yield the
 * same results as calling tfFreeDynamicMemory().
 * Called by the user, when the wants currently unused
 * memory to be returned to the user's system.
 */
TM_PROTO_EXTERN int tfFreeRangeDynamicMemory(int minSize, int maxSize);

#ifdef TM_ERROR_CHECKING
void tfLockCheck(void);
#endif /* TM_ERROR_CHECKING */

/* Add an Interface */
/*
 * This function is the main function to use when adding an interface to
 * the Treck TCP/IP system.  Note: Interface are added in the CLOSED state
 * An example would be as follows:
 *
 * tfAddInterface("NE2000.001",
 *                linkLayerHandle,
 *                NE2kOpen,
 *                NE2kClose,
 *                NE2KSend,
 *                NE2kRecv,
 *                (ttDevFreeRecvFuncPtr)0,
 *                NE2kIoctl,
 *                NE2KGetPhysicalAAddr,
 *                &errorValue);
 *
 */
typedef int (TM_CODE_FAR *ttDevOpenCloseFuncPtr)(
    ttUserInterface interfaceHandle );

typedef int (TM_CODE_FAR *ttDevSendFuncPtr)(
    ttUserInterface interfaceHandle,
    char   TM_FAR * dataPtr,
    int             dataLength,
    int             flag );

#ifdef TM_DEV_RECV_OFFLOAD
typedef struct tsDevRecvOffloadStruct
{
/*
 * One of more of the TM_DEVOR_XXX flags as defined in this file.
 */
    ttUser16Bit devoRFlags;
/* Recv checksum as computed by the board */
    ttUser16Bit devoRecvXsum;
/* Offset to start of checksum computation */
    ttUser16Bit devoRecvXsumOffset;
/* Checksum done on this amount of data */
    ttUser16Bit devoRecvXsumLength;
} ttDevRecvOffloadStruct;
typedef ttDevRecvOffloadStruct TM_FAR * ttDevRecvOffloadPtr;
#endif /* TM_DEV_RECV_OFFLOAD */


typedef int (TM_CODE_FAR *ttDevRecvFuncPtr) (
    ttUserInterface            interfaceHandle,
    char     TM_FAR * TM_FAR * dataPtr,
    int               TM_FAR * dataLength,
    ttUserBufferPtr            userBufferHandlePtr
#ifdef TM_DEV_RECV_OFFLOAD
  , ttDevRecvOffloadPtr        devRecvOffloadPtr
#endif /* TM_DEV_RECV_OFFLOAD */
                                            );

typedef int (TM_CODE_FAR *ttDevFreeRecvFuncPtr)(
    ttUserInterface    interfaceHandle,
    char      TM_FAR * dataPtr );

typedef int (TM_CODE_FAR *ttDevIoctlFuncPtr)(
    ttUserInterface interfaceHandle,
    int             flag,
    void   TM_FAR * optionPtr,
    int             optionLen );

typedef int (TM_CODE_FAR *ttDevGetPhysAddrFuncPtr)(
    ttUserInterface  interfaceHandle,
    char    TM_FAR * physicalAddress );

typedef int (TM_CODE_FAR * ttPapAuthenticateFunctPtr)(
    char TM_FAR * username,
    char TM_FAR * password );

typedef char TM_FAR * (TM_CODE_FAR * ttChapAuthenticateFunctPtr)(
    char TM_FAR * username );

TM_PROTO_EXTERN ttUserInterface tfAddInterface (
    const char    TM_FAR  * namePtr,
    ttUserLinkLayer         linkLayerHandle,
    ttDevOpenCloseFuncPtr   drvOpenFuncPtr,
    ttDevOpenCloseFuncPtr   drvCloseFuncPtr,
    ttDevSendFuncPtr        drvSendFuncPtr,
    ttDevRecvFuncPtr        drvRecvFuncPtr,
    ttDevFreeRecvFuncPtr    drvFreeRecvFuncPtr,
    ttDevIoctlFuncPtr       drvIoctlFuncPtr,
    ttDevGetPhysAddrFuncPtr drvGetPhysAddrFuncPtr,
    int            TM_FAR * drvAddErrorPtr );

/*
 * (* drvIoctlFunctPtr)() flags parameter value for IGMP multicast support
 */
/* Set the device to receive all multicast addresses provided in a list */
#define TM_DEV_SET_MCAST_LIST          0x1000
/* Set the device to receive all multicast addresses */
#define TM_DEV_SET_ALL_MCAST           0x2000

/*
 * Device driver scattered send buffer interface using a single call
 * to the device driver send with a linked list of packets.
 */

/*
 * Packet length type. NOTE if this is changed check for alignment in
 * all structures containing it
 */
typedef unsigned ttPktLen;

#ifdef TM_DEV_SEND_OFFLOAD
typedef struct tsDevSendOffloadStruct
{
/*
 * One of more of the device offload flags as defined in the
 * TM_DEV_IOCTL_OFFLOAD_SET tfIoctlInterface().
 * Note that a TM_DEVO_TCP_SEGMENT_OFFLOAD_V4 flag, indicates an IPv4 protocol
 * and a TM_DEVO_TCP_SEGMENT_OFFLOAD_V6 indicates an IPv6 protocol (needed for
 * segmentation)
 */
    ttUser16Bit devoFlags;
/* TCP/IP or UDP/IP header length to be used in each frame */
    ttUser16Bit devoFrameHdrLen;
/* Total TCP payload (used in segmentation) not including header */
    ttUser32Bit devoPayload;
/* Maximum TCP or UDP payload segment per frame not including header */
    ttUser16Bit devoFramePayload;
/* UDP/TCP header checksum */
    ttUser16Bit devoTucss; /* offset to first byte to be included in Xsum */
    ttUser16Bit devoTucso; /* Offset to Xsum field */
    ttUser16Bit devoTucse; /* Offset to last byte to be in included in Xsum */
/* IP header checksum */
    ttUser16Bit devoIpcss; /* offset to first byte to be included in Xsum */
    ttUser16Bit devoIpcso; /* Offset to Xsum field */
    ttUser16Bit devoIpcse; /* Offset to last byte to be in included in Xsum */
    ttUser16Bit devoUnusedField; /* Multiple of 4 bytes */
} ttDevSendOffloadStruct;

typedef ttDevSendOffloadStruct TM_FAR * ttDevSendOffloadPtr;
#endif /* TM_DEV_SEND_OFFLOAD */

/*
 * Data type used for the linked list of packets passed to the device
 * driver single call scattered send function prototype.
 */
typedef struct tsUserPacket
{
/* Next tsUserPacket for scattered data  */
/*    struct tsUserPacket TM_FAR   *pktuLinkNextPtr; */
    void               TM_FAR   *pktuLinkNextPtr;
/* Pointer to data */
    ttUser8BitPtr                pktuLinkDataPtr;
#if defined(TM_DEV_SEND_OFFLOAD) || defined(TM_DEV_RECV_OFFLOAD)
    ttUserVoidPtr                pktuDevOffloadPtr;
#endif /* TM_DEV_SEND_OFFLOAD || TM_DEV_RECV_OFFLOAD */
/* Size of data pointed by pktuLinkDataPtr */
    ttPktLen                     pktuLinkDataLength;
/*
 * Total packet length (of chained scattered data). Valid in first link
 * only
 */
    ttPktLen                     pktuChainDataLength;
/*
 * Number of links linked to this one (not including this one).
 * Valid in first link only.
 */
    int                          pktuLinkExtraCount;
} ttUserPacket;

typedef ttUserPacket TM_FAR * ttUserPacketPtr;

#ifdef TM_USE_DRV_ONE_SCAT_SEND
/*
 * Device driver single call scattered send function prototype.
 */
typedef int (TM_CODE_FAR *ttDevOneScatSendFuncPtr)(
    ttUserInterface interfaceHandle,
    ttUserPacketPtr packetUPtr );

/*
 * Specify a driver driver scattered send function, that is called with
 * a linked list of packets on a given interface.
 * Must be called prior to calling tfConfigInterface, and after having called
 * tfAddInterface. The device send logic in the Turbo Treck stack will call
 * this driver send function, instead of the one provided in tfAddInterface(),
 * if this call is made.
 * Note that this call is not supported for Point to Point interface, and is
 * not supported in combination with an interface transmit queue.
 */
TM_PROTO_EXTERN int tfUseInterfaceOneScatSend(
    ttUserInterface         interfaceHandle,
    ttDevOneScatSendFuncPtr drvOneScatSendFuncPtr );
#endif /* TM_USE_DRV_ONE_SCAT_SEND */

#ifdef TM_USE_DRV_SCAT_RECV
/*
 * . Data type to convey information from the device driver scattered
 *   recv function to the tfRecvScatInterface() API.
 */
typedef struct tsDruBlock
{
#define druBufferPtr      druBufferUnion.druUnBufferPtr
#define druStackBufferPtr druBufferUnion.druUnStackBufferPtr
    union
    {
/*
 * Device Driver Scattered Recv API:
 * . using non pre-allocated device driver buffers, or
 * . using Turbo Treck pre-allocated device driver buffer (from
 *   tfGetDriverBuffer/tfGetEthernetBuffer) (pointer to buffer as gotten
 *   from the first parameter of tfGetDriverBuffer/tfGetEthernetBuffer).
 *
 * Pointer to buffer that will be passed to the device driver scattered
 * free routine (non pre-allocated device driver buffers, or to the
 * stack (pre-allocated device driver buffers)
 */
        char     TM_FAR * druUnBufferPtr;
        ttUserBufferPtr   druUnStackBufferPtr;
    } druBufferUnion;
/* Pointer to beginning of data */
    char TM_FAR * druDataPtr;
/* Data length */
    int           druDataLength;
} ttDruBlock;

typedef ttDruBlock    TM_FAR * ttDruBlockPtr;
typedef ttDruBlockPtr TM_FAR * ttDruBlockPtrPtr;


#define TM_DEV_SCAT_RECV_USER_BUFFER  1
#define TM_DEV_SCAT_RECV_STACK_BUFFER 2

/*
 * Allowable flag value set in the flag parameter of the
 * (*drvScatRecvFuncPtr) call.
 */
#define TM_DEV_SCAT_RECV_FLAGS \
    ( TM_DEV_SCAT_RECV_USER_BUFFER  | TM_DEV_SCAT_RECV_STACK_BUFFER )

/*
 * device driver scattered recv function prototype. Allows the user
 * to pass scattered buffers up the stack, when tfRecvScatInterface() is
 * called. Flag should be set to
 * TM_DEV_SCAT_RECV_USER_BUFFER if the user used its own buffers,
 * or
 * TM_DEV_SCAT_RECV_STACK_BUFFER if the user used pre-allocated stack buffers.
 */
TM_PROTO_EXTERN typedef int (
    TM_CODE_FAR *       ttDevScatRecvFuncPtr)(
    ttUserInterface     nterfaceHandle,
    ttDruBlockPtrPtr    uDevBlockPtrPtr,
    int TM_FAR *        uDevBlockCountPtr,
    int TM_FAR *        flagPtr
#ifdef TM_DEV_RECV_OFFLOAD
  , ttDevRecvOffloadPtr devRecvOffloadPtr
#endif /* TM_DEV_RECV_OFFLOAD */
    );

/*
 * Specify a driver driver scattered recv function. This enables the device
 * driver to use scattered recv buffers.
 * User need to call tfUseInterfaceScatRecv between the
 * tfAddInterface, and tfOpenInterface/tfConfigInterface calls.
 */
TM_PROTO_EXTERN int tfUseInterfaceScatRecv(
    ttUserInterface      interfaceHandle,
    ttDevScatRecvFuncPtr drvScatRecvFunc );

/*
 * If the user has called tfUseInterfaceScatRecv:
 * When user has been notified that an incoming packet has arrived (via
 * either tfCheckReceiveInterface(), or tfWaitReceiveInterface()), it
 * then calls tfRecvScatInterface() so that the stack can call the
 * driver scattered receive function, and retrieve the data
 * If the user has not called tfUseInterfaceScatRecv, then tfRecvInterface()
 * shoudl be used instead. See below.
 */
TM_PROTO_EXTERN int tfRecvScatInterface(ttUserInterface interfaceHandle);

#endif /* TM_USE_DRV_SCAT_RECV */

#ifdef TM_KERNEL_LINUX_APP
/*
 * Add a Linux PF_PACKET as driver below link layer.
 * Retuns an interface handle
 */
TM_PROTO_EXTERN ttUserInterface tfUseLinuxDriver (
    char TM_FAR *    namePtr,
    ttUserLinkLayer  linkLayerHandle,
    int  TM_FAR *    errorCodePtr );
#endif /* TM_KERNEL_LINUX_APP */

/* Add the WinPcap dirver below the link layer */
TM_PROTO_EXTERN ttUserInterface tfUseWin32Driver(
    char TM_FAR *    namePtr,
    ttUserLinkLayer  linkLayerHandle,
    int  TM_FAR *    errorCodePtr );

/* Free all previously added WinPcap drivers */
TM_PROTO_EXTERN void tfFreeWin32Drivers(void);

/* Add a loop back driver below link layer. Retuns an interface handle */
TM_PROTO_EXTERN ttUserInterface tfUseIntfDriver (
    char TM_FAR *    namePtr,
    ttUserLinkLayer  linkLayerHandle,
    int  TM_FAR *    errorCodePtr );

/*
 * Add a loop back driver that supports multiple context below link layer.
 * Retuns an interface handle
 */
TM_PROTO_EXTERN ttUserInterface tfUseIntfMCtxDriver(
    char TM_FAR *    namePtr,
    ttUserLinkLayer  linkLayerHandle,
    int  TM_FAR *    errorCodePtr );

/*
 * Add a loop back driver below link layer using one scattered device
 * driver send call, and scattered device driver recv.
 * Retuns an interface handle.
 */
ttUserInterface tfUseScatIntfDriver (
    char TM_FAR *    namePtr,
    ttUserLinkLayer  linkLayerHandle,
    int  TM_FAR *    errorCodePtr );

typedef void (TM_CODE_FAR * ttDevNotifyFuncPtr)(
    ttUserInterface interfaceHandle,
    int             errorCode);
#define TM_DEV_NOTIFY_FUNC_NULL_PTR  (ttDevNotifyFuncPtr)0


/* Use BOOTP protocol */
TM_PROTO_EXTERN int tfUseBootp(
    ttUserInterface         interfaceHandle,
    ttDevNotifyFuncPtr      intfNotifyFuncPtr );

/* Use DHCP protocol */
TM_PROTO_EXTERN int tfUseDhcp(
    ttUserInterface     interfaceHandle,
    ttDevNotifyFuncPtr  intfNotifyFuncPtr );

/*
 * Configure an interface with an IP address. Can only be used for first
 * IP address configuration on the interface. For additional multi-home
 * IP addresses, use tfConfigInterface().
 */
TM_PROTO_EXTERN int tfOpenInterface(
    ttUserInterface interfaceId,
    ttUserIpAddress ipAddress,
    ttUserIpAddress netMask,
    int             flags,
    int             buffersPerFrameCount );

/* Pointer to the user-defined notification function to call to notify the
   user of IPv6-specific address configuration events. */
typedef void (TM_CODE_FAR *tt6DevAddrNotifyFuncPtr) (
    ttUserInterface                     interfaceHandle,
    unsigned int                        multiHomeIndex,
    struct sockaddr_storage TM_FAR *    ipv6AddrPtr,
    int                                 event );
#define TM_6_DEV_ADDR_NOTIFY_FUNC_NULL_PTR (tt6DevAddrNotifyFuncPtr) 0

/* This function is a replacement for tfOpenInterface to support IPv6. */
TM_PROTO_EXTERN int tfNgOpenInterface(
    ttUserInterface                         interfaceHandle,
    const struct sockaddr_storage TM_FAR *  ipAddrPtr,
    int                                     prefixLen,
    int                                     flags,
    int                                     ipv6Flags,
    int                                     buffersPerFrameCount,
    tt6DevAddrNotifyFuncPtr                 dev6AddrNotifyFuncPtr );

/*
 * Configure an interface with an IP address. Can be used for additional
 * IP Addresses on an Interface (Multihoming). For first IP address
 * configuration (first multi-home), use tfOpenInterface instead.
 */
TM_PROTO_EXTERN int tfConfigInterface(
    ttUserInterface interfaceId,
    ttUserIpAddress ipAddress,
    ttUserIpAddress netMask,
    int             flags,
    int             buffersPerFrameCount,
    unsigned char   multiHomeIndex );

/* This function is a replacement for tfConfigInterface to support IPv6. */
TM_PROTO_EXTERN int tfNgConfigInterface(
    ttUserInterface                         interfaceHandle,
    const struct sockaddr_storage TM_FAR *  ipAddrPtr,
    int                                     prefixLen,
    int                                     flags,
    int                                     ipv6Flags,
    int                                     buffersPerFrameCount,
    tt6DevAddrNotifyFuncPtr                 dev6AddrNotifyFuncPtr,
    unsigned int                            multiHomeIndex );

/*
 * User finish configuring an Interface. Only valid if user
 * called tfOpenInterface with TM_DEV_IP_USER_BOOT flag.
 */
TM_PROTO_EXTERN int tfFinishOpenInterface(
    ttUserInterface interfaceHandle,
    ttUserIpAddress ipAddress,
    ttUserIpAddress ipNetMask );

/*
 * Remove IP address from an interface for a particular multi home index.
 */
TM_PROTO_EXTERN int tfUnConfigInterface(
    ttUserInterface interfaceHandle,    /* Interface */
    unsigned char   multiHomeIndex );   /* Index for IP Address */

/* This function is a replacement for tfUnConfigInterface to support IPv6. */
TM_PROTO_EXTERN int tfNgUnConfigInterface(
    ttUserInterface interfaceHandle,
    int             addrFamily,
    unsigned int    multiHomeIndex );

/* Close an Interface will shutdown driver and remove the ISR */
/* For PPP this will also attempt to shutdown the connection */
TM_PROTO_EXTERN int tfCloseInterface(ttUserInterface interfaceHandle);

/*
 * Wait for outgoing data to be transmitted, if there is none, then wait.
 * when tfWaitXmitInterface returns, the user can then call
 * tfXmitInterface() so that the stack can transmit the next packet ready
 * to be sent in the interface send queue.
 * VERY IMPORTANT:
 * This is task pending method. Can only be used when there is a separate
 * transmit task, and TM_TASK_XMIT is defined in trsystem.h.
 * Also the user has to set the option TM_DEV_OPTIONS_XMIT_TASK with
 * tfInterfaceSetOptions(), to let the treck stack know that it needs
 * to interface to a transmit task, instead of the driver.
 */
TM_PROTO_EXTERN int tfWaitXmitInterface(ttUserInterface interfaceHandle);

/*
 * Check to see if data is waiting to be transmitted (polling method)
 * When tfCheckXmitInterface returns 0, the user can then call
 * tfXmitInterface() so that the stack can transmit the next packet ready
 * to be sent in the interface send queue.
 * VERY IMPORTANT:
 * This is task polling method. Can only be used if the user has set
 * the option TM_DEV_OPTIONS_XMIT_TASK with tfInterfaceSetOptions(), to
 * let the treck stack know that it needs to interface to a transmit task,
 * instead of the driver.
 * Returns TM_ENOERROR    if there is a packet ready to be transmitted,
 *         TM_EWOULDBLOCK if there is no packet ready to be transmitted
 */
TM_PROTO_EXTERN int tfCheckXmitInterface(ttUserInterface interfaceHandle);

/*
 * Transmit a packet from the Interface. Called from the transmit task
 * by the user. Used in conjunction with either tfCheckXmitInterface(), or
 * tfWaitXmitInterface().
 * Returns:
 * TM_ENOERROR A packet was successfully sent to the driver.
 * TM_ENOENT   No packet waiting to be sent
 * TM_ENXIO    Device was not opened/configured.
 * TM_EIO      At least the last buffer in the packet was not sent by the
 *             device driver. It was then removed from the send queue,
 *             and freed.
 */
TM_PROTO_EXTERN int tfXmitInterface(ttUserInterface interfaceHandle);

/*
 * Called from the device driver send routine to yield the CPU to
 * allow spin lock while waiting for room to transmit the data.
 * This function will release the device driver lock, and yield the CPU,
 * but only if the user is using a transmit
 * task to interface between the Treck stack and the device driver,
 * i.e the user set the option TM_DEV_OPTIONS_XMIT_TASK with
 * tfInterfaceSetOptions(). Otherwise the function will just return.
 */
TM_PROTO_EXTERN int tfInterfaceSpinLock(ttUserInterface interfaceHandle);

/*
 * tfNotifyInterfaceIsr() function description
 * Function Called by driver ISR routine.
 * Replaces both tfNotifyReceiveInterfaceIsr() and tfNotifySentInterfaceIsr()
 * which are now obsolote.
 * VERY IMPORTANT: should be only called once per ISR.
 * Notify the user with the number of packets received in one ISR,
 * and the number of packets that have been transmitted by the chip.
 * 1. If number of packets received is non zero:
 *   Will cause:
 *    tfCheckReceiveInterface to return 0 (polling check method),
 *   or
 *    tfWaitReceiveInterface (task pending check method) to return.
 * 2. If number of packets transmitted by the chip is non zero:
 *    Will cause:
 *     tfCheckSentInterface to return 0 (polling check method),
 *    or
 *     tfWaitSentInterface (task pending check method) to return.
 *    But only if number of bytes that have been transmitted reaches a
 *    configurable threshold tvNotifySendCompLowWater.
 * Parmaeters:
 * interfaceHandle           Interface handle as returned by tfAddInterface()
 * numberRecvPackets         Count of Number of packets received in the ISR.
 *                           A zero value indicates that the ISR did not
 *                           get any recv packet hit.
 * numberSendCompletePackets Total number of send complete packets. A zero
 *                           value indicates that the ISR did not get any
 *                           send complete hit.
 * totalNumberBytesSent      Total number of bytes sent in the send complete
 *                           packets.
 * Return
 * No return
 */
void tfNotifyInterfaceIsr (
    ttUserInterface interfaceHandle,
    int             numberRecvPackets,
    int             numberSendCompletePackets,
    ttUser32Bit     totalNumberBytesSent,
    ttUser32Bit     flag );

/*
 * tfNotifyInterfaceTask() function description
 * Function Called by driver, but NOT from an ISR routine.
 * VERY IMPORTANT: should be called from a driver routine, but NOT from an ISR
 * Only to be used if the chip fails to interrupt for events such as received
 * packets, or send completions. Should not be used, if the chip is not buggy.
 * Notify the user with the number of packets received
 * and the number of packets that have been transmitted by the chip.
 * 1. If number of packets received is non zero:
 *   Will cause:
 *    tfCheckReceiveInterface to return 0 (polling check method),
 *   or
 *    tfWaitReceiveInterface (task pending check method) to return.
 * 2. If number of packets transmitted by the chip is non zero:
 *    Will cause:
 *     tfCheckSentInterface to return 0 (polling check method),
 *    or
 *     tfWaitSentInterface (task pending check method) to return.
 *    But only if number of bytes that have been transmitted reaches a
 *    configurable threshold tvNotifySendCompLowWater.
 * Parmaeters:
 * interfaceHandle           Interface handle as returned by tfAddInterface()
 * numberRecvPackets         Count of Number of packets received in the ISR.
 *                           A zero value indicates that the ISR did not
 *                           get any recv packet hit.
 * numberSendCompletePackets Total number of send complete packets. A zero
 *                           value indicates that the ISR did not get any
 *                           send complete hit.
 * totalNumberBytesSent      Total number of bytes sent in the send complete
 *                           packets.
 * Return
 * No return
 */
void tfNotifyInterfaceTask (
    ttUserInterface interfaceHandle,
    int             numberRecvPackets,
    int             numberSendCompletePackets,
    ttUser32Bit     totalNumberBytesSent,
    ttUser32Bit     flag );


/*
 * Called by driver ISR routine.
 * (DEPRECATED. Use tfNotifyInterfaceIsr() instead.)
 * Notify the user that there have been packets sent so that they can call
 * send complete
 * Will cause:
 *  tfCheckSentInterface to return 0 (polling check method),
 * or
 *  tfWaitSentInterface (task pending check method) to return.
 */
TM_PROTO_EXTERN void tfNotifySentInterfaceIsr(
    ttUserInterface interfaceHandle,
    int             bytesSent );

/*
 * Check to see if data has been sent and is waiting to be freed
 */
TM_PROTO_EXTERN int tfCheckSentInterface(ttUserInterface interfaceHandle);

/*
 * Wait for incoming data, if there is none, then wait
 * VERY IMPORTANT:
 * Can only be used when there is a separate send task (TM_TASK_SEND
 * defined in trsystem.h).
 */
TM_PROTO_EXTERN int tfWaitSentInterface(ttUserInterface interfaceHandle);

/*
 * Notify stack that send has completed, so that the stack can remove
 * the first packet from the send queue, and free it.
 * VERY IMPORTANT:
 * . flag should be set to TM_DEV_SEND_COMPLETE_DRIVER if
 *   tfSendCompleteInterface is called from any device driver routine (
 *   in line send complete mechanism).
 * . flag should be set to TM_DEV_SEND_COMPLETE_APP if
 *   tfSendCompleteInterface is called from either a send task, or the main
 *   loop (tfWaitSentInterface(), or tfCheckSentInterface() mechanism).
 */
TM_PROTO_EXTERN void tfSendCompleteInterface(
    ttUserInterface interfaceHandle,
    int             devDriverLockFlag );

/*
 * Notify stack that send has completed for a given packetPtr, so that the
 * stack can remove it from the send queue, and free it.
 * VERY IMPORTANT:
 * . flag should be set to TM_DEV_SEND_COMPLETE_DRIVER if
 *    tfSendCompletePacketInterface is called from any device driver routine
 *    (in line send complete mechanism).
 * . flag should be set to TM_DEV_SEND_COMPLETE_APP if
 *    tfSendCompletePacketInterface is called from either a send task, or the
 *    main loop (tfWaitSentInterface(), or tfCheckSentInterface() mechanism).
 */
TM_PROTO_EXTERN void tfSendCompletePacketInterface(
    ttUserInterface interfaceHandle,
    ttUserPacketPtr packetPtr,
    int             devDriverLockFlag );


/*
 * Called at ISR time.
 * (DEPRECATED. Use tfNotifyInterfaceIsr() instead.)
 * Notify the user that there have been packets received.
 * Will cause:
 *  tfCheckReceiveInterface to return 0 (polling check method),
 * or
 *  tfWaitReceiveInterface (task pending check method) to return.
 */
TM_PROTO_EXTERN void tfNotifyReceiveInterfaceIsr(
    ttUserInterface interfaceHandle);

/*
 * Check to see if data is waiting to be received
 * Returns 0              if there is data ready to be received
 *         TM_EWOULDBLOCK if there is no data waiting to be received.
 */
TM_PROTO_EXTERN int tfCheckReceiveInterface(ttUserInterface interfaceHandle);

/*
 * Wait for incoming data, if there is none, then wait
 * VERY IMPORTANT:
 * Can only be used when there is a separate receive task (TM_TASK_RECV
 * defined in trsystem.h)
 */
TM_PROTO_EXTERN int tfWaitReceiveInterface(ttUserInterface interfaceHandle);

/*
 * When user has been notified that an incoming packet has arrived (via
 * either tfCheckReceiveInterface(), or tfWaitReceiveInterface()), it
 * then calls tfRecvInterface() so that the stack can call the
 * driver receive function, and retrieve the data
 */
TM_PROTO_EXTERN int tfRecvInterface(ttUserInterface interfaceHandle);

/*
 * This routine is used to call any other driver functions
 * (i.e. refresh receive pools, empty the device driver transmit queue)
 * Empty the device driver transmit queue:
 * If the user uses the interface transmit queue, he should call
 * tfIoctlInterface(interfaceHandle, TM_DEV_IOCTL_EMPTY_XMIT_FLAG, 0, 0)
 * periodically to try and empty the device transmit queue (which
 * contains all the buffers that the device driver could not send right away).
 * Refill the Treck recv pool:
 * If the user uses the Treck pre-allocated recv pool of buffers, he
 * should call:
 * tfIoctlInterface(interfaceHandle, TM_DEV_IOCTL_REFILL_POOL_FLAG, 0, 0)
 * periodically to try and refill the Treck recv pool of pre-allocated
 * recv buffers.
 * To query the device driver for offload capabilities, tfIoctlInterface
 * will be called as follows by the stack after the device driver has
 * been opened:
 * tfIoctlInterface(interfaceHandle, TM_DEV_IOCTL_OFFLOAD_GET,
 *                  &devOffloadStruct, sizeof(ttDevIoctlOffloadStruct));
 * To let the device driver know which offload capabilities the stack
 * will support, the stack will call tfIoctlInterface as follows:
 * tfIoctlInterface(interfaceHandle, TM_DEV_IOCTL_OFFLOAD_SET,
 *                  &devOffloadStruct, sizeof(ttDevIoctlOffloadStruct));
 * Returns
 * TM_ENOERROR    (xmit)   All buffers have been transmitted.
 *                (refill) All buffers are allocated
 * TM_EINPROGRESS (xmit)   Some buffers (but not all) have been transmitted
 *                (refill) Some buffers (but not all) have been allocated
 * TM_EPERM       (xmit)   if transmit queue not enabled (i.e user did not
 *                         call tfUseInterfaceXmitQueue() successfully.
 *                (refill) if the user did not create a recv pool
 * TM_ENOBUFS     (refill) No buffer could be allocated.
 * Other          As returned from the device driver send routine.
 *                Buffers are still in the interface transmit queue.
 *                None was sent.
 */
#define TM_DEV_IOCTL_EMPTY_XMIT_FLAG  0x0400
#define TM_DEV_IOCTL_REFILL_POOL_FLAG 0x0800

#if defined(TM_DEV_SEND_OFFLOAD) || defined(TM_DEV_RECV_OFFLOAD)
#define TM_DEV_IOCTL_OFFLOAD_GET      0x1000
#define TM_DEV_IOCTL_OFFLOAD_SET      0x2000

/* Device Offload Structure */
typedef struct tsDevIoctlOffloadStruct
{
    ttUser16Bit offloadFlags;
    ttUser16Bit offloadMaxSize;
} ttDevIoctlOffloadStruct;

typedef ttDevIoctlOffloadStruct TM_FAR * ttDevIoctlOffloadPtr;

#endif /* TM_DEV_SEND_OFFLOAD || TM_DEV_RECV_OFFLOAD */

TM_PROTO_EXTERN int tfIoctlInterface(
    ttUserInterface interfaceHandle,
    int             flag,
    void   TM_FAR * optionPtr,
    int             optionLen );

#if defined(TM_DEV_SEND_OFFLOAD) || defined(TM_DEV_RECV_OFFLOAD)
/*
 * Flags for devOffloadFlags field
 */
/*
 * . devIoctl() with TM_DEV_IOCTL_OFFLOAD_GET
 * Upon return the device driver will set one or more of the following flags
 * in the offloadFlags field of the ttDevIoctlOffloadStruct structure to
 * indicate what the device supports.
 * . devIoctl() with TM_DEV_IOCTL_OFFLOAD_SET
 * The stack will let the driver know which of the following flags
 * returned by TM_DEV_IOCTL_OFFLOAD_GET, it will support,
 * in the offloadFlags field of the ttDevIoctlOffloadStruct structure to
 * indicate what the device should support.
 */
/*
 * Device calculates recv checksum on the entier packet (legacy)
 * (device will append the recv checksum on the entire packet
 * (minus the MAC layer) at the end of the data in network byte order.
 * (Note that some drivers appends the checksum in little endian oder, in
 * this case the recv checksum should be flipped)
 */
#define TM_DEVO_PACKET_RECV_CHKSUM_COMPUTE     0x01
/*
 * Device validates a combination of IP/TCP/UDP/ICMPv6 recv checksum.
 */
#define TM_DEVO_IP_RECV_CHKSUM_VALIDATE        0x02
#define TM_DEVO_TCP_RECV_CHKSUM_VALIDATE       0x04
#define TM_DEVO_UDP_RECV_CHKSUM_VALIDATE       0x08
#define TM_DEVO_ICMP6_RECV_CHKSUM_VALIDATE     0x10
/*
 * The device driver will not pass up the stack packets that have
 * bad checksums.
 */
#define TM_DEVO_DITCH_BAD_CHECKSUM_PACKETS     0x20
/* Device supports TCP checksum offload only (no segmenation offload) */
#define TM_DEVO_TCP_CHKSUM_OFFLOAD             0x40
/* Device supports UDP checksum offload */
#define TM_DEVO_UDP_CHKSUM_OFFLOAD             0x80
/* Device supports ICMPV6 checksum offload */
#define TM_DEVO_ICMP6_CHKSUM_OFFLOAD           0x100
/* Device supports IPv4 checksum offload */
#define TM_DEVO_IP_CHKSUM_OFFLOAD              0x200
/* Device supports TCP Segmentation offload for IPv4 packets */
#define TM_DEVO_TCP_SEGMENT_OFFLOAD_V4         0x400
/* Device supports TCP Segmentation offload for IPv6 packets */
#define TM_DEVO_TCP_SEGMENT_OFFLOAD_V6         0x800
#endif /* TM_DEV_SEND_OFFLOAD || TM_DEV_RECV_OFFLOAD */

#ifdef TM_DEV_RECV_OFFLOAD
/*
 * flags set in incoming path when checksum has been computed/validated/or
 * has failed
 */
#define TM_DEVOR_PACKET_RECV_CHKSUM_COMPUTED     0x01
/*
 * Device validates a combination of IP/TCP/UDP/ICMPv6 recv checksum.
 */
#define TM_DEVOR_IP_RECV_CHKSUM_VALIDATED        0x02
#define TM_DEVOR_TCP_RECV_CHKSUM_VALIDATED       0x04
#define TM_DEVOR_UDP_RECV_CHKSUM_VALIDATED       0x08
#define TM_DEVOR_ICMP6_RECV_CHKSUM_VALIDATED     0x10
#define TM_DEVOR_IP_RECV_CHKSUM_FAILED           0x20
#define TM_DEVOR_TCP_RECV_CHKSUM_FAILED          0x40
#define TM_DEVOR_UDP_RECV_CHKSUM_FAILED          0x100
#define TM_DEVOR_ICMP6_RECV_CHKSUM_FAILED        0x200
#endif /* TM_DEV_RECV_OFFLOAD */

/*
 * Interface options flags.
 */
/* Options not allowed on a Pt2Pt connection */
#define TM_DEV_OFLAG_NOT_PT2PT  0x0100
/* Option length is a short */
#define TM_DEV_OFLAG_SHORT      0x0200
/* Option value should be >= 0 */
#define TM_DEV_OFLAG_ABSOLUTE   0x0400
/* Option only allowed when device is not opened (or configured) */
#define TM_DEV_OFLAG_NOT_OPEN   0x0800
/* Option length is an unsigned character */
#define TM_DEV_OFLAG_UCHAR      0x1000

/*
 * Interface options macros
 */
/*
 * Copy received driver buffers whose length is smaller than a given
 * user threshold. This option does not need to be used on a point to point
 * device, because buffer is copied anyways in the link layer recv function.
 */
#define TM_DEV_OPTIONS_RECV_COPY  (   0x01                   \
                                    | TM_DEV_OFLAG_SHORT     \
                                    | TM_DEV_OFLAG_ABSOLUTE  \
                                    | TM_DEV_OFLAG_NOT_PT2PT )
/*
 * Turn on/off a transmit task for flow control between the TCP/IP stack and
 * a device driver. Can only be changed when device is not opened
 * (i.e not configured). Cannot be set if the transmit buffer queue is used.
 */
#define TM_DEV_OPTIONS_XMIT_TASK  (   0x02                   \
                                    | TM_DEV_OFLAG_SHORT     \
                                    | TM_DEV_OFLAG_ABSOLUTE  \
                                    | TM_DEV_OFLAG_NOT_OPEN )

/*
 * Set the hardware transmit buffer alignement on that interface.
 * Default is TM_ETHER_HW_TX_ALIGN (2 bytes).
 */
#define TM_DEV_OPTIONS_TX_ALIGN   (   0x03                   \
                                    | TM_DEV_OFLAG_SHORT      \
                                    | TM_DEV_OFLAG_ABSOLUTE   \
                                    | TM_DEV_OFLAG_NOT_OPEN )


/* BOOTP/DHCP option: Number of retries */
#define TM_DEV_OPTIONS_BOOT_RETRIES (   0x04                   \
                                      | TM_DEV_OFLAG_UCHAR     \
                                      | TM_DEV_OFLAG_ABSOLUTE )

/* BOOTP/DHCP option: Initial timeout value */
#define TM_DEV_OPTIONS_BOOT_TIMEOUT (   0x05                   \
                                      | TM_DEV_OFLAG_UCHAR     \
                                      | TM_DEV_OFLAG_ABSOLUTE )

#ifdef TM_USE_DRV_SCAT_RECV
/*
 * For driver scattered recv only. Set the minimum number of
 * bytes at the head of a packet that have to be contiguous.
 * If a scattered recv buffer is received with a first link length below that
 * minimum, that minimum number of bytes is copied into a new buffer.
 * If not set, default value is TM_DEV_DEF_RECV_CONT_HDR_LENGTH .
 */
#define TM_DEV_OPTIONS_SCAT_RECV_LENGTH                        \
                                    (   0x06                   \
                                      | TM_DEV_OFLAG_SHORT     \
                                      | TM_DEV_OFLAG_ABSOLUTE )
#endif /* TM_USE_DRV_SCAT_RECV */



/* Allows user to set above options on the interface */
TM_PROTO_EXTERN int tfInterfaceSetOptions(
    ttUserInterface interfaceHandle,
    int             optionName,
    void   TM_FAR * optionValuePtr,
    int             optionLength );

/*
 * Enable/Disable the use of an interface transmit queue:
 * Device driver wants to use a device transmit queue of specified depth,
 * to queue packets that it cannot transmit right away in the driver send
 * routine. If specified depth is zero, this option is turned off.
 * Can only be called when the device is not configured.
 * Cannot be called for a point to point interface.
 * Cannot be called if a transmit task is used.
 * Parameters:
 * interfaceHandle        Interface handle as returned by tfAddInterface()
 * numberXmitDescriptors  Length of the transmit queue. If positive, the
 *                        interface transmit queue is enabled. If 0, it
 *                        is disabled.
 * Returns:
 * TM_ENOERROR         no error
 * TM_ENOBUFS          Not enough memory to allocate the empty transmit ring
 * TM_EPERM            Device is configured, or transmit queue is used,
 *                     or point to point link layer is used.
 * TM_EINVAL           Invalid interface handle, or numberXmitDescriptors < 0
 */
TM_PROTO_EXTERN int tfUseInterfaceXmitQueue(
    ttUserInterface interfaceHandle,
    short           transmitQueueDepth );

/* Check whether the user has turned on the interface XMIT queue */
TM_PROTO_EXTERN int tfInterfaceXmitQueueOn(ttUserInterface interfaceHandle);

/* Query for the user's scatter send buffer count */
TM_PROTO_EXTERN int tfInterfaceScatterCount(ttUserInterface interfaceHandle);

#ifdef TM_USE_VCHAN
/*
 * Set the virutal channel on an interface for a given multihome.
 */
TM_PROTO_EXTERN int tfInterfaceSetVirtualChannel(
    ttUserInterface interfaceHandle,
    ttUser32Bit     virtualChannel,
    int             mhomeIndex );
TM_PROTO_EXTERN int tfNgInterfaceSetVirtualChannel(
    ttUserInterface interfaceHandle,
    ttUser32Bit     virtualChannel,
    int             addrFamily,
    int             mhomeIndex );

/*
 * Get the virutal channel associated with the packet currently being sent
 * Function should only be called from the device driver send function.
 */
TM_PROTO_EXTERN int tfInterfaceGetVirtualChannel(
    ttUserInterface      interfaceHandle,
    ttUser32Bit TM_FAR * virtualChannelPtr );
#endif /* TM_USE_VCHAN */

/*
 * Get a pre-allocated buffer from the created interface recv pool.
 * Called from an ISR
 */
TM_PROTO_EXTERN char TM_FAR * tfPoolIsrGetBuffer(
    ttUserInterface interfaceHandle,
    int             size);

/*
 * Free the buffer returned by tfPoolIsrGetBuffer() from within the same
 * Recv ISR.
 */
TM_PROTO_EXTERN void tfPoolIsrFreeBuffer(
    ttUserInterface interfaceHandle,
    int             size );

/* Flag for last parameter of tfPoolCreate */

#define TM_POOL_REFILL_IN_LINE 1
/*
 * Create a pool of receive buffers on the given interface
 */
TM_PROTO_EXTERN int tfPoolCreate(
     ttUserInterface interfaceHandle,
     int             numberFullSizeBuffers,
     int             numberSmallSizebuffers,
     int             fullBufferSize,
     int             smallBufferSize,
     int             alignment,
/* 0, or TM_POOL_REFILL_IN_LINE. Indicates whether to re-allocate in line */
     int             flag );
/*
 * Free all allocated receive buffers in the interface pool, and delete
 * pool from the interface.
 */
TM_PROTO_EXTERN int tfPoolDelete(ttUserInterface interfaceHandle);

/*
 * Receive next buffer ready to be received by the stack.
 * Can be used as the driver receive function, when driver uses the Treck
 * recv pool.
 * Fill in parameters.
 * TM_DEV_OKAY if there exists a recv buffer, otherwise TM_DEV_ERROR
 */
TM_PROTO_EXTERN int tfPoolReceive(
    ttUserInterface        interfaceHandle,
    char TM_FAR * TM_FAR * dataPtrPtr,
    int  TM_FAR *          dataLengthPtr,
    ttUserBufferPtr        bufHandlePtr );

/* User uses GHS INDRT debugger on this interface. */
TM_PROTO_EXTERN int tfUseIndrt(ttUserInterface interfaceHandle);
/*
 * Transmit done notification to GHS INDRT.
 * Called from transmit done ISR either directly from the driver ISR, if
 * tfNotifyInterfaceIsr() is not called for a transmit done interrupt,
 * or from tfNotifyInterfaceIsr() otherwise.
 * Returns 1, if one of the packet notified is an INDRT packet.
 * Returns 0 otherwise.
 */
TM_PROTO_EXTERN int tfIndrtSentIsr(
    ttUserInterface interfaceHandle,
    int             sentPacketCount );
/*
 * Called from device driver recv ISR, so that GHS INDRT packet
 * can be processed during ISR.
 * Returns 1, if the packet received is a GHS INDRT packet.
 * Returns 0, otherwise.
 */
TM_PROTO_EXTERN int tfIndrtRecvIsr(
    ttUserInterface interfaceHandle,
    char  TM_FAR *  bufDataPtr,
    int             stackBuffer );

/* Add and Delete System Default Gateway */
TM_PROTO_EXTERN int tfAddDefaultGateway(
    ttUserInterface interfaceHandle,
    ttUserIpAddress gatewayIpAddress );

TM_PROTO_EXTERN int tfDelDefaultGateway(
    ttUserIpAddress gatewayIpAddress);

/*
 * tfAddInterfaceMhomeAddress():
 * Add a multihome IP address to the interface without adding it to
 * the routing table
 * Parameters:
 * interfaceHandle  User device interface handle
 * ipAddress        User IP address
 * mhomeIndex       Mhome index of the configuration on the interface
 * Return value:
 * TM_ENOERROR      No error
 * TM_EADDRNOTAVAIL Invalid IP address
 * TM_EINVAL        Invalid interface
 * TM_EALREADY      Interface already configured for that mhome address
 */
TM_PROTO_EXTERN int tfAddInterfaceMhomeAddress(
    ttUserInterface interfaceHandle,
    ttUserIpAddress ipAddress,
    unsigned char   mHomeIndex );

/*
 * Start BOOTP relay agent. BOOTP/DHCP server can be reached via
 * interfaceHandle, multihomeindex. Server Ip address is: ipAddress.
 */
TM_PROTO_EXTERN int tfStartBootRelayAgent(
    ttUserIpAddress ipAddress,
    ttUserInterface interfaceHandle,
    unsigned char   multiHomeIndex );

/* STOP BOOTP relay agent */
TM_PROTO_EXTERN int tfStopBootRelayAgent(void);

/* Use Ethernet Link layer */
TM_PROTO_EXTERN ttUserLinkLayer tfUseEthernet(void);

/* Use Tagged Ethernet Link Layer */
TM_PROTO_EXTERN ttUserLinkLayer tfUseEthernetTagged(void);

/* Use Null Link Layer */
TM_PROTO_EXTERN ttUserLinkLayer tfUseNullLinkLayer(void);

/* Use Transparent Link layer */
TM_PROTO_EXTERN ttUserLinkLayer tfUseTransp(void);

typedef void (TM_CODE_FAR * ttUserLnkNotifyFuncPtr)(
    ttUserInterface interfaceHandle,
    int             flags );

#define TM_LNK_NOTIFY_FUNC_NULL_PTR  (ttUserLnkNotifyFuncPtr)0

#ifdef TM_PPP_LQM
/* function prototype for user-defined handler for PPP LCP Echo-Reply
   message */
typedef int (TM_CODE_FAR * ttEchoReplyFuncPtr)(
    ttUserInterface     interfaceHandle,
    ttUser8Bit          echoRequestId,
    const char TM_FAR * dataPtr,
    int                 dataLen);

#define TM_PPP_ECHO_REPLY_FUNC_NULL_PTR (ttEchoReplyFuncPtr)0

typedef struct tsLqrCounts
{
    ttUser32Bit lastOutLqrs;
    ttUser32Bit lastOutPackets;
    ttUser32Bit lastOutOctets;
    ttUser32Bit peerInLqrs;
    ttUser32Bit peerInPackets;
    ttUser32Bit peerInDiscards;
    ttUser32Bit peerInErrors;
    ttUser32Bit peerInOctets;
    ttUser32Bit peerOutLqrs;
    ttUser32Bit peerOutPackets;
    ttUser32Bit peerOutOctets;
    ttUser32Bit saveInLqrs;
    ttUser32Bit saveInPackets;
    ttUser32Bit saveInDiscards;
    ttUser32Bit saveInErrors;
    ttUser32Bit saveInOctets;
} ttLqrCounts;
typedef ttLqrCounts TM_FAR * ttLqrCountsPtr;
typedef const ttLqrCounts TM_FAR * ttConstLqrCountsPtr;

typedef struct  tsLqrCountDeltas
{
    ttUser32Bit deltaLastOutLqrs;
    ttUser32Bit deltaLastOutPackets;
    ttUser32Bit deltaLastOutOctets;
    ttUser32Bit deltaPeerInLqrs;
    ttUser32Bit deltaPeerInPackets;
    ttUser32Bit deltaPeerInDiscards;
    ttUser32Bit deltaPeerInErrors;
    ttUser32Bit deltaPeerInOctets;
    ttUser32Bit deltaPeerOutLqrs;
    ttUser32Bit deltaPeerOutPackets;
    ttUser32Bit deltaPeerOutOctets;
    ttUser32Bit deltaSaveInLqrs;
    ttUser32Bit deltaSaveInPackets;
    ttUser32Bit deltaSaveInDiscards;
    ttUser32Bit deltaSaveInErrors;
    ttUser32Bit deltaSaveInOctets;
} ttLqrCountDeltas;
typedef ttLqrCountDeltas TM_FAR * ttLqrCountDeltasPtr;

/* function prototype for user-defined link quality monitoring function */
typedef ttUser8Bit (TM_CODE_FAR * ttLqmMonitorFuncPtr)(
    ttUserInterface         interfaceHandle,
    int                     reasonCode,
    ttUser32Bit             timeElapsedMsec,
    ttLqrCountDeltasPtr     countDeltasPtr,
    ttConstLqrCountsPtr     countsPtr,
    ttUser32Bit             outLqrs,
    ttUser32Bit             outPackets,
    ttUser32Bit             outOctets);
#endif /* TM_PPP_LQM */

/* Use SLIP Link Layer */
TM_PROTO_EXTERN ttUserLinkLayer tfUseSlip(void);

/*
 * SLIP options
 */
/* Set SLIP send buffer size (to send to driver) */
#define TM_SLIP_OPT_SEND_BUF_SIZE      0x01

/*
 * Configure SLIP options. Only option supported is
 * TM_SLIP_OPT_SEND_BUF_SIZE
 */
int tfSlipSetOptions(
    ttUserInterface interfaceHandle,
    int             optionName,
    void   TM_FAR * optionValuePtr,
    int             optionLength );

/* Use PPP Async Link Layer */
TM_PROTO_EXTERN ttUserLinkLayer tfUseAsyncPpp(
    ttUserLnkNotifyFuncPtr linkNotifyFuncPtr);

TM_PROTO_EXTERN ttUserLinkLayer tfUseAsyncServerPpp(
    ttUserLnkNotifyFuncPtr linkNotifyFuncPtr);

#ifdef TM_PPP_LQM
/* Use PPP Link Quality Monitoring */
TM_PROTO_EXTERN int tfUsePppLqm(
                        ttUserInterface interfaceHandle,
                        ttUser32Bit     lqrReTxPeriodMsec);

/* Deallocate/free PPP Link Quality Monitoring */
TM_PROTO_EXTERN int tfFreePppLqm(
    ttUserInterface interfaceHandle);

/* Register a user-defined link quality monitoring function */
TM_PROTO_EXTERN int tfLqmRegisterMonitor(
    ttUserInterface         interfaceHandle,
    ttLqmMonitorFuncPtr     monitorFuncPtr,
    ttUser16Bit             hysteresisMaxFailures,
    ttUser16Bit             hysteresisSamples);

TM_PROTO_EXTERN int tfLqmSendLinkQualityReport(
    ttUserInterface interfaceHandle);

TM_PROTO_EXTERN int tfPppSendEchoRequest(
    ttUserInterface         interfaceHandle,
    ttUser8Bit              echoRequestId,
    const char TM_FAR *     dataPtr,
    int                     dataLen,
    ttEchoReplyFuncPtr      echoReplyFuncPtr);

TM_PROTO_EXTERN int tfLqmSetLqrTimerPeriod(
    ttUserInterface interfaceHandle,
    ttUser32Bit     lqrTimerPeriodMsec);

TM_PROTO_EXTERN ttUser32Bit tfLqmGetLocalLqrTimerPeriod(
    ttUserInterface interfaceHandle);

TM_PROTO_EXTERN ttUser32Bit tfLqmGetPeerLqrTimerPeriod(
    ttUserInterface interfaceHandle);
#endif /* TM_PPP_LQM */


/* Configure PPP Options */
TM_PROTO_EXTERN int tfPppSetOption(
    ttUserInterface     interfaceHandle,
    int                 protocolLevel,
    int                 remoteLocalFlag,
    int                 optionName,
    const char TM_FAR * optionValuePtr,
    int                 optionLength );

/* when mschap is used, we have mschap1, mschap2, chap, and pap,
 * we need to assign priority to each of them.
 */
TM_PROTO_EXTERN
int tfPppSetAuthPriority(ttUser8Bit     authMethod,
                         ttUser8Bit     priorityValue);
/*
 * Macros used for return values from the PPP CHAP user authentication
 * function.  One is an actual null pointer (indicating failure) and the
 * second is a empty string, corresponding to an actual empty password.
 */
extern const char    TM_FAR * tvChapEmptyString;

#define TM_CHAP_INVALID_USER  (char TM_FAR *)0
#define TM_CHAP_NULL_PASSWORD tvChapEmptyString


/*
 * Dialer API
 */

/* Add a dialer send->expect pair */
int tfDialerAddSendExpect(
    ttUserInterface interfaceHandle,
    char TM_FAR *   sendString,
    char TM_FAR *   expectString,
    char TM_FAR *   errorString,
    int             numRetries,
    int             timeout,
    unsigned char   flags);

/* Add a dialer expect->send pair */
int tfDialerAddExpectSend(
    ttUserInterface interfaceHandle,
    char TM_FAR *   sendString,
    char TM_FAR *   expectString,
    char TM_FAR *   errorString,
    int             numRetries,
    int             timeout,
    unsigned char   flags);

/* Initialize the dialer (does NOT start dialing) */
int tfUseDialer(
    ttUserInterface        interfaceHandle,
    ttUserLnkNotifyFuncPtr notifyFuncPtr);


#define TM_DIALER_FAIL_ON_ERROR 0x01
#define TM_DIALER_COMPLETE      0x02
#define TM_DIALER_EXPECT_FIRST  0x04

/* Dialer notification flags */
#define TM_DIALER_OPEN_COMPLETE 1
#define TM_DIALER_OPEN_FAILED   2


/*
 * Get a buffer of specified length from the system for the User to use
 * for a device driver. Return value is a pointer to the allocated buffer.
 * uBufferPtr    Pointer to the user buffer handle to be filled in by
 *               this routine.
 * length        Length of the data that the user need to copy
 * alignment     Buffer pointer will be aligned on a multiple of this value
 * Note: It is not required for a user to use our buffer pool because
 * some devices may not support it.
 */
char TM_FAR * tfGetDriverBuffer (
    ttUserBufferPtr uBufferPtr,
    int             length,
    int             alignment );

/*
 * Get a buffer of maximum ethernet size from the system for the User to
 * use for an ethernet device driver.
 * The data pointer is offset by two bytes to allow the data portion
 * to be long word aligned
 * Note: It is not required for a user to use our buffer pool because
 * some devices may not support it.
 */
TM_PROTO_EXTERN char TM_FAR *tfGetEthernetBuffer(
    ttUserBufferPtr userBufferPtr);

/*
 * User free a buffer that he allocated with either tfGetEthernetBuffer(),
 * or tfGetDriverBuffer(), and did not hand to the stack.
 */
TM_PROTO_EXTERN int tfFreeDriverBuffer (ttUserBuffer userBuffer);

/* User RIP protocol */
TM_PROTO_EXTERN int             tfUseRip(void);

/*
 * Routine to call when any socket call fails (TM_SOCKET_ERROR), to get
 * the error value back.
 */
TM_PROTO_EXTERN int tfGetSocketError (int socketDescriptor);

/*
 * Set blocking or nonblocking on a socket.
 * (Can Also be done with tfIoctl)
 */
TM_PROTO_EXTERN int tfBlockingState(int socketDescriptor, int blockingState);

/*
 * Get the number of bytes waiting to be read on a socket.
 * (Can Also be done with tfIoctl)
 */
TM_PROTO_EXTERN int tfGetWaitingBytes(int socketDescriptor);

/*
 * Get the number of bytes sent and acked on a socket.
 */
TM_PROTO_EXTERN int tfGetSendCompltBytes(int socketDescriptor);

/*
 * Get the number of bytes left to be written on a socket.
 */
TM_PROTO_EXTERN int tfGetSendBytesLeft(int socketDescriptor);


/*
 * Get the offset for out of band (OOB) data in the Receive Queue
 *  (Can Also be done with tfIoctl, but with tfIoctl we only can query
 *   whether we are at the OOB mark)
 */
TM_PROTO_EXTERN int tfGetOobDataOffset(int socketDescriptor);

/*
 * BSD ioctl. Valid 'request' values are:
 *             FIONBIO (can also be done with tfBlockingState()),
 *             FIONREAD (can also be done with tfGetWaitingBytes()), and
 *             SIOCATMARK (can also be done with tfGetOobDataOffset()).
 * tfIoctl will return an error for any other request.
 */
TM_PROTO_EXTERN int tfIoctl(
    int             socketDescriptor,
    ttUser32Bit     request,
    int TM_FAR *    dataArg );

/* Same as sendto + specify interface handle for data output */
TM_PROTO_EXTERN int tfSendToInterface(
    int                             socketDescriptor,
    char TM_FAR *                   bufferPtr,
    int                             bufferLength,
    int                             flags,
    const struct sockaddr TM_FAR *  toAddressPtr,
    int                             toAddressLength,
    ttUserInterface                 interfaceHandle,
    unsigned char                   mhomeIndex);

/* Get the Physical Address for an Interface */
TM_PROTO_EXTERN int tfGetIfPhysAddr(
    ttUserInterface interfaceHandle,
    char TM_FAR *   physAddrPtr,
    int             physAddrLength );

/* Set the MTU for an interface */
TM_PROTO_EXTERN int tfSetIfMtu(
    ttUserInterface interfaceHandle,
    int             ifMtu);

/* Get the MTU for an interface */
TM_PROTO_EXTERN int tfGetIfMtu(
    ttUserInterface interfaceHandle,
    int TM_FAR *    ifMtuPtr);

/* Get the IP address of an interface */
TM_PROTO_EXTERN int tfGetIpAddress(
    ttUserInterface          interfaceHandle,
    ttUserIpAddress TM_FAR * ifIpAddressPtr,
    unsigned char            multiHomeIndex );

/* This function is a replacement for tfGetIpAddress to support IPv6. */
TM_PROTO_EXTERN int tfNgGetIpAddress(
    ttUserInterface                     interfaceHandle,
    struct sockaddr_storage TM_FAR *    ifIpAddrPtr,
    int                                 addrFamily,
    unsigned int                        multiHomeIndex );

/* Get the Broadcast address of an interface */
TM_PROTO_EXTERN int tfGetBroadcastAddress(
    ttUserInterface          interfaceHandle,
    ttUserIpAddress TM_FAR * ifBroadcastAddressPtr,
    unsigned char            mHomeIndex );

/* Convert an IP address to a dotted string */
TM_PROTO_EXTERN void tfInetToAscii(
    ttUser32Bit   ipAddress,
    char TM_FAR * outputIpAddressString );

/* Get the Net Mask of an interface */
TM_PROTO_EXTERN int tfGetNetMask(
    ttUserInterface          interfaceHandle,
    ttUserIpAddress TM_FAR * netMaskPtr,
    unsigned char            multiHomeIndex );

/* This function is a replacement for tfGetNetMask to support IPv6. */
TM_PROTO_EXTERN int tfNgGetPrefixLen(
    ttUserInterface     interfaceHandle,
    ttUser8Bit TM_FAR * prefixLenPtr,
    int                 addrFamily,
    unsigned int        multiHomeIndex );

/*
 * Get the IP address of any configured interface
 */
TM_PROTO_EXTERN ttUserIpAddress tfGetRouterId(void);


/* Add an ARP Entry to ARP table */
TM_PROTO_EXTERN int tfAddArpEntry(
    ttUserIpAddress     arpIpAddress,
    const char TM_FAR * physAddrPtr,
    int                 physAddrLength );

/* This function is a replacement for tfAddArpEntry to support IPv6. */
TM_PROTO_EXTERN int tfNgAddArpEntry(
    const struct sockaddr_storage TM_FAR *  arpIpAddrPtr,
    const ttUser8Bit TM_FAR *               physAddrPtr,
    int                                     physAddrLen );

/* Delete an ARP Entry from ARP table using IP address to locate entry*/
TM_PROTO_EXTERN int tfDelArpEntryByIpAddr(ttUserIpAddress arpIpAddress);

/* This function is a replacement for tfDelArpEntryByIpAddr to support IPv6. */
TM_PROTO_EXTERN int tfNgDelArpEntryByIpAddr(
    const struct sockaddr_storage TM_FAR * arpIpAddrPtr );

/* Delete an ARP Entry from ARP table using physical address to locate entry*/
TM_PROTO_EXTERN int tfDelArpEntryByPhysAddr(
    const char TM_FAR * physAddrPtr,
    int                 physAddrLen );

/* This function is a replacement for tfDelArpEntryByPhysAddr to support
   IPv6. */
TM_PROTO_EXTERN int tfNgDelArpEntryByPhysAddr (
    int                         addrFamily,
    const ttUser8Bit TM_FAR *   physAddrPtr,
    int                         physAddrLen );

/* Get an ARP Entry from ARP table using physical address to locate entry*/
TM_PROTO_EXTERN int tfGetArpEntryByPhysAddr(
    const char      TM_FAR * physAddrPtr,
    int                      physAddrLen,
    ttUserIpAddress TM_FAR * arpIpAddressPtr );

/* This function is replacement for tfGetArpEntryByPhysAddr to support IPv6. */
TM_PROTO_EXTERN int tfNgGetArpEntryByPhysAddr (
    int                                 addrFamily,
    const ttUser8Bit TM_FAR *           physAddrPtr,
    int                                 physAddrLen,
    struct sockaddr_storage TM_FAR *    arpIpAddrPtr );

/* Get an ARP Entry from ARP table using IP Address to locate entry*/
TM_PROTO_EXTERN int tfGetArpEntryByIpAddr(
    ttUserIpAddress  arpIpAddress,
    char  TM_FAR *   physAddrPtr,
    int              physAddrLen );

/* This function is a replacement for tfGetArpEntryByIpAddr to support IPv6. */
TM_PROTO_EXTERN int tfNgGetArpEntryByIpAddr (
    const struct sockaddr_storage TM_FAR *  arpIpAddrPtr,
    ttUser8Bit TM_FAR *                     physAddrPtr,
    int                                     physAddrBufLen,
    int TM_FAR *                            physAddrLenPtr );

/* Add a Proxy Arp entry */
TM_PROTO_EXTERN int tfAddProxyArpEntry(ttUserIpAddress arpIpAddress);

/* Delete a Proxy Arp entry */
TM_PROTO_EXTERN int tfDelProxyArpEntry(ttUserIpAddress arpIpAddress);


/* Set the IP address for a PPP peer */
TM_PROTO_EXTERN int tfSetPppPeerIpAddress(
    ttUserInterface interfaceHandle,
    ttUserIpAddress ifIpAddress );

/* This function is a replacement for tfSetPppPeerIpAddress to support IPv6. */
TM_PROTO_EXTERN int tfNgSetPt2PtPeerIpAddress(
    ttUserInterface                         interfaceHandle,
    const struct sockaddr_storage TM_FAR *  ifIpAddrPtr );

/* Get the IP address of a PPP peer */
TM_PROTO_EXTERN int tfGetPppPeerIpAddress(
    ttUserInterface          interfaceHandle,
    ttUserIpAddress TM_FAR * ifIpAddressPtr );

/* This function is a replacement for tfGetPppPeerIpAddress to support IPv6. */
TM_PROTO_EXTERN int tfNgGetPt2PtPeerIpAddress(
    ttUserInterface                     interfaceHandle,
    struct sockaddr_storage TM_FAR *    ifIpAddrPtr );

/* Get the IP address of a PPP DNS Server */
TM_PROTO_EXTERN int tfGetPppDnsIpAddress(
    ttUserInterface          interfaceHandle,
    ttUserIpAddress TM_FAR * dnsIpAddressPtr,
    int                      flag );

/* Add and Delete Local Route. Users use tfConfigInterface() and
 * tfUnconfigInterface()
 */

/* Add and Delete Destination Route */
TM_PROTO_EXTERN int tfAddStaticRoute(
    ttUserInterface interfaceHandle,
    ttUserIpAddress destIpAddress,
    ttUserIpAddress destNetMask,
    ttUserIpAddress gateway,
    int             hops );

/* This function is a replacement for tfAddStaticRoute to support IPv6. */
TM_PROTO_EXTERN int tfNgAddStaticRoute(
    ttUserInterface                         interfaceHandle,
    const struct sockaddr_storage TM_FAR *  destIpAddrPtr,
    int                                     prefixLen,
    const struct sockaddr_storage TM_FAR *  gatewayPtr,
    int                                     hops );

TM_PROTO_EXTERN int tfDelStaticRoute( ttUserIpAddress DestIpAddress,
                                      ttUserIpAddress DestNetMask );

/* This function is a replacement for tfDelStaticRoute to support IPv6. */
TM_PROTO_EXTERN int tfNgDelStaticRoute(
    const struct sockaddr_storage TM_FAR *  destIpAddrPtr,
    int                                     prefixLen );

TM_PROTO_EXTERN int tfAddMcastRoute(
    ttUserInterface interfaceId,
    ttUserIpAddress mcastAddress,
    ttUserIpAddress netmask,
    unsigned char   mhomeIndex );

/*
 * Find out if a route to a destination IP address/destination netmask
 * exists
 */
TM_PROTO_EXTERN int tfRtDestExists(
    ttUserIpAddress ipAddress,
    ttUserIpAddress netMask );

/* This function is a replacement for tfRtDestExists to support IPv6. */
TM_PROTO_EXTERN int tfNgRtDestExists(
    const struct sockaddr_storage TM_FAR *  ipAddrPtr,
    int                                     prefixLen );

#ifdef TM_RT_OUTPUT
/* Print a snaphot of the routing tree */
TM_PROTO_EXTERN void tfRtTreePrint(void);
#endif /* TM_RT_OUTPUT */

#if (!defined(TM_DISABLE_PMTU_DISC) && defined(TM_USE_IPV4))
/*
 * Disable Path MTU discovery, by adding a static indirect route with a
 * path MTU discovery disabled flag. If pathMtu is zero, then the device
 * MTU is used as path MTU, otherwise the pathMTU parameter is used instead.
 */
TM_PROTO_EXTERN int tfDisablePathMtuDisc(
    ttUserIpAddress     destIpAddress,
    unsigned short      pathMtu );
#endif /* !TM_DISABLE_PMTU_DISC and TM_USE_IPV4*/

#if (!defined(TM_DISABLE_PMTU_DISC) || !defined(TM_6_DISABLE_PMTU_DISC))
TM_PROTO_EXTERN int tfNgDisablePathMtuDisc(
    struct sockaddr_storage TM_FAR * destIpAddrPtr,
    int                              pathMtu);
#endif /* !TM_DISABLE_PMTU_DISC || !TM_6_DISABLE_PMTU_DISC*/

/* Used to Get a Message Buffer for Zero Copy */
TM_PROTO_EXTERN ttUserMessage tfGetZeroCopyBuffer(
    int                    size,
    char TM_FAR * TM_FAR * dataPtrPtr );

/* Get the default gateway */
TM_PROTO_EXTERN int tfGetDefaultGateway(
    ttUserIpAddress TM_FAR * gatewayIpAddressPtr);

/*
 * User specified default interface to be used to send multicast
 * destination IP packets.
 */
int tfSetMcastInterface(
    ttUserInterface interfaceHandle,
    unsigned char   mhomeIndex );

/* Free a zero copy message buffer */
TM_PROTO_EXTERN int tfFreeZeroCopyBuffer(ttUserMessage bufferHandle);

/* Used to send a zero copy message */
TM_PROTO_EXTERN int tfZeroCopySendTo(
    int                             socketDescriptor,
    ttUserMessage                   bufferHandle,
    int                             bufferLength,
    int                             flags,
    const struct sockaddr TM_FAR *  toAddressPtr,
    int                             toAddressLength );

/*
 * Data type for user free function pointer passed to
 * tfZeroCopyUserBufferSendTo(), tfZeroCopyUserBufferSend(),
 * tfIpScatteredSend(), and tfSocketScatteredSendTo API's.
 */
typedef int (TM_CODE_FAR *ttUserFreeFuncPtr)(char TM_FAR * bufferPtr);

/* Used to send a zero copy user buffer */
TM_PROTO_EXTERN int tfZeroCopyUserBufferSendTo (
                            int                            socketDescriptor,
                            char TM_FAR *                  userBufferPtr,
                            char TM_FAR *                  userDataPtr,
                            int                            userDataLength,
                            ttUserFreeFuncPtr              userFreeFunction,
                            int                            flags,
                            const struct sockaddr TM_FAR * toAddressPtr,
                            int                            toAddressLength );

/* Used to send a zero copy message */
TM_PROTO_EXTERN int tfZeroCopySend(
    int           socketDescriptor,
    ttUserMessage bufferHandle,
    int           bufferLength,
    int           flags );

/* Used to send a zero copy user buffer */
TM_PROTO_EXTERN int tfZeroCopyUserBufferSend (
                                   int                  socketDescriptor,
                                   char TM_FAR *        userBufferPtr,
                                   char TM_FAR *        userDataPtr,
                                   int                  userDataLength,
                                   ttUserFreeFuncPtr    userFreeFunction,
                                   int                  flags);

/* Used to receive a network message without a copy */
TM_PROTO_EXTERN int tfZeroCopyRecv(
    int                    socketDescriptor,
    ttUserMessage TM_FAR * bufferHandlePtr,
    char TM_FAR * TM_FAR * dataPtrPtr,
    int                    maxDataLength,
    int                    flags );

/* Used to receive a network message without a copy */
TM_PROTO_EXTERN int tfZeroCopyRecvFrom(
    int                     socketDescriptor,
    ttUserMessage   TM_FAR * bufferHandlePtr,
    char   TM_FAR * TM_FAR * dataPtrPtr,
    int                      maxDataLength,
    int                      flags,
    struct sockaddr TM_FAR * fromAddressPtr,
    int             TM_FAR * addressLengthPtr );

/* Same as sento, but also specifies the source address */
TM_PROTO_EXTERN int tfSendToFrom(
    int                             socketDescriptor,
    const char      TM_FAR *        bufferPtr,
    int                             bufferLength,
    int                             flags,
    const struct sockaddr TM_FAR *  toAddressPtr,
    int                             addressLength,
    const struct sockaddr TM_FAR *  fromAddressPtr );

/* Same as recvfrom, but also get the destination address */
TM_PROTO_EXTERN int tfRecvFromTo(
    int                      socketDescriptor,
    char            TM_FAR * bufferPtr,
    int                      bufferLength,
    int                      flags,
    struct sockaddr TM_FAR * fromAddressPtr,
    int             TM_FAR * addressLengthPtr,
    struct sockaddr TM_FAR * toAddressPtr );

/* Flush a socket receive queue */
TM_PROTO_EXTERN int tfFlushRecvQ(int socketDescriptor);

/*
 * Data type to convey information about scattered send user buffers,
 * in the tfSocketScatteredSendTo() API, and tfIpScatteredSend() API.
 */
typedef struct tsUserBlock
{
/* Pointer to buffer (passed to the free routine) */
    char TM_FAR * userBufferPtr;
/* Pointer to beginning of data */
    char TM_FAR * userDataPtr;
/* Data length */
    int           userDataLength;
} ttUserBlock;
typedef ttUserBlock TM_FAR * ttUserBlockPtr;

/*
 * User send non TCP scattered data on a socket, using its own buffers.
 */
TM_PROTO_EXTERN int tfSocketScatteredSendTo(
    int                             socketDescriptor,
    ttUserBlockPtr                  userBlockPtr,
    int                             userBlockCount,
    ttUserFreeFuncPtr               userFreeFunction,
    int                             flags,
    const struct sockaddr TM_FAR *  toAddressPtr,
    int                             toAddressLength );

/*
 * User send scattered data, using its own buffers, on a given interface,
 * Data already contains the IP header.
 */
TM_PROTO_EXTERN int tfIpScatteredSend(
    ttUserInterface     interfaceHandle,
    ttUserBlockPtr      userBlockPtr,
    int                 userBlockCount,
    ttUserFreeFuncPtr   userFreeFunction );

/*
 * socket call back function type
 */
/*
 * Flags
 * TM_CB_CONNECT_COMPLT
 * TM_CB_ACCEPT
 * TM_CB_RECV
 * TM_CB_RECV_OOB
 * TM_CB_SEND_COMPLT
 * TM_CB_REMOTE_CLOSE
 * TM_CB_SOCKET_ERROR
 * TM_CB_RESET
 * TM_CB_CLOSE_COMPLT
 * TM_CB_WRITE_READY
 */

typedef void (TM_CODE_FAR * ttUserSocketCBFuncPtr)(
    int socketDescriptor,
    int flags );

typedef void (TM_CODE_FAR * ttUserSocketCBParamFuncPtr)(
    int             socketDescriptor,
    void TM_FAR *   userParamPtr,
    int             flags );

/*
 * Used to register a function to call upon completion of any socket
 * event set in the flags parameters.
 */
TM_PROTO_EXTERN int tfRegisterSocketCB(
    int                      socketDescriptor,
    ttUserSocketCBFuncPtr    socketCBFuncPtr,
    int                      flags );

/*
 * Used to register a function to call upon completion of any socket
 * event set in the flags parameters. User pointer 'socketUserPtr', is
 * being passed as an extra parameter by the call back function.
 */
TM_PROTO_EXTERN int tfRegisterSocketCBParam(
    int                         socketDescriptor,
    ttUserSocketCBParamFuncPtr  socketCBParamFuncPtr,
    void               TM_FAR * socketUserPtr,
    int                         flags );

/*
 * User opens a raw socket, specifying the IP address to bind to,
 * and specifying the transport layer protocol.
 */
TM_PROTO_EXTERN int tfRawSocket(ttUserIpAddress ipAddress, int protocol);

/* To Avoid Close conflicts with File System Close */
TM_PROTO_EXTERN int tfClose(int socketDescriptor);

/*
 * User Call Back Function type for tfSocketArrayWalk.
 */
typedef int (TM_CODE_FAR * ttWalkCBFuncPtr)(
    int           socketDesc,
    ttUserVoidPtr argPtr );
/*
 * Walk the list of sockets, calling the user supplied call back function,
 * passing the socket index, and user argPtr, until we reach the end
 * of the list, or the user call back function returns an error whichever
 * comes first. Return error value from the call back function to the user.
 */
TM_PROTO_EXTERN int tfSocketArrayWalk(
    ttWalkCBFuncPtr callBackFuncPtr,
    ttUserVoidPtr   argPtr );


/* To Avoid IOCTL conflicts with Operating System */
/*Redundantly declared, previouly declared at line 2056
 *TM_PROTO_EXTERN int tfIoctl( int              socketDescriptor,
                             ttUser32Bit      request,
                             int     TM_FAR * argumentPtr );
 */

/* To Abort the TCP Connection */
TM_PROTO_EXTERN int tfResetConnection(int socketDescriptor);

/* IP forward call back function type. See below */
typedef int (TM_CODE_FAR * ttUserIpForwCBFuncPtr)(
    ttUserIpAddress srcIpAddress,
    ttUserIpAddress destIpAddress );

/*
 * Used to register a function for the Treck stack to call when a packet
 * cannot be forwarded. The function's parameters will indicate the source
 * IP address, and destination IP address of the packet in network byte order.
 * If the call back function returns an error code, then the stack will
 * send a host unreachable ICMP error message as before.
 * If the call back function returns TM_ENOERROR, then the stack
 * will silently drop the packet.
 */
TM_PROTO_EXTERN int tfRegisterIpForwCB(ttUserIpForwCBFuncPtr ipForwCBFuncPtr);

/* To Copy Data */
TM_PROTO_EXTERN void tfBcopy(
    const void TM_FAR * source,
    void TM_FAR *       destination,
    unsigned int        length);

/* To Zero a Data Area */
TM_PROTO_EXTERN void tfBzero(
    void TM_FAR * bufferPtr,
    unsigned int  length );

/* To copy a string */
TM_PROTO_EXTERN void tfStrCpy(
    char TM_FAR *       destinationPtr,
    const char TM_FAR * sourcePtr );

/* To compute a string length */
TM_PROTO_EXTERN unsigned int tfStrLen(const char TM_FAR  *strPtr);

/*
 * Compare two strings to see if they are equal
 */
TM_PROTO_EXTERN int tfStrCmp(
    const char TM_FAR * stringPtr1,
    const char TM_FAR * stringPtr2 );

/*
 * Compare two strings to see if they are equal up to nBytes.
 */
TM_PROTO_EXTERN int tfStrNCmp(
    const char TM_FAR * stringPtr1,
    const char TM_FAR * stringPtr2,
    unsigned int        nBytes );

/*
 * Return the length of the initial segment of bufferString that consists
 * entirely of characters not included in string.
 */
TM_PROTO_EXTERN unsigned int tfStrCSpn(
    const char TM_FAR * bufferStringPtr,
    const char TM_FAR * stringPtr );

/*
 * Check whether stringBuffer contains string. If so return pointer to
 * copy of string within stringBuffer, otherwise return null pointer.
 */
TM_PROTO_EXTERN char TM_FAR * tfStrStr(
    const char TM_FAR * stringBufferPtr,
    const char TM_FAR * stringPtr );

/* Concatenate (append) stringSrc to stringDest */
TM_PROTO_EXTERN char TM_FAR * tfStrCat(
    char TM_FAR *       stringDestPtr,
    const char TM_FAR * stringSrcPtr );

/*
 * Return pointer to first instance of character inside string. If no match
 * return NULL.
 */
TM_PROTO_EXTERN char TM_FAR * tfStrChr(
    const char TM_FAR * stringPtr,
    int                 character);

/*
 * Return pointer to last instance of character in string. Return null if
 * no match is found.
 */
TM_PROTO_EXTERN char TM_FAR * tfStrRChr(
    const char TM_FAR * stringPtr,
    int                 character );

/*
 * Copy stringSrc to stringDest up to nBytes. If nBytes bigger than
 * string length of stringSrc, append 0 up to nbytes.
 * Return pointer to Destination string.
 */
TM_PROTO_EXTERN char TM_FAR * tfStrNCpy (
    char TM_FAR *       stringDestPtr,
    const char TM_FAR * stringSrcPtr,
    unsigned int        nBytes );

/*
 * Map error code to an error string.
 */
TM_PROTO_EXTERN char TM_FAR * tfStrError( int errorCode );

/*
 * Convert strings to a long-integer value.
 */
TM_PROTO_EXTERN ttUserS32Bit tfStrToL(
    const char TM_FAR *     nptr,
    char TM_FAR * TM_FAR *  endptr,
    int                     base );

/*
 * Convert strings to a unsigned long-integer value.
 */
TM_PROTO_EXTERN ttUser32Bit tfStrToUl(
    const char TM_FAR *     nptr,
    char TM_FAR * TM_FAR *  endptr,
    int                     base );

/*
 * Access variable-argument lists.
 */

#include <stdarg.h>

/*
 * Write formatted output using a pointer to a list of arguments.
 */
TM_PROTO_EXTERN int tfVSPrintF(
    char TM_FAR *       buf0,
    const char TM_FAR * fmt0,
    va_list             ap );

/*
 * Write formatted data to a string.
 */
TM_PROTO_EXTERN int tfSPrintF(
    char TM_FAR *       pszBuffer,
    const char TM_FAR * pszFormat, ... );

/*
 * Read formatted input using a pointer to a list of arguments.
 */
TM_PROTO_EXTERN int tfVSScanF(
    const char TM_FAR * pszBuffer,
    const char TM_FAR * pszFormat,
    va_list       ap );

/*
 * Read formatted data from a string
 */
TM_PROTO_EXTERN int tfSScanF(
    const char TM_FAR * pszBuffer,
    const char TM_FAR * pszFormat, ... );

/*
 * Performs a quick sort.
 */
typedef  int (TM_CODE_FAR * ttCmpFuncPtr)(
    const void TM_FAR * parmPtr1,
    const void TM_FAR * parmPtr2 );

TM_PROTO_EXTERN void tfQSort(
    void TM_FAR * a,
    unsigned int  n,
    unsigned int  es,
    ttCmpFuncPtr  cmpFuncPtr );

/*
 * Subtract the (TM_MS_TICK_LENGTH * tlTimerUpdateCount) from countdown for
 * each timer in the active list, execute timers that have countdown <= 0
 * and Add initial time back to countdown then it is AutoCycle
 * otherwise move it to the recycle queue
 */
TM_PROTO_EXTERN void tfTimerExecute(void);

/*
 * Identical routines.
 * Just increase the tlTimerUpdateCount.
 */
TM_PROTO_EXTERN void tfTimerUpdate(void);
TM_PROTO_EXTERN void tfTimerUpdateIsr(void);

/*
 * Kernel Entry Points
 */

/*
 * Initialize the Kernel Data Structures
 */
TM_PROTO_EXTERN void  tfKernelInitialize(void);

/*
 * Begin a Critical Section
 */
TM_PROTO_EXTERN void tfKernelSetCritical(void);

/*
 * End a Critical Section
 */
TM_PROTO_EXTERN void tfKernelReleaseCritical(void);

/*
 * Create a counting semaphore
 */
TM_PROTO_EXTERN int tfKernelCreateCountSem(
    ttUserGenericUnionPtr countingSemaphore );

/*
 * Wait on a counting semaphore
 */
TM_PROTO_EXTERN int tfKernelPendCountSem(
    ttUserGenericUnionPtr countingSemaphore );

/*
 * Wait on a counting semaphore up to a timeout value (in ticks)
 */
TM_PROTO_EXTERN int tfKernelPendCountSemTimeout(
    ttUserGenericUnionPtr countingSemaphore, ttUser32Bit tickTimeout );

/*
 * Signal Processes waiting on a counting semaphore
 */
TM_PROTO_EXTERN int tfKernelPostCountSem(
    ttUserGenericUnionPtr countingSemaphore );

/*
 * Signal Processes waiting on a counting semaphore
 */
TM_PROTO_EXTERN int tfKernelPostCountSem(
    ttUserGenericUnionPtr countingSemaphore );

/*
 * Remove a counting semaphore
 */
TM_PROTO_EXTERN int   tfKernelDeleteCountSem(
    ttUserGenericUnionPtr countingSemaphore );

/*
 * Dynamically allocate the Treck simple heap. Only called when
 * TM_DYNAMIC_CREATE_SHEAP has been #define'd. The size of the simple heap
 * allocated must be sheapSize, with the start of the simple
 * heap (i.e. the pointer returned) being aligned on a 32-bit boundary.
 */
TM_PROTO_EXTERN ttUser32Bit TM_FAR * tfKernelSheapCreate(
    ttUser32Bit TM_FAR * sheapSize);

/*
 * Allocate a block of memory
 */
TM_PROTO_EXTERN void TM_FAR * tfKernelMalloc(unsigned size);

/*
 * Free a block of memory
 */
TM_PROTO_EXTERN void tfKernelFree(void TM_FAR *memoryBlock);

/*
 * Report an error and stop the system (DEBUG ONLY)
 */
TM_PROTO_EXTERN void tfKernelError(
    const char TM_FAR * functionName,
    const char TM_FAR * errorMessage);

/*
 * Current task yields the CPU.
 */
TM_PROTO_EXTERN void tfKernelTaskYield(void);

/*
 * Treck Stack calls this fucntion to ask for the system time. It is
 * used in the appliction protocols such as the web server and SMTP client.
 * User could get the time from the system clock, from network,
 * or from application user interface.
 *
 * Parameters:
 *  secondsPtr:
 *      Pointer to store number of seconds elapsed since midnight (00:00:00),
 *      January 1, 1970, Coordinated Universal Time (UTC).
 *      UTC is the international time standard. It is the current term for
 *      what was commonly referred to as Greenwich Meridian Time (GMT).
 *      This matches the return value of the ANSI C library
 *      function time_t time(time_t *)
 *  daysPtr:
 *      The maximum number of seconds a 32 bit unsigned integer can hold is
 *      2**32 = 4G. this is approximately 126 year.
 *      In case this value ever wrap arounds, we need another space to hold the
 *      a bigger units of time. This is the purpose of daysPtr.
 *  NOTE: The total laps of time is the days stored in daysPtr plus the seconds
 *        stored in the secondsPtr
 * Returns:
 *  TM_KERN_OKAY:   Sucessful
 *  TM_KERN_ERROR:  Failed to obtain the system time.
 *
 */
TM_PROTO_EXTERN int tfKernelGetSystemTime(
    ttUser32Bit * daysPtr,
    ttUser32Bit * secondsPtr);

#ifdef TM_MULTIPLE_CONTEXT
#if defined(TM_KERNEL_WIN32_X86) || defined(TM_KERNEL_LINUX_APP)
/*
 * For each thread store and retrieve it's context pointer
 */
TM_PROTO_EXTERN void tfKernelSetContext(ttUserContext context);
TM_PROTO_EXTERN ttUserContext tfKernelGetContext(void);
#endif /* defined(TM_KERNEL_WIN32_X86) || defined(TM_KERNEL_LINUX_APP) */
#endif /* TM_MULTIPLE_CONTEXT */

/*
 *  Re-allocate the previously allocated block
 *  in userPtr, making the new block size bytes long.
 */
TM_PROTO_EXTERN void TM_FAR * tfRealloc(
    void TM_FAR * ptr,
    unsigned int  size );

#define tm_realloc(ptr, size) tfRealloc(ptr, size)

/* Allocate SIZE bytes of memory.  */
TM_PROTO_EXTERN void TM_FAR *             tfMalloc(unsigned int size);
/* Free a block allocated by `tfMalloc', `tfRealloc'.  */
TM_PROTO_EXTERN void                      tfFree(void TM_FAR * ptr);
#define tm_malloc(size)                   tfMalloc(size)
#define tm_free                           tfFree
#define tm_malloc_init()
/*
 * Call tfKernelMalloc/tfSheapMalloc,
 * and tfFreeDynamicMemory() + tfKernelMalloc/tfSheapMalloc
 * if first one fails.
 */
TM_PROTO_EXTERN ttUserVoidPtr tfBufferDoubleMalloc(unsigned size);


TM_PROTO_EXTERN void tfMemCpy(
    void TM_FAR *       destination,
    const void TM_FAR * source,
    unsigned            length );

TM_PROTO_EXTERN void tfMemSet(
    void TM_FAR * buffer,
    int           fillCharacter,
    unsigned      length );

/*
 * Finds characters in a buffer.
 * If nByte isn't found then NULL is returned.
 */

TM_PROTO_EXTERN void TM_FAR * tfMemChr(
    const void TM_FAR * pBuf,
    int           nByte,
    unsigned int  uCount );

/*
 * Compare buffer1 to buffer2 up to length bytes. Return TM_STRCMP_EQUAL
 * if they match, TM_STRCMP_LESS if string1 is less than string2,
 * TM_STRCMP_MORE otherwise.
 */
TM_PROTO_EXTERN int tfMemCmp(
    const void TM_FAR * bufPtr1,
    const void TM_FAR * bufPtr2,
    unsigned int  length );

/*
 * Log a network error.
 */
TM_PROTO_EXTERN void  tfKernelWarning(
    const char TM_FAR * functionName,
    const char TM_FAR * errorMessage );

/*
 * Return the Kernel Task Id
 */
TM_PROTO_EXTERN ttUser32Bit tfKernelTaskId(void);

/*
 * Create an event structure for pend/post from an ISR
 */
TM_PROTO_EXTERN void tfKernelCreateEvent(ttUserGenericUnionPtr eventPtr);

/*
 * Wait on an Event from task.
 */
TM_PROTO_EXTERN void tfKernelPendEvent(ttUserGenericUnionPtr eventPtr);

/*
 * Wait on an Event from task up to a timeout value (in ticks).
 */
TM_PROTO_EXTERN int tfKernelPendEventTimeout( ttUserGenericUnionPtr eventPtr,
                                              ttUser32Bit tickTimeout );

/*
 * Resume task that is waiting on this event
 * CALLED FROM AN ISR
 */
TM_PROTO_EXTERN void tfKernelIsrPostEvent(ttUserGenericUnionPtr eventPtr);

/*
 * Resume task that is waiting on this event.
 * CALLED FROM A TASK.
 */
TM_PROTO_EXTERN void tfKernelTaskPostEvent(ttUserGenericUnionPtr eventPtr);


/*
 * Install an ISR handler (for Device Drivers)
 */
TM_PROTO_EXTERN void tfKernelInstallIsrHandler(
    ttUserIsrHandlerPtr funcPtr,
    ttUser32Bit         offSet );

/*
 * Simple heap malloc.
 * This call used by the TRECK stack only when
 *    1. TM_USE_SHEAP, and TM_SHEAP_SIZE are defined in trsystem.h,
 * or
 *    2. when it is called from tfKernelMalloc() and TM_SHEAP_SIZE is defined
 *       in trsystem.h.
 * SHOULD NEVER be called directly by a user application.
 */
TM_PROTO_EXTERN void TM_FAR *tfSheapMalloc(unsigned size);

/*
 * Simple heap free
 * This call used by the TRECK stack only when
 *    1. TM_USE_SHEAP, and TM_SHEAP_SIZE are defined in trsystem.h,
 * or
 *    2. when it is called from tfKernelFree() and TM_SHEAP_SIZE is defined
 *       in trsystem.h.
 * SHOULD NEVER be called directly by a user application.
 */
TM_PROTO_EXTERN void tfSheapFree(void TM_FAR * memoryBlockPtr);

/* Used to register a function to validate a PAP request */
TM_PROTO_EXTERN int tfPapRegisterAuthenticate(
    ttUserInterface           interfaceHandle,
    ttPapAuthenticateFunctPtr functPtr );

/* Used to register a function to validate a CHAP request */
TM_PROTO_EXTERN int tfChapRegisterAuthenticate(
    ttUserInterface            interfaceHandle,
    ttChapAuthenticateFunctPtr functPtr );

#ifdef TM_USE_DHCP_FQDN
#define TM_FQDN_MAX_LEN 252
#endif /* TM_USE_DHCP_FQDN */

/*
 * DHCP User interface
 */

/*
 * Boot entry type. To store information collected from the BOOPT/DHCP
 * server.
 */
typedef struct tsUserBtEntry
{
/* 2nd phase BOOT Server (TFTP server) */
    ttUserIpAddress                   btuBootSIpAddress;
/* Domain name server (for DNS) */
    ttUserIpAddress                   btuDns1ServerIpAddress;
/* Second domain name server (for DNS) */
    ttUserIpAddress                   btuDns2ServerIpAddress;
/* Our (leased) IP address */
    ttUserIpAddress                   btuYiaddr;
/* Our subnet mask */
    ttUserIpAddress                   btuNetMask;
/* Default router */
    ttUserIpAddress                   btuDefRouter;
/* Default router after lease renewal (if changed) */
    ttUserIpAddress                   btuLeaseDefRouter;
/* DHCP selected server IP address */
    ttUserIpAddress                   btuDhcpServerId;
/* Time stamp of DHCP request (Internal) */
    ttUser32Bit                       btuDhcpRequestTimeStamp;
/* DHCP Address lease time (internal) */
    ttUser32Bit                       btuDhcpLeaseTime;
/* DHCP T1 time (internal) */
    ttUser32Bit                       btuDhcpT1;
/* DHCP T2 time (internal) */
    ttUser32Bit                       btuDhcpT2;
/* Boot file size (in 512-bytes blocks) (for TFTP download) */
    unsigned short                    btuBootFileSize;
/* seconds since we started sending a BOOTP request/DHCP discover*/
    unsigned short                    btuSeconds;
#ifndef TM_DHCP_NBNS_NUM_SERVER
#define TM_DHCP_NBNS_NUM_SERVER       2
#endif /* TM_DHCP_NBNS_NUM_SERVER */
    ttUserIpAddress                   btuNetBiosNameServers[
                                          TM_DHCP_NBNS_NUM_SERVER];
    unsigned char                     btuNetBiosNumNameServers;
    unsigned char                     btuDhcpClientIdLength;
    unsigned char                     btuDhcpHostNameLength;
#ifndef TM_DHCP_CLIENTID_SIZE
#define TM_DHCP_CLIENTID_SIZE 16
#endif /* TM_DHCP_CLIENTID_SIZE */
    unsigned char                     btuDhcpClientIDPtr[
                                        ((TM_DHCP_CLIENTID_SIZE + 3) /4) * 4];
#ifndef TM_DHCP_HOSTNAME_SIZE
#define TM_DHCP_HOSTNAME_SIZE 16
#endif /* TM_DHCP_HOSTNAME_SIZE */
    unsigned char                     btuDhcpHostNamePtr[
                                        ((TM_DHCP_HOSTNAME_SIZE + 3) /4) * 4];
/* BOOTP or DHCP server host name array size */
#define TM_BOOTSNAME_SIZE             64
/* Domain name (64 bytes) (for DNS) */
    unsigned char                      btuDomainName[TM_BOOTSNAME_SIZE];
/* TFTP server name (64 bytes) (for TFTP download) */
    unsigned char                      btuBootSname[TM_BOOTSNAME_SIZE];
/* BOOTP or DHCP boot file name array size */
#define TM_BOOTFILENAME_SIZE          128
/* Boot file name (128 bytes) (for TFTP download) */
    unsigned char                      btuBootFileName[TM_BOOTFILENAME_SIZE];
#ifdef TM_USE_DHCP_FQDN
    unsigned char                      btuServerFqdn[TM_FQDN_MAX_LEN+1];
    int                                btuServerFqdnLen;
    int                                btuFqdnStatus;
    unsigned char                      btuFqdnRCode1;
    unsigned char                      btuFqdnRCode2;
    unsigned char                      fqdnPad[2];
#endif /* TM_USE_DHCP_FQDN */
} ttUserBtEntry;

typedef ttUserBtEntry TM_FAR * ttUserBtEntryPtr;

/*
 * Notify function, called when a DHCP IP address has been obtained
 * (errorCode == TM_ENOERROR), or when an error occur either while
 * trying to obtain it (TM_ETIMEDOUT), or because the DHCP server
 * has cancelled our lease.
 */
typedef void (TM_CODE_FAR * ttUserDhcpNotifyFuncPtr) (
    ttUserInterface interfaceHandle,
    int             errorCode,
    int             userIndex );

#define TM_DHCP_NOTIFY_FUNC_NULL_PTR  (ttUserDhcpNotifyFuncPtr)0

/*
 * Flags bit used in the flags field of tfDhcpUserSet(), or tfDhcpConfSet()
 */
#define TM_DHCPF_INIT_REBOOT          0x01 /* Start in INIT-REBOOT state */
#define TM_DHCPF_REQUESTED_IP_ADDRESS 0x02 /* Use requested IP address field */
#define TM_DHCPF_SUPPRESS_CLIENT_ID   0x04 /* Suppress Client ID option */
#define TM_DHCPF_FQDN_ENABLE          0x08 /* DHCP FQDN is enabled */
#define TM_DHCPF_FQDN_PARTIAL         0x10 /* FQDN domain name is partial */
/* FQDN domain name should be sent in ASCII */
#define TM_DHCPF_FQDN_ASCII           0x20
#define TM_DHCPF_HOST_NAME            0x40 /* Host name option */


/* Status bits */
#define TM_FQDNF_PARTIAL         0x10 /* Indicates the FQDN is partial */
#define TM_FQDNF_MALFORMED       0x20 /* Invalid FQDN format           */
#define TM_FQDNF_NOT_SUPPORTED   0x40 /* server doesn't support FQDN   */
#define TM_FQDNF_NAMEREPLY       0x80 /* server reply contained FQDN   */

#define TM_DHCPF_FLAGS (   TM_DHCPF_INIT_REBOOT                         \
                         | TM_DHCPF_REQUESTED_IP_ADDRESS                \
                         | TM_DHCPF_SUPPRESS_CLIENT_ID                  \
                         | TM_DHCPF_FQDN_ENABLE                         \
                         | TM_DHCPF_FQDN_PARTIAL                        \
                         | TM_DHCPF_FQDN_ASCII                          \
                         | TM_DHCPF_HOST_NAME )

#ifdef TM_USE_DHCP_FQDN
/*
 * Set the DHCP global FQDN domain name.
 * This name is the default FQDN domain name. It will be sent in the
 * DHCP FQDN option unless the user provided a domain name for a specific
 * interface/multi home index or user index using tfDhcpUSerSet or
 * tfDhcpConfSet.
 */
TM_PROTO_EXTERN int tfUserSetFqdn(const char TM_FAR *, int, int);

/*
 * This function converts a host name from text to binary format.
 */
TM_PROTO_EXTERN int tfHostnameTextToBinary (
                                 unsigned char TM_FAR * hostNameBinPtr,
                                 int TM_FAR           * binLengthPtr,
                                 char TM_FAR          * hostNameTxtPtr,
                                 int                    txtLength,
                                 int                    flags);
/*
 * This function converts a hostname from binary to text format
 */
TM_PROTO_EXTERN int tfHostnameBinaryToText (
                                 char TM_FAR          * hostNameTxtPtr,
                                 int TM_FAR           * txtLengthPtr,
                                 unsigned char TM_FAR * hostNameBinPtr,
                                 int                    binLength,
                                 int TM_FAR           * flags);
#endif /* TM_USE_DHCP_FQDN */
/*
 * Set the DHCP initial state (TM_DHCP_INIT, or TM_DHCP_INITREBOOT) prior
 * to calling tfDhcpUserStart(). This call is optional.
 * Use this only:
 *  1. if you want to specify your own Client ID, or
 *  2. if you want to start in INIT-REBOOT state, or
 *  3. if you want to specify a requested IP address in INIT sate, or
 *  4. if you want to prevent the stack from using a CLIENT ID option.
 *
 * 1. If the TM_DHCPF_INITREBOOT bit is set in the flags field, the stack
 * will set DHCP in the INIT-REBOOT state. In that case, specify
 * the IP address, and Client ID option that was used in the previous lease.
 * If no Client ID option was used in the previously obtained lease, set
 * the Client ID option pointer to NULL, and set the
 * TM_DHCPF_SUPPRESS_CLIENT_ID bit in the flags field, to prevent the stack
 * from generating a CLIENT ID option.
 * 2. If the TM_DHCPF_INITREBOOT bit is not set in the flags field, the stack
 * will set DHCP in the INIT state. In that case, optionally specify
 * a requested IP address, and/or the CLIENT ID option, and/or suppress
 * the CLIENT ID option insertion by the stack.
 * If CLIENT ID pointer is null, and the TM_DHCPF_SUPPRESS_CLIENT bit is not
 * set in the flags field, the stack will pick a CLIENT ID for you, and insert
 * it in all messages. Note that the stack will pick a CLIENT ID that will
 * be the same across reboots provided that you use tfDhcpUserSet
 * with the same physical board, and same index.
 *
 * Index has to be between 0 and and tvMaxUserDhcpEntries - 1.
 * (tvMaxUserDhcpEntries can be set to a
 * value different from its default 0 value, with tfInitTreckOptions
 * with option name TM_OPTION_DHCP_MAX_ENTRIES).
 * In INITREBOOT state, make sure that you use the Client ID used in a
 * previous lease (if not suppressed). If you had let the stack pick its own,
 * make sure that you retrieve it in the structure returned by
 * tfDhcpUserGetBootEntry().
 *
 * flags field == TM_DHCPF_HOST_NAME:
 * This function can also be called separately to set/reset the DHCP host name
 * option. In that case the flags field must be set to TM_DHCPF_HOST_NAME. 
 * By default the stack will not send the DHCP host name option. The
 * user can call this function to set a host name with a non null clientIdPtr
 * and non zero clientIdLength parameters. In that case the stack will send
 * the DHCP host name option. (This option can be reset by setting those 2
 * parameters to zero.) 
 */
TM_PROTO_EXTERN int tfDhcpUserSet (
    ttUserInterface interfaceHandle,
    int             index,
    int             flags,
    ttUserIpAddress requestedIpAddress,
    unsigned char   TM_FAR * clientIdPtr,
    int             clientIdLength );

/*
 * Start sending DHCP request. index has to be between 0 and
 * and tvMaxUserDhcpEntries - 1. (tvMaxUserDhcpEntries can be
 * set to a value different from its default 0 value, with tfInitTreckOptions
 * with option name TM_OPTION_DHCP_MAX_ENTRIES).
 */
TM_PROTO_EXTERN int tfDhcpUserStart(
    ttUserInterface         interfaceHandle,
    int                     index,
    ttUserDhcpNotifyFuncPtr dhcpNotifyFunc);

/*
 * Get the user DHCP boot entry where all the information gathered from
 * the DHCP server has been stored. (Result from a tfDhcpUserStart())
 * Returns a pointer only if we got a bound IP addres from the DHCP server.
 */
TM_PROTO_EXTERN ttUserBtEntryPtr tfDhcpUserGetBootEntry(
    ttUserInterface interfaceHandle,
    int             index );

/*
 * Cancel DHCP request and/or DHCP lease.
 */
TM_PROTO_EXTERN int tfDhcpUserRelease(
    ttUserInterface    interfaceHandle,
    int                userIndex );

/*
 * Set the DHCP initial state (TM_DHCP_INIT, or TM_DHCP_INITREBOOT) prior
 * to calling tfOpenInterface/tfConfigInterface()). This call is optional
 * Use this only:
 *  1. if you want to specify your own Client ID, or
 *  2. if you want to start in INIT-REBOOT state, or
 *  3. if you want to specify a requested IP address in INIT sate, or
 *  4. if you want to prevent the stack from using a CLIENT ID option.
 *
 * 1. If the TM_DHCPF_INITREBOOT bit is set in the flags field, the stack
 * will set DHCP in the INIT-REBEBOOT state. In that case, specify
 * the IP address, and Client ID option that was used in the previous lease.
 * If no Client ID option was used in the previously obtained lease, set
 * the Client ID option pointer to NULL, and set the
 * TM_DHCPF_SUPPRESS_CLIENT_ID bit in the flags field, to prevent the stack
 * from generating a CLIENT ID option.
 * 2. If the TM_DHCPF_INITREBOOT bit is not set in the flags field, the stack
 * will set DHCP in the INIT state. In that case, optionally specify
 * a requested IP address, and/or the CLIENT ID option, and/or suppress
 * the CLIENT ID option insertion by the stack.
 * If CLIENT ID pointer is null, and the TM_DHCPF_SUPPRESS_CLIENT bit is not
 * set in the flags field, the stack will pick a CLIENT ID for you, and insert
 * it in all messages. Note that the stack will pick a CLIENT ID that will
 * be the same across reboots provided that you use tfDhcpConfSet()
 * with the same physical board, and same index.
 *
 * In INITREBOOT state, make sure that you use the Client ID used in a
 * previous lease (if not suppressed). If you had let the stack pick its own,
 * make sure that you retrieve it in the structure returned by
 * tfConfGetBootEntry().
 *
 * flags field == TM_DHCPF_HOST_NAME:
 * This function can also be called separately to set/reset the DHCP host name
 * option. In that case the flags field must be set to TM_DHCPF_HOST_NAME. 
 * By default the stack will not send the DHCP host name option. The
 * user can call this function to set a host name with a non null clientIdPtr
 * and non zero clientIdLength parameters. In that case the stack will send
 * the DHCP host name option. (This option can be reset by setting those 2
 * parameters to zero.) 
 */
TM_PROTO_EXTERN int tfDhcpConfSet(
    ttUserInterface interfaceHandle,
    int             mHomeIndex,
    int             flags,
    ttUserIpAddress requestedIpAddress,
    unsigned char   TM_FAR * clientIdPtr,
    int             clientIdLength );

/*
 * Get the DHCP/BOOTP boot entry where all the information gathered from
 * the DHCP/BOOTP server has been stored. (Result from a tfConfigInterface()
 * with either flag TM_DEV_IP_BOOTP, or flag TM_DEV_IP_DHCP)
 * Returns a pointer only if we got a bound IP addres from the DHCP server,
 * or a valid response from the BOOTP server.
 */
TM_PROTO_EXTERN ttUserBtEntryPtr tfConfGetBootEntry(
    ttUserInterface interfaceHandle,
    unsigned char   multiHomeIndex );


/*
 * Ping interface
 */
/*
 * Ping info Structure where statistics on the PING connection are kept.
 */
typedef struct tsPingInfo
{
/* Last round trip time computed */
    ttUser32Bit                  pgiLastRtt;
/* maximum round trip time */
    ttUser32Bit                  pgiMaxRtt;
/* minimum round trip time */
    ttUser32Bit                  pgiMinRtt;
/* average round trip time */
    ttUser32Bit                  pgiAvgRtt;
/* sum of all round trip time received so far (to compute average) */
    ttUser32Bit                  pgiSumRtt;
/* number of ping transmitted packets */
    ttUser32Bit                  pgiTransmitted;
/*
 * number of received echo replies packets (not including duplicated
 * packets)
 */
    ttUser32Bit                  pgiReceived;
/* Number of duplicated ping echo replies packets */
    ttUser32Bit                  pgiDuplicated;
/* Send Error code if any that occured */
    int                          pgiSendErrorCode;
/* Recv CB Error code if any that occured */
    int                          pgiRecvErrorCode;
} ttPingInfo;

/* Pointer to a ping info structure */
typedef ttPingInfo TM_FAR * ttPingInfoPtr;

/* Ping info function call back */
typedef  void  (TM_CODE_FAR * ttPingCBFuncPtr)(int pingSocketDescriptor);

/*
 * PING User API
 */

/*
 * Open a PING ICMP socket, start sending PING requests to remoteHostName,
 * and keep sending PING requests at every pingInterval seconds (which
 * defaults to 1 second if set to zero). (If TM_PING_MILLISECONDS_INTERVAL
 * is defined then pingInterval is in milliseconds (which defaults to 1000
 * milliseconds if set to zero).)
 * Size of the ping request data is pingDataLength (which defaults to 56
 * if set to zero).
 * If pingUserCBFuncPtr is non nil then this function is called upon
 * receiving a PING echo reply.
 * To stop the system from sending PING ICMP echo requests, the user
 * need to call tfPingClose(pingSocketDesc);
 * tfPingOpen returns a valid socket descriptor upon success,
 * TM_SOCKET_ERROR otherwise.
 */
TM_PROTO_EXTERN int tfPingOpenStart (
    char TM_FAR *   remoteHostName,
#ifdef TM_PING_MILLISECONDS_INTERVAL
    ttUser32Bit     pingMilliSecondsInterval,
#else /* !TM_PING_MILLISECONDS_INTERVAL */
    int             pingInterval,
#endif /* !TM_PING_MILLISECONDS_INTERVAL */
    int             pingDataLength,
    ttPingCBFuncPtr pingUserCBFuncPtr );

/*
 * This function is a replacement for tfPingOpenStart to support IPv6.
 * pingMilliSecondsInterval is in milliseconds always, which defaults to
 * 1000 milliseconds if set to zero.
 */
TM_PROTO_EXTERN int tfNgPingOpenStart(
    const struct sockaddr_storage TM_FAR *  remoteAddrPtr,
    ttUser32Bit                             pingMilliSecondsInterval,
    int                                     pingDataLength,
    ttPingCBFuncPtr                         pingUserCBFuncPtr );

/*
 * Get PING statistics in the ttPingInfo structure that pingInfoPtr
 * points to. pingSocketDesc should match the tfPingOpenStart return value.
 * pingInfoPtr should point to a ttPingInfo structure that is
 * allocated by the user.
 */
TM_PROTO_EXTERN int tfPingGetStatistics (
    int           pingSocketDesc,
    ttPingInfoPtr pingInfoPtr );

/*
 * Stop sending PING requests to remoteHostName, and close pingSocketDesc.
 * pingSocketDesc should match the tfPingOpenStart() return value.
 */
TM_PROTO_EXTERN int tfPingClose (int pingSocketDesc);



/*
 * Functions and macros for the FTP and FTPD application program interface
 */

/*
 * File system flags. What the file system allows:
 */
#define TM_FS_CWD_FLAG    0x0001 /* supports change working directory */
#define TM_FS_SMNT_FLAG   0x0002 /* supports structure mount */
#define TM_FS_RETR_FLAG   0x0004 /* supports reading from a file */
#define TM_FS_STOR_FLAG   0x0008 /* supports writing to a file */
#define TM_FS_STORU_FLAG  0x0010 /* supports writing to a file with a
                                  * unique name
                                  */
#define TM_FS_APPEND_FLAG 0x0020 /* supports appending to a file */
#define TM_FS_RENAME_FLAG 0x0040 /* supports renaming of file name */
#define TM_FS_DELETE_FLAG 0x0080 /* supports renaming of file name */
#define TM_FS_RMD_FLAG    0x0100 /* supports removing directory */
#define TM_FS_MKD_FLAG    0x0200 /* supports making directory */
#define TM_FS_PWD_FLAG    0x0400 /* supports printing the working directory */
#define TM_FS_LIST_FLAG   0x0800 /* supports long listing of directory */
#define TM_FS_NLST_FLAG   0x1000 /* supports short listing of directory */
#define TM_FS_CRLF_FLAG   0x2000 /* end of line is <CR>,<LF> */
#define TM_FS_RECORD_FLAG 0x4000 /* Interpret FTP record structure */

/* All of the above flags ORed together */
#define TM_FS_ALLCMND_MASK 0x7FFF

/* Read, write, or append flag. Used when opening a file */
#define TM_FS_READ          0
#define TM_FS_WRITE         1
#define TM_FS_APPEND        2

/*
 * Type Asccii, or type Binary. Used when opening a file, and also when
 * giving an FTP TYPE command
 */
#define TM_TYPE_ASCII       0  /* Ascii (default) */
#define TM_TYPE_BINARY      2  /* Image */

/*
 * Structure stream, or structure record. Used when opening a file, and
 * also when giving an FTP STRU command
 */
#define TM_STRU_STREAM      0  /* File */
#define TM_STRU_RECORD      1  /* Record */
/*
 * Long listing of directory. Used when opening a directory, and also
 * to differentiate between FTP NLST and LIST commands.
 */
#define TM_DIR_SHORT    0
#define TM_DIR_LONG     1

/* Syntax error, command unrecognized */
#define TM_FTP_SYNTAXCMD        8500

/* Syntax error in parameters or arguments */
#define TM_FTP_SYNTAXARG        8501

/* Command not implemented */
#define TM_FTP_NOCMD            8502

/* Bad sequence of commands */
#define TM_FTP_BADCMDSEQ        8503

/* Command not implemented for that parameter */
#define TM_FTP_NOCMDPARAM       8504

/* Restart marker reply */
#define TM_FTP_RESTARTMARK      8110

/* Service ready in nnn minutes */
#define TM_FTP_SERVREADY        8120

/* Service not available, closing TELNET connection */
#define TM_FTP_SERVNAVAIL       8421

/* Data connection already open; transfer starting */
#define TM_FTP_XFERSTART        8125

/* Can't open data connection */
#define TM_FTP_DATAOPEN         8425

/* Connection trouble, closed; transfer aborted */
#define TM_FTP_XFERABOR         8426

/* Not logged in */
#define TM_FTP_NOTLOGIN         8530

/* User name okay, need password */
#define TM_FTP_NEEDPASS         8331

/* Need account for login */
#define TM_FTP_NEEDACCTLOGIN    8332

/* Need account for storing files */
#define TM_FTP_NEEDACCTFILE     8532

/* File status okay; about to open data connection */
#define TM_FTP_FILEOKAY         8150

/* Requested file action pending further information */
#define TM_FTP_FILEPEND         8350

/* Requested file action not taken: file unavailable */
#define TM_FTP_FILENAVAIL       8450

/* Requested action not taken: file unavailable */
#define TM_FTP_NAVAIL           8550

/* Requested action aborted: local error in processing */
#define TM_FTP_LOCALERR         8451

/* Requested action not taken: insufficient storage */
#define TM_FTP_NOSPACE          8452

/* Requested action not taken: exceeded storage */
#define TM_FTP_EXSPACE          8552

/* Requested action not taken: file name not allowed */
#define TM_FTP_FILENAME         8553

/* Start mail input; end with <CR><LF>.<CR><LF> */
#define TM_FTP_STARTMAIL        8354

/* Passvie mode flags, used for tfFtpTurnPasv() */
#define TM_FTP_PASSIVE_MODE_ON      1
#define TM_FTP_PASSIVE_MODE_OFF     0

/*
 * Flags for tfNgFtpdUserStart
 */
/* When using a dual stack, only allow IPv4 connections to the FTP server. */
#define TM_FTPD_IPV4_ONLY TM_UL(0x00000001)

/*
 * FTPD USER API
 */
/* Stop the FTP server */
TM_PROTO_EXTERN int tfFtpdUserStop(void);

/* Execute the FTP server (non blocking mode only) */
TM_PROTO_EXTERN int tfFtpdUserExecute(void);

/* Start the FTP server */
TM_PROTO_EXTERN int tfNgFtpdUserStart (
    int           fileFlags,
    int           maxConnections,
    int           maxBackLog,
    int           idleTimeout,
    int           blockingState,
    ttUser32Bit   flags);

/* Deprecated - use tfNgFtpdUserStart */
TM_PROTO_EXTERN int tfFtpdUserStart(
    int fileSystemFlags,
    int maxConnections,
    int maxBackLog,
    int idleTimeout,
    int blockingState );

/*
 * FTP USER API
 */
typedef void TM_FAR * ttUserFtpHandle;

/* Allocate and initialize a new session */
TM_PROTO_EXTERN ttUserFtpHandle tfFtpNewSession (
    int           fileSystemFlags,
    int           blockingState,
    char TM_FAR * fsUsernamePtr,
    char TM_FAR * fsPasswordPtr);

/* Deallocate a session */
TM_PROTO_EXTERN int tfFtpFreeSession(ttUserFtpHandle ftpSessionPtr);

/*
 * PORT command. Turn on, or off the port usage. Default is on with FTP.
 * If onFlag is 1, turns on the FTP port usage. If onFlag is 0
 * turns it off.
 */
TM_PROTO_EXTERN int tfFtpTurnPort(
    ttUserFtpHandle ftpSessionPtr,
    int             onFlag );

/*
 * PASV command. Turn on, or off the pasv usage. Default is off with FTP.
 * If onFlag is 1, turns on the FTP port usage. If onFlag is 0
 * turns it off.
 */
TM_PROTO_EXTERN int tfFtpTurnPasv(
    ttUserFtpHandle ftpSessionPtr,
    int             onFlag );

/*
 * Close an FTP client socket, without sending a QUIT command. Useful in
 * case the connection failed, and hence the tfFtpQuit() could not be
 * executed, or tfFtpQuit() failed for any other reason.
 */
TM_PROTO_EXTERN int tfFtpClose(ttUserFtpHandle ftpSessionPtr);

/* Connect to an FTP server */
TM_PROTO_EXTERN int tfNgFtpConnect (
    ttUserFtpHandle                  ftpUserHandle,
    struct sockaddr_storage TM_FAR * ipAddrPtr);

/* Deprecated - use tfNgFtpConnect */
TM_PROTO_EXTERN int tfFtpConnect(
    ttUserFtpHandle ftpSessionPtr,
    char TM_FAR   * ipAddressPtr );

/* Login command */
TM_PROTO_EXTERN int tfFtpLogin(
    ttUserFtpHandle ftpsessionPtr,
    char   TM_FAR * userNamePtr,
    char   TM_FAR * passwordNamePtr,
    char   TM_FAR * accountNamePtr );

/* CWD command */
TM_PROTO_EXTERN int tfFtpCwd(
    ttUserFtpHandle ftpSessionPtr,
    char   TM_FAR * pathNamePtr );

/* CDUP command */
TM_PROTO_EXTERN int tfFtpCdup(ttUserFtpHandle ftpSessionPtr);

/* QUIT command */
TM_PROTO_EXTERN int tfFtpQuit(ttUserFtpHandle ftpSessionPtr);

/* SMNT (structure mount) command */
TM_PROTO_EXTERN int tfFtpSmnt(
    ttUserFtpHandle ftpSessionPtr,
    char   TM_FAR * mountPathPtr );

/* REIN (reinitialize) command */
TM_PROTO_EXTERN int tfFtpRein(ttUserFtpHandle ftpSessionPtr);


/* PASV (passive) command */
TM_PROTO_EXTERN int tfFtpPasv(
    ttUserFtpHandle             ftpSessionPtr,
    struct sockaddr_in TM_FAR * portAddrPtr );

/* TYPE command, type one of TM_ASCII, TM_BINARY */
TM_PROTO_EXTERN int tfFtpType(
    ttUserFtpHandle ftpSessionPtr,
    int             type );

/* STRU command, structure one of TM_STRU_STREAM, TM_STRU_RECORD */
TM_PROTO_EXTERN int tfFtpStru(
    ttUserFtpHandle ftpSessionPtr,
    int             structure );

/* RETR command */
TM_PROTO_EXTERN int tfFtpRetr(
    ttUserFtpHandle   ftpUserHandleA,
    ttUserFtpHandle   ftpUserHandleB,
    char TM_FAR *     fromFileNamePtr,
    char TM_FAR *     toFileNamePtr  );

/* STOR command */
TM_PROTO_EXTERN int tfFtpStor(
    ttUserFtpHandle   ftpUserHandleA,
    ttUserFtpHandle   ftpUserHandleB,
    char TM_FAR *     fromFileNamePtr,
    char TM_FAR *     toFileNamePtr  );

/* STOU command */
TM_PROTO_EXTERN int tfFtpStou (
    ttUserFtpHandle  ftpUserHandleA,
    ttUserFtpHandle  ftpUserHandleB,
    char TM_FAR *    fromFileNamePtr,
    char TM_FAR *    toFileNamePtr,
    int              bufferSize );

/* APPE command */
TM_PROTO_EXTERN int tfFtpAppe(
    ttUserFtpHandle   ftpUserHandleA,
    ttUserFtpHandle   ftpUserHandleB,
    char TM_FAR *     fromFileNamePtr,
    char TM_FAR *     toFileNamePtr  );

/* ALLO command */
TM_PROTO_EXTERN int tfFtpAllo(
    ttUserFtpHandle ftpSessionPtr,
    ttUser32Bit     sizeBytes,
    int             recordSizeBytes );

/* RNFR, RNTO command */
TM_PROTO_EXTERN int tfFtpRename(
    ttUserFtpHandle ftpSessionPtr,
    char   TM_FAR * fromNamePtr,
    char   TM_FAR * toNamePtr );

/* ABOR command */
TM_PROTO_EXTERN int tfFtpAbor(ttUserFtpHandle ftpSessionPtr);

/* DELE command */
TM_PROTO_EXTERN int tfFtpDele(
    ttUserFtpHandle ftpSessionPtr,
    char   TM_FAR * pathNamePtr );

/* RMD command */
TM_PROTO_EXTERN int tfFtpRmd(
    ttUserFtpHandle ftpSessionPtr,
    char   TM_FAR * directoryPathNamePtr );

/* MKD command */
TM_PROTO_EXTERN int tfFtpMkd(
    ttUserFtpHandle ftpSessionPtr,
    char   TM_FAR * directoryPathNamePtr,
    char   TM_FAR * bufferPtr,
    int             bufferSize );

/* PWD command */
TM_PROTO_EXTERN int tfFtpPwd(
    ttUserFtpHandle ftpSessionPtr,
    char   TM_FAR * bufferPtr,
    int             bufferSize );

typedef int (TM_CODE_FAR * ttFtpCBFuncPtr)(
    ttUserFtpHandle ftpSessionPtr,
    char   TM_FAR * bufferPtr,
    int             bufferSize );
/*
 * LIST/NLST command. (*ftpDirCBFuncPtr)() called for each line of
 * directory entry
 */
TM_PROTO_EXTERN int tfFtpDirList(
    ttUserFtpHandle     ftpSessionPtr,
    char       TM_FAR * pathNamePtr,
    int                 directoryFlag,
    ttFtpCBFuncPtr      ftpDirCBFuncPtr );

/* SITE command */
TM_PROTO_EXTERN int tfFtpSite(
    ttUserFtpHandle ftpSessionPtr,
    char   TM_FAR * sitePtr );

/* SYST command */
TM_PROTO_EXTERN int tfFtpSyst(
    ttUserFtpHandle ftpSessionPtr,
    char   TM_FAR * bufferPtr,
    int             bufferSize );

/* STAT command */
TM_PROTO_EXTERN int tfFtpStat(
    ttUserFtpHandle ftpSessionPtr,
    char   TM_FAR * fileNamePtr,
    char   TM_FAR * bufferPtr,
    int             bufferSize );

/*
 * HELP command. (*ftpCBFuncPtr)() called for each help line.
 */
TM_PROTO_EXTERN int tfFtpHelp(
    ttUserFtpHandle ftpSessionPtr,
    char   TM_FAR * commandPtr,
    ttFtpCBFuncPtr  ftpCBFuncPtr );

/* MODE command */
TM_PROTO_EXTERN int tfFtpMode (
    ttUserFtpHandle ftpUserHandle,
    int             mode);

/* NOOP command */
TM_PROTO_EXTERN int tfFtpNoop (ttUserFtpHandle ftpUserHandle);

/* Sets the incoming data port for FTP client operations */
TM_PROTO_EXTERN int tfFtpPort (
    ttUserFtpHandle ftpUserHandle,
    ttUserIpPort    ftpPortNo);

/* Returns full reply text for most recent command */
TM_PROTO_EXTERN int tfFtpGetReplyText(
    ttUserFtpHandle ftpUserHandle,
    char TM_FAR *   replyStrPtr,
    int             replyStrLen);

/*
 * Execute and check on the status of the last FTP client command
 * (non blocking mode only)
 */
TM_PROTO_EXTERN int tfFtpUserExecute(ttUserFtpHandle ftpSessionPtr);

/*
 * TFTP Client User API
 */

/* TFTP return codes */
#define TM_TFTP_SUCCESS     (ttUser32Bit)  0
#define TM_TFTP_EXECUT      ((ttUser32Bit) -1)
#define TM_TFTP_TIMEOUT     ((ttUser32Bit) -2)
#define TM_TFTP_EBUF        ((ttUser32Bit) -3)
#define TM_TFTP_EINVAL      ((ttUser32Bit) -4)
#define TM_TFTP_ESOCK       ((ttUser32Bit) -5)
#define TM_TFTP_ERROR       ((ttUser32Bit) -6)
/* error codes/messages returned from the server in the error packet */
#define TM_TFTP_UNDEF       ((ttUser32Bit) -7)  /* Undefined error code     */
#define TM_TFTP_NOTFOUND    ((ttUser32Bit) -8)  /* File not found           */
#define TM_TFTP_ACCESS      ((ttUser32Bit) -9)  /* Access violation         */
#define TM_TFTP_NOSPACE     ((ttUser32Bit) -10) /* Disk full/alloc exceeded */
#define TM_TFTP_BADOP       ((ttUser32Bit) -11) /* Illegal TFTP operation   */
#define TM_TFTP_BADID       ((ttUser32Bit) -12) /* Unknown transfer ID      */
#define TM_TFTP_FILEEXISTS  ((ttUser32Bit) -13) /* File already exists      */
#define TM_TFTP_NOUSER      ((ttUser32Bit) -14) /* No such user             */

/* Initialize the TFTP client */
TM_PROTO_EXTERN void tfTftpInit(void);

/* Retrieve a file from the server */
TM_PROTO_EXTERN ttUser32Bit tfTftpGet(
    char            TM_FAR * filename,
    struct sockaddr TM_FAR * remoteAddr,
    char            TM_FAR * tftpbuf,
    ttUser32Bit              bufsize,
    unsigned short           mode,
    int                      blocking );

/* Transmit a file to the server */
TM_PROTO_EXTERN ttUser32Bit tfTftpPut(
    char            TM_FAR * filename,
    struct sockaddr TM_FAR * remoteAddr,
    char            TM_FAR * tftpbuf,
    ttUser32Bit              bufsize,
    unsigned short           mode,
    int                      blocking );

/* Execute TFTP main loop iteration (non-blocking only) */
TM_PROTO_EXTERN ttUser32Bit tfTftpUserExecute(void);

/* Set timeout value for TFTP client */
TM_PROTO_EXTERN int tfTftpSetTimeout(int timeout, int retry);

/*
 * TFTP Server User API
 */

 /* Initialize the TFTP server */
TM_PROTO_EXTERN void tfTftpdInit(void);

/* Start the TFTP server */
TM_PROTO_EXTERN int tfTftpdUserStart(
    int maxConnections,
    int sendTimeout,
    int timeoutTime,
    int blockingState );

/* Stop the TFTP server */
TM_PROTO_EXTERN int tfTftpdUserStop(void);

/* Execute TFTP server main loop iteration (non-blocking only) */
TM_PROTO_EXTERN int tfTftpdUserExecute(void);

/*
 * File system interface API for FTP and FTPD
 */

#define TM_FS_MATCH_FILE_NAME   1   /* File name                */
#define TM_FS_MATCH_FILE_EXT    2   /* File extension           */
#define TM_FS_MATCH_FILE_PATH   3   /* Full path to a file      */
#define TM_FS_MATCH_DIR_NAME    4   /* Directory name           */
#define TM_FS_MATCH_DIR_PATH    5   /* Full path to a directory */

TM_PROTO_EXTERN int tfFSUserAllowed(const char TM_FAR * userNamePtr);

TM_PROTO_EXTERN void TM_FAR * tfFSUserLogin(
    const char TM_FAR * userNamePtr,
    const char TM_FAR * passwordPtr );

TM_PROTO_EXTERN void tfFSUserLogout(void TM_FAR *  userDataPtr);

TM_PROTO_EXTERN int tfFSSystem(
    char TM_FAR * bufferPtr,
    int bufferSize);

TM_PROTO_EXTERN int tfFSStructureMount(
    void TM_FAR * userDataPtr,
    const char TM_FAR * pathNamePtr );

TM_PROTO_EXTERN int tfFSRenameFile(
    const void TM_FAR * userDataPtr,
    const char TM_FAR * fromPathNamePtr,
    const char TM_FAR * toPathNamePtr );

TM_PROTO_EXTERN int tfFSGetUniqueFileName(
    void TM_FAR * userDataPtr,
    char TM_FAR * bufferPtr,
    int           bufferSize );

TM_PROTO_EXTERN int tfFSDeleteFile(
    const void TM_FAR * userDataPtr,
    const char TM_FAR * pathNamePtr );

TM_PROTO_EXTERN ttUser32Bit tfFSGetFileLen(
    const void TM_FAR * userDataPtr,
    const char TM_FAR * pathNamePtr );

/* returns:
 *  TM_ENOERROR: file exists
 *  TM_ENOENT  : file doesn't exist
 *  TM_EINVAL  : Envalid input parameter(s)
 */
TM_PROTO_EXTERN int tfFSFileExists(
    const void TM_FAR * userDataPtr,
    const char TM_FAR * pathNamePtr );

/*
 * flag      TM_FS_READ, TM_FS_WRITE, TM_FS_APPEND,
 * type      TM_TYPE_ASCII, TM_TYPE_BINARY,
 * structure TM_STRU_STREAM, TM_STRU_RECORD
 */
TM_PROTO_EXTERN void TM_FAR * tfFSOpenFile(
    const void TM_FAR * userDataPtr,
    const char TM_FAR * pathNamePtr,
    int                 flag,
    int                 type,
    int                 structure );

TM_PROTO_EXTERN int tfFSReadFile(
    void TM_FAR * userDataPtr,
    void TM_FAR * fileDataPtr,
    char TM_FAR * bufferPtr,
    int           bytes );

TM_PROTO_EXTERN int tfFSReadFileRecord(
    void TM_FAR * userDataPtr,
    void TM_FAR * fileDataPtr,
    char TM_FAR * bufferPtr,
    int           bytes,
    int  TM_FAR * eorPtr );

TM_PROTO_EXTERN int tfFSWriteFile(
    void TM_FAR *       userDataPtr,
    void TM_FAR *       fileDataPtr,
    const char TM_FAR * bufferPtr,
    int                 bytes );

TM_PROTO_EXTERN int tfFSWriteFileRecord(
    void TM_FAR * userDataPtr,
    void TM_FAR * fileDataPtr,
    char TM_FAR * bufferPtr,
    int           bytes,
    int           eor );

TM_PROTO_EXTERN int tfFSFlushFile(
    void TM_FAR * userDataPtr,
    void TM_FAR * fileDataPtr );

TM_PROTO_EXTERN int tfFSCloseFile(
    void TM_FAR * userDataPtr,
    void TM_FAR * fileDataPtr );

/* Flag is either TM_DIR_SHORT or TM_DIR_LONG */
TM_PROTO_EXTERN void TM_FAR * tfFSOpenDir(
    void TM_FAR * userDataPtr,
    const char TM_FAR * pathNamePtr,
    int           flag );

TM_PROTO_EXTERN void tfFSCloseDir(
    void TM_FAR * userDataPtr,
    void TM_FAR * dirDataPtr );

TM_PROTO_EXTERN int tfFSChangeDir(
    void TM_FAR * userDataPtr,
    const char TM_FAR * pathNamePtr );

TM_PROTO_EXTERN int tfFSChangeParentDir(
    void TM_FAR * userDataPtr);

TM_PROTO_EXTERN int tfFSRemoveDir(
    const void TM_FAR * userDataPtr,
    const char TM_FAR * pathNamePtr );

TM_PROTO_EXTERN int tfFSMakeDir(
    const void TM_FAR * userDataPtr,
    const char TM_FAR * pathNamePtr,
    char TM_FAR *       bufferPtr,
    int                 bufferSize );

TM_PROTO_EXTERN int tfFSGetWorkingDir(
    const void TM_FAR * userDataPtr,
    char TM_FAR *       bufferPtr,
    int                 bufferSize );

TM_PROTO_EXTERN int tfFSGetNextDirEntry(
    void TM_FAR * userDataPtr,
    void TM_FAR * dirDataPtr,
    char TM_FAR * bufferPtr,
    int           maxBytes );

/* This interface function expects the user to provide the most efficient 
 * file block size. Normally it would be the cluster size of the 
 * underlying file system. This is used to optimize the file read and 
 * write performance. If returns zero, the default size 
 * TM_FS_FILE_BLOCK_SIZE (512) will be used by the stack.
 */
TM_PROTO_EXTERN int tfFSGetFileBlockSize(void);

/*
 * SNMP agent entry point
 */
#define tm_blocking_state(flags) (flags & TM_BLOCKING_ON)
/*
 * SNMP Research agent only:
 * Set this flag in the snmpFlags parameter to send a warm start instead
 * of a cold start when the SNMP agent is started.
 */
#define TM_SNMP_WARM_START             (0x100)
TM_PROTO_EXTERN int tfSnmpdMain(
    int           blockingState,
    ttUser32Bit   timerInterval );


/*
 * TELNETD USER API
 */
/*
 * Telnet server flags. What options the user allows the telnet client
 * Used as flags in the telnetOptionsAllowed of tfTeldUserStart().
 */

#define TM_TELD_BINARY_ON 0x1
#define TM_TELD_ECHO_ON   0x2

/* TELNETD handle */
typedef void TM_FAR * ttUserTeldHandle;

/* Stop the TELNET server */
TM_PROTO_EXTERN int tfTeldUserStop(void);
/* Execute the TELNET server (non blocking mode only) */
TM_PROTO_EXTERN int tfTeldUserExecute(void);
/* Start the TELNET server */
TM_PROTO_EXTERN int tfTeldUserStart(
    int telnetOptionsAllowed,
    int maxConnections,
    int maxBackLog,
    int blockingState );
/*
 * Increase the default 300 seconds idle telnet server timeout. Should be
 * called after tfTeldUserStart() for the change to take effect.
 */
int tfTeldUserIdleTimeout(ttUser32Bit timeout);


/*
 * User give data to the telnet server to send to the telnet client.
 * If TM_TELD_SEND_EOL flag is set, indicates whether end of line has been
 * reached, and means that the telnet server need to send CR, LF in ascii
 * mode, or IAC, EOR in binary mode .
 * If TM_TELD_SEND_COMMAND flag is set, indicates that the user wants to send
 * an IAC command, rather than data, and that the IAC character should
 * not be mapped.
 * Returns TM_ENOERROR if the telnet server buffered the data,
 * TM_EWOULDBLOCK if there is no room for the data, or TM_ENOBUFS if
 * we failed to allocate a buffer.
 */
#define TM_TELD_SEND_EOL     0x01
#define TM_TELD_SEND_COMMAND 0x02

TM_PROTO_EXTERN int tfTeldUserSend(
    ttUserTeldHandle teldHandle,
    char TM_FAR *    teldSendBufPtr,
    int              teldBytes,
    int              flag );

/*
 * User forces the telnet server to close the specified telnet connection
 */
TM_PROTO_EXTERN int tfTeldUserClose( ttUserTeldHandle teldHandle );

/*
 * Called from the telnet server to let the user know that the specified
 * telnet client has established an new connection to the telnet server.
 */
TM_PROTO_EXTERN void tfTeldOpened(
    ttUserTeldHandle            teldHandle,
    struct sockaddr_in TM_FAR * sockAddrPtr );


/*
 * Called from the telnet server to give incoming data to the user
 * If eolFlag is set, then CR,LF, or CR,NUL has been received in ascii mode.
 * In binary mode, then IAC,EOR has been received.
 * Return TM_ENOERROR on success, TM_EWOULDBLOCK if the user has
 * no room for the data.
 */
TM_PROTO_EXTERN int tfTeldIncoming(
    ttUserTeldHandle teldHandle,
    char    TM_FAR * teldRecvBufPtr,
    int              teldBytes,
    int              eolFlag );
/*
 * Called from the telnet server to let the user know that
 * the telnet client has closed the specified telnet connection.
 */
TM_PROTO_EXTERN void tfTeldClosed(ttUserTeldHandle teldHandle);

/*
 * Returns the number of bytes currently in the TCP socket send queue for
 * this telnet session.
 */
TM_PROTO_EXTERN int tfTeldSendQueueBytes(ttUserTeldHandle teldHandle);

/*
 * Returns the total size of the TCP socket send queue for this telnet
 * session.
 */
TM_PROTO_EXTERN int tfTeldSendQueueSize(ttUserTeldHandle teldHandle);

/*
 * Treck Test API
 */

/*
 * Test type macros
 */
#define TM_TEST_UDP                (0)
#define TM_TEST_UDP_SEND           (1)
#define TM_TEST_UDP_RECV           (2)
#define TM_TEST_UDP_ECHO_CLIENT    (3)
#define TM_TEST_UDP_ECHO_SERVER    (4)

#define TM_TEST_TCP                (256)
#define TM_TEST_TCP_SEND           (257)
#define TM_TEST_TCP_RECV           (258)
#define TM_TEST_TCP_ECHO_CLIENT    (259)
#define TM_TEST_TCP_ECHO_SERVER    (260)
#define TM_TEST_TCP_CONNECT        (261)

#define TM_TEST_MISC               (512)
#define TM_TEST_LOCK               (513)

/* Test flags */
#define TM_TEST_FLAG_ZEROCOPY      (0x0001)
#define TM_TEST_FLAG_NONBLOCKING   (0x0002)
#define TM_TEST_FLAG_UDP_CONNECT   (0x0004)
#define TM_TEST_FLAG_UDP_CS_OFF    (0x0008)
#define TM_TEST_FLAG_TCP_NODELAY   (0x0010)
#define TM_TEST_FLAG_VALIDATE      (0x0020)
#define TM_TEST_FLAG_RANDOM        (0x0040)
#define TM_TEST_FLAG_FILL_DATA     (0x0080)

#define TM_TEST_VALID_FLAGS        (0x00FF)

typedef void TM_FAR * ttUserTestHandle;
typedef ttUserTestHandle TM_FAR * ttUserTestHandlePtr;

/*
 * Performs the specified test.  In blocking mode, returns when the test is
 * complete; in non-blocking mode, returns immediately, and the user should
 * call tfTestUserExecute with the returned test handle.
 */
TM_PROTO_EXTERN int tfTestTreck(
    int                 testType,
    char TM_FAR *       ipAddress,
    int                 portNumber,
    int                 dataSize,
    ttUser32Bit         testCount,
    ttUser32Bit         flags,
    ttUserTestHandlePtr testHandlePtr,
    void TM_FAR *       testParam );

TM_PROTO_EXTERN int tfTestUserExecute(ttUserTestHandle testSessionPtr);

/*
 * DNS Resolver User API
 */

/*
 * DNS error codes
 */

#define TM_DNS_ERROR_BASE   7000

/* Format error - The name server was unable to interpret the query */
#define TM_DNS_EFORMAT      (TM_DNS_ERROR_BASE + 1)


/*
 * Server Failure - The name server was unable to process this query due to a
 * problem with the name server.
 */
#define TM_DNS_ESERVER      (TM_DNS_ERROR_BASE + 2)

/*
 * Name Error - Meaningful only for responses from an authoritative name
 * server, this code signifies that the domain name referenced in the query
 * does not exist.
 */
#define TM_DNS_ENAME_ERROR  (TM_DNS_ERROR_BASE + 3)

/*
 * Not Implemented - The name server does not support the requested kind of
 * query.
 */
#define TM_DNS_ENOT_IMPLEM  (TM_DNS_ERROR_BASE + 4)

/*
 * Refused - The name server refuses to perform the specified operation for
 * policy reasons.  For example, a name server may not wish to provide the
 * information to the particular requester, or a name server may not wish to
 * perform a particular operation (e.g., zone transfer) for particular data.
 */
#define TM_DNS_EREFUSED     (TM_DNS_ERROR_BASE + 5)

/* Error codes 6-15 are reserver, per RFC */

/*
 * No answer received from name server (i.e., response packet received,
 * but it did not contain the answer to our query).
 */
#define TM_DNS_EANSWER      (TM_DNS_ERROR_BASE + 16)

/*
 * DNS request/reply cache is full.
 */
#define TM_DNS_ECACHE_FULL  (TM_DNS_ERROR_BASE + 17)


/*
 * DNS Option types
 */
#define TM_DNS_OPTION_CACHE_SIZE 0
#define TM_DNS_OPTION_RETRIES    1
#define TM_DNS_OPTION_TIMEOUT    2

/*
 * DNS Server numbers
 */
#define TM_DNS_PRI_SERVER 0
#define TM_DNS_SEC_SERVER 1

/*
 * Structure used to return address information for a hostname to a user by
 * getaddrinfo, and used by a user to specify what sort of addresses should be
 * returned by getaddrinfo.
 */
struct addrinfo
{
/* AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST */
    int                      ai_flags;

/* PF_INET, PF_INET6 */
    int                      ai_family;

/* SOCK_xxx (not used) */
    int                      ai_socktype;

/* 0 or IPPROTO_xxx (not used) */
    int                      ai_protocol;

/* length of ai_addr */
    unsigned int             ai_addrlen;
#define ai_mxpref ai_addrlen

/* canonical name for nodename */
    char TM_FAR *            ai_canonname;
#define ai_mxhostname ai_canonname

/* binary address */
    struct sockaddr TM_FAR * ai_addr;

/* next structure in linked list */
    struct addrinfo TM_FAR * ai_next;
};

/* Initializes the resolver services; called once when the system is started */
TM_PROTO_EXTERN int tfDnsInit(int blockingMode);

/* Sets various options for the DNS resolver (cache size, timeouts, etc). */
TM_PROTO_EXTERN int tfDnsSetOption(
    int optionType,
    int optionValue);

/* Sets the primary or secondary name server address */
TM_PROTO_EXTERN int tfDnsSetServer(
    ttUserIpAddress serverIpAddr,
    int             serverNumber);

/* Returns the IP Address associated with the given hostname. */
TM_PROTO_EXTERN int tfDnsGetHostByName(
    const char TM_FAR * hostnameStr,
    ttUserIpAddressPtr  ipAddressPtr );

/* Returns the IP address associated with the given hostname.  */
TM_PROTO_EXTERN int tfDnsGetHostByAddr(
    ttUserIpAddress serverIpAddr,
    char TM_FAR *   hostnameStr,
    int             hostnameStrLength );

/* Returns the IP address of any additional mail server for this hostname */
TM_PROTO_EXTERN int tfDnsGetNextMailHost(
    const char TM_FAR *     hostnameStr,
    ttUserIpAddress         lastIpAddress,
    unsigned short          lastPref,
    ttUserIpAddressPtr      ipAddressPtr,
    unsigned short TM_FAR * mxPrefPtr );

/* Returns the IP address of the primary mail server for this hostname.  */
TM_PROTO_EXTERN int tfDnsGetMailHost(
    const char TM_FAR *      hostnameStr,
    ttUserIpAddressPtr       ipAddressPtr,
    unsigned short TM_FAR *  mxPrefPtr );

/*
 * Sets the address of the primary and secondary DNS server.  To set the
 * primary DNS server serverNumber should be set to TM_DNS_PRI_SERVER; for the
 * secondary server it should be set to TM_DNS_SEC_SERVER.  To remove a
 * previously set entry, set serverAddrPtr to NULL.
 */
int tfNgDnsSetServer(struct sockaddr_storage TM_FAR * serverAddrPtr,
                     int                              serverNumber);

/*
 * Translates a node-name (hostname) to an address.  Similar to gethostbyname,
 * used for both IPv4 and IPv6.  Retrieving a service name or port number is
 * not supported in our implementation of getaddrinfo, so serviceName must be
 * set to NULL.
 */
int getaddrinfo(const char TM_FAR *               nodeName,
                const char TM_FAR *               serviceName,
                const struct addrinfo TM_FAR *    hintsPtr,
                struct addrinfo TM_FAR * TM_FAR * resPtrPtr);

/*
 * Translates an address into it's corresponding nodename (hostname).  Similar
 * to gethostbyaddr.  Retrieving a service name or port number is not
 * supported in our implementation of getnameinfo, so serviceName must be set
 * to NULL.
 */
int getnameinfo( const struct sockaddr TM_FAR * addressPtr,
                 int           addressLength,
                 char TM_FAR * hostname,
                 int           hostnameLength,
                 char TM_FAR * serviceName,
                 int           serviceNameLength,
                 int           flags );

/*
 * Frees memory used by address information structure chain returned by
 * getaddrinfo.
 */
void freeaddrinfo(struct addrinfo TM_FAR * addrInfoPtr);

/*
 * Returns a string describing the error code, returned from getaddrinfo
 * or getnameinfo.
 */
char TM_FAR * gai_strerror(int errorCode);

/*
 * getaddrinfo/getnameinfo return codes.  Description can be retrieved
 * using gai_strerror.
 */
#define EAI_AGAIN     -1
#define EAI_BADFLAGS  -2
#define EAI_FAIL      -3
#define EAI_FAMILY    -4
#define EAI_MEMORY    -5
#define EAI_NONAME    -6
#define EAI_SERVICE   -7
#define EAI_SOCKTYPE  -8
#define EAI_SYSTEM    -9
#define EAI_OVERFLOW  -10

/*
 * Flags for addrinfo structure for getaddrinfo.
 */

/* Not supported */
/* #define AI_PASSIVE  0x0001 */

/* Request for canonical name */
#define AI_CANONNAME   0x0002

/* Don't use name resolution */
#define AI_NUMERICHOST 0x0004

/* Not supported */
/* #define AI_NUMERICSERV 0x0008 */

/* Return IPv4 addresses in IPv4-mapped format if no native IPv6 are found. */
#define AI_V4MAPPED    0x0010

/* Return all IPv4 and IPv6 addresses found. */
#define AI_ALL         0x0020

/*
 * Return IPv4 addresses only if IPv4 is configured on this device; return
 * IPv6 addresses only if IPv6 is configured on this device.
 */
#define AI_ADDRCONFIG 0x0040

/* Allowed flags for getaddrinfo call */
#define TM_GETADDRINFO_FLAGS (AI_CANONNAME  | AI_NUMERICHOST | \
                              AI_V4MAPPED   | AI_ALL         | \
                              AI_ADDRCONFIG )

/*
 * Flags for getnameinfo
 */
/* Not supported */
/* #define NI_NOFQND       0x0001 */

/* Don't do a DNS lookup for this address - just return the address as a
   numeric string. */
#define NI_NUMERICHOST  0x0002
/* Return an error if the host's name cannot be found. */
#define NI_NAMEREQD     0x0004

/* Not supported. */
/* #define NI_NUMERICSERV  0x0008 */
/* Not supported. */
/* #define NI_NUMERICSCOPE 0x0010 */
/* Not supported. */
/* #define NI_DGRAM        0x0020 */




TM_PROTO_EXTERN void TM_FAR * tfDeviceClearPointer(
    ttUserInterface interfaceHandle);

TM_PROTO_EXTERN void TM_FAR * tfDeviceGetPointer(
    ttUserInterface interfaceHandle);

TM_PROTO_EXTERN int tfDeviceStorePointer(ttUserInterface interfaceHandle,
    void TM_FAR   * deviceDriverPtr);

TM_PROTO_EXTERN ttUserContext tfCreateTreckContext(void);

#ifdef TM_MULTIPLE_CONTEXT

TM_PROTO_EXTERN ttUserContext tfGetCurrentContext (void);

TM_PROTO_EXTERN void tfInitTreckMultipleContext(void);

TM_PROTO_EXTERN void tfSetCurrentContext(ttUserContext contextHandle);

#define tm_set_current_context(contextHandle)           \
                    tfSetCurrentContext(contextHandle)
#define tm_get_current_context(contextHandle)           \
                    contextHandle = tfGetCurrentContext()
#else  /* !TM_MULTIPLE_CONTEXT */

#define tm_set_current_context(contextHandle)
#define tm_get_current_context(contextHandle)

#endif /* TM_MULTIPLE_CONTEXT */

/*
 * IP header compression entry point
 */
TM_PROTO_EXTERN int tfUseIpHdrComp(ttUserLinkLayer linkLayer);

/*
 * Mobile IP notification events
 */

/* Mobile IP is opened and the mobile node has left it's home subnet. */
#define TM_MIP_NOTIFY_OPEN   0

/* The mobile node has bound to either it's home agent or to a foreign
   agent. */
#define TM_MIP_NOTIFY_BOUND  1

/*
 * Mobile IP is closed, either due to a user request or because the node has
 * moved back to it's home subnet.
 */
#define TM_MIP_NOTIFY_CLOSE  2

/*
 * The mobile node is on a foreign subnet but couldn't obtain a valid
 * care-of address.
 */
#define TM_MIP_NOTIFY_NO_COA 3

#define TM_MIP_NOTIFY_UNREG  4

/*
 * Mobile IP foreign agent authentication return codes
 */

/*
 * Authentication information is not currently available and must be
 * acquired.
 */
#define TM_MIP_AUTH_WAIT 1

/*
 * An error occurred while retrieving authentication information.  Retry
 * registration without using foreign agent authentication.
 */
#define TM_MIP_AUTH_NONE 2

/*
 * An error occurred while retrieving authentication information.  Do not
 * try to re-register with this foreign agent.
 */
#define TM_MIP_AUTH_FAIL 3

/*
 *tfMipSetGetOption flags
 */
/* Set Mobile IP option. */
#define TM_MIP_SET_OPTION 0
/* Get Mobile IP option/status. */
#define TM_MIP_GET_OPTION 1


/*
 * tfMipConfig flags
 */

/* Mobile IP is being opened. */
#define TM_MIP_OPEN  0
/* Mobile IP is being closed. */
#define TM_MIP_CLOSE 1

/*
 * Errors returned from foreign agent
 */
/* reason unspecified */
#define TM_MIP_EUNSPEC_FA   64

/* administratively prohibited */
#define TM_MIP_EADMIN_FA    65

/* insufficient resources */
#define TM_MIP_ERESRC_FA    66

/* mobile node failed authentication */
#define TM_MIP_EMNAUTH _FA  67

/* home agent failed authentication */
#define TM_MIP_EHAAUTH _FA  68

/* requested Lifetime too long */
#define TM_MIP_ELIFETIME    69

/* poorly formed Request */
#define TM_MIP_EBADREQ_FA   70

/* poorly formed Reply */
#define TM_MIP_EBADREP      71

/* requested encapsulation unavailable */
#define TM_MIP_EENCUNAVAIL  72

/* requested Van Jacobson compression unavailable */
#define TM_MIP_EVJUNAVAIL   73

/* home network unreachable (ICMP error received) */
#define TM_MIP_ENETUNREACH  80

/* home agent host unreachable (ICMP error received) */
#define TM_MIP_EHOSTUNREACH 81

/* home agent port unreachable (ICMP error received) */
#define TM_MIP_EPORTUNREACH 82

/* home agent unreachable (other ICMP error received) */
#define TM_MIP_EUNREACH     88

/*
 * Errors returned from home agent
 */
/* reason unspecified */
#define TM_MIP_EUNSPEC_HA   128

/* administratively prohibited */
#define TM_MIP_EADMIN_HA    129

/* insufficient resources */
#define TM_MIP_ERESRC_HA    130

/* mobile node failed authentication */
#define TM_MIP_EMNAUTH_HA   131

/* foreign agent failed authentication */
#define TM_MIP_EFAAUTH_HA   132

/* registration Identification mismatch */
#define TM_MIP_EIDENT       133

/* poorly formed Request */
#define TM_MIP_EBADREQ_HA   134

/* too many simultaneous mobility bindings */
#define TM_MIP_EBINDINGS    135

/* unknown home agent address */
#define TM_MIP_EHAADDR      136

#define TM_MIP_OPTION_BASE 0x8000
/* Enable DHCP */
#define TM_MIP_OPT_DHCP_ENB       TM_MIP_OPTION_BASE
/* IP address of the node's home agent */
#define TM_MIP_HOME_AGENT         (TM_MIP_OPTION_BASE+1)
/* The mobile node's home IP address. */
#define TM_MIP_HOME_ADDR          (TM_MIP_OPTION_BASE+2)
/* The mobile node's home IP netmask. */
#define TM_MIP_HOME_NETMASK       (TM_MIP_OPTION_BASE+3)
/* Always attempt to use a co-located care-of address. */
#define TM_MIP_OPT_COLOCATE       (TM_MIP_OPTION_BASE+4)
/* Number of times to retry sending a registration request. */
#define TM_MIP_REG_RETRY          (TM_MIP_OPTION_BASE+5)
/* Minimum amount of time to wait before resending a registration request. */
#define TM_MIP_REG_TIMEOUT        (TM_MIP_OPTION_BASE+6)
/* Number of times to retry sending an agent solicitation. */
#define TM_MIP_AGT_DISC_RETRY     (TM_MIP_OPTION_BASE+7)
/* Amount of time (in secs) to wait before resending a agent solicitation. */
#define TM_MIP_AGT_DISC_TIMEOUT   (TM_MIP_OPTION_BASE+8)
/*
 * Number of times to retry sending an agent solicitation when rediscovering
 * an agent.
 */
#define TM_MIP_AGT_RDISC_RETRY    (TM_MIP_OPTION_BASE+9)
/*
 * Amount of time (in seconds) to wait before resending an agent solicitation
 * when rediscovering an agent.
 */
#define TM_MIP_AGT_RDISC_TIMEOUT  (TM_MIP_OPTION_BASE+10)
/*
 * A pointer to a function used for authenticating the mobile node to a
 * foreign node.
 */
#define TM_MIP_FA_AUTH_FUNC       (TM_MIP_OPTION_BASE+11)
/* The shared secret used to authenticate the mobile node to the home agent. */
#define TM_MIP_HA_SECRET          (TM_MIP_OPTION_BASE+12)
/*
 * Security Parameter Index.  An index identifying a security context between
 * a pair of nodes.
 */
#define TM_MIP_HA_SPI             (TM_MIP_OPTION_BASE+13)
/*
 * Use the Prefix-Length extension in Agent Advertisements for move
 * detection.
 */
#define TM_MIP_USE_PREFIX_LEN     (TM_MIP_OPTION_BASE+14)
/*
 * Request that the home agent tunnel any broadcast packets received on the
 * home network to the mobile node.
 */
#define TM_MIP_BCAST              (TM_MIP_OPTION_BASE+15)
/*
 * The amount of time, in seconds, to downward bias the Registration Lifetime
 * returned from the mobility agent.  This can be used to force Mobile IP to
 * re-register more frequently than required by the agent.
 */
#define TM_MIP_REG_LT_BIAS        (TM_MIP_OPTION_BASE+16)
/*
 * If the mobile node cannot register with a foreign agent, retry with the
 * home agent.
 */
#define TM_MIP_REG_RETRY_HA       (TM_MIP_OPTION_BASE+17)
/*
 * If the mobile node cannot register with the home agent, try to discover
 * the address of an additional home agent.
 */
#define TM_MIP_REG_RETRY_DISC     (TM_MIP_OPTION_BASE+18)
/* Default lifetime (in seconds) to request from the mobility agent. */
#define TM_MIP_DEF_REG_LIFETIME   (TM_MIP_OPTION_BASE+19)
/*
 * The current registration status of this mobile node (registration pending,
 * deregistered on home network, etc)
 */
#define TM_MIP_REG_STATUS         (TM_MIP_OPTION_BASE+20)

#define TM_MIP_LAST_OPTION        TM_MIP_REG_STATUS

/*
 * User registration status codes
 */

/* Deregistration pending on home subnet */
#define TM_MIP_DEREG_PENDING 0

/* Deregistered on home subnet */
#define TM_MIP_DEREG         1

/* Move detection pending */
#define TM_MIP_MOVE_PENDING  2

/* Registration pending on foreign subnet */
#define TM_MIP_REG_PENDING   3

/* Registered on foreign subnet */
#define TM_MIP_REG           4

/* Registration stopped/failed on foreign subnet */
#define TM_MIP_REG_STOPPED   5


/* Pointer to user's Mobile IP notification function */
typedef void (TM_CODE_FAR *ttUserMobileNotifyFuncPtr)(
    ttUserInterface interfaceHandle,
    ttUserIpAddress ipAddress,
    int             event,
    int             errorCode);


/* Pointer to user's Mobile IP authentication function */
typedef int (TM_CODE_FAR *ttUserMobileAuthFuncPtr)(
    ttUserInterface        interfaceHandle,
    ttUserIpAddress        ipAddress,
    char TM_FAR * TM_FAR * secretPtrPtr,
    int TM_FAR *           secretLen,
    ttUser32Bit TM_FAR *   secParamIndexPtr);

/* Enables the use of Mobile IP on the specified interface.  */
int tfUseMobileIp(
    ttUserInterface           interfaceHandle,
    ttUserMobileNotifyFuncPtr notifyFuncPtr,
    ttUser32Bit               flags);

int tfMobileIpReRegister(
    ttUserInterface interfaceHandle,
    int             flags);

int tfMobileIpMoveDetect(
     ttUserInterface interfaceHandle);

int tfMobileIpMoveNotify(
    ttUserInterface interfaceHandle,
    int             flags);


/* Enables the reception of encapsulated IP packets. */
void tfUseIpTunneling( void );


/*
 * NAT user API.
 * (All IP addresses and ports are in host byte order.)
 */

int tfNatConfig ( ttUserInterface interfaceHandle );

int tfNatUnConfig ( ttUserInterface interfaceHandle );

int tfNatConfigStatic (
    ttUserInterface interfaceHandle,
    ttUserIpAddress ipPublic,
    ttUserIpAddress ipPrivate );

int tfNatConfigDynamic (
    ttUserInterface interfaceHandle,
    ttUserIpAddress ipPublic );

int tfNatConfigNapt (
    ttUserInterface interfaceHandle,
    ttUserIpAddress ipPublic,
    ttUserIpPort    portPublicMin,
    ttUserIpPort    portPublicMax );  /* inclusive */

int tfNatConfigInnerTcpServer (
    ttUserInterface interfaceHandle,
    ttUserIpAddress ipPublic,
    ttUserIpAddress ipPrivate,
    ttUserIpPort    portPublic,
    ttUserIpPort    portPrivate );

int tfNatConfigInnerFtpServer (
    ttUserInterface interfaceHandle,
    ttUserIpAddress ipPublic,
    ttUserIpAddress ipPrivate,
    ttUserIpPort    portPublicMin,   /* for PASV */
    ttUserIpPort    portPublicMax ); /* connects */

int tfNatConfigInnerUdpServer (
    ttUserInterface interfaceHandle,
    ttUserIpAddress ipPublic,
    ttUserIpAddress ipPrivate,
    ttUserIpPort    portPublic,
    ttUserIpPort    portPrivate );

void tfNatConfigMaxEntries ( int maxentries );

#ifdef TM_NAT_DUMP
void tfNatDump ( ttUserInterface interfaceHandle );
#endif /* TM_NAT_DUMP */

/* User-specified function used to walk the debug/error/trace log, refer to
   tfWalkLog. Returns 0 to continue the walk, otherwise returns non-zero to
   terminate the walk. */
typedef int (*ttLogWalkFuncPtr)(
    int                 msgSeqNo,
    const char TM_FAR * pszLogMsg,
    ttUserGenericUnion  genParam);

/* Maximum number of data rows returned by query using a cursor. */
#define TM_CURSOR_MAX_ROWS 4

/* Check the status of either IPv4 or IPv6 protocol operation on the specified
   interface. */
TM_PROTO_EXTERN int tfCheckOpenInterface(
    ttUserInterface interfaceHandle,
    int             protocolFamily );

TM_PROTO_EXTERN int tfSetReachable(
    struct sockaddr_storage TM_FAR * addrPtr,
    ttUserInterface                  interfaceHandle,
    ttUser32Bit TM_FAR *             reachableMsecPtr);

/* IPv4-specific functions supporting tfNg "Next Generation" APIs */
/* convert an IPv4 netmask into a prefix length */
TM_PROTO_EXTERN int tf4NetmaskToPrefixLen(ttUserIpAddress netMask);


#ifdef TM_USE_NETSTAT
/* netstat API */
/* for the tableId parameter for tfNetstat */
typedef enum teNtTableId
{
    TM_NT_TABLE_ARP,
    TM_NT_TABLE_RTE,
    TM_NT_TABLE_TCP,
    TM_NT_TABLE_UDP,
    TM_NT_TABLE_DEVICE

} ttNtTableId;

/* 
 * TM_NT_MAX_IP_PER_IF
 * Max number of IP address per interface supported by netstat
 * which should be:
 *            TM_MAX_IPS_PER_IF
 *          + TM_6_MAX_AUTOCONF_IPS_PER_IF  (IPV6 only)
 *          + TM_6_MAX_MIP_PROXY_IPS_PER_IF (IPv6 only)
 */

#ifndef TM_MAX_IPS_PER_IF
#define TM_NT_MAX_IPS_PER_IF 4 /* match worst case value in trmacro.h */
#else /* TM_MAX_IPS_PER_IF */
/* Use trsystem.h redefinition */
#define TM_NT_MAX_IPS_PER_IF TM_MAX_IPS_PER_IF
#endif /* TM_MAX_IPS_PER_IF */


#define TM_NT_MAX_IP_PER_IF TM_NT_MAX_IPS_PER_IF


#define TM_NT_ENTRY_STR_LEN     116 /* TCP round(114 + 1) */

/* max length of a physical addr */
#define TM_NT_MAX_DEVICE_NAME   14
#define TM_NT_MAX_PHYS_ADDR     6

/* routing entry flag bits */
#define TM_NT_RTE_UP         0x0001
#define TM_NT_RTE_HOST       0x0002
#define TM_NT_RTE_GW         0x0004
#define TM_NT_RTE_DYNAMIC    0x0008
#define TM_NT_RTE_REJECT     0x0010
#define TM_NT_RTE_REDIRECT   0x0020
#define TM_NT_RTE_CLONABLE   0x0040
#define TM_NT_RTE_CLONED     0x0080
#define TM_NT_RTE_STATIC     0x0100
#define TM_NT_RTE_LINK_LAYER 0x0200

/* HW types */
#define TM_NT_HWTYPE_ETHERNET   0
#define TM_NT_HWTYPE_TOKENRING  1
#define TM_NT_HWTYPE_TOKENBUS   2
#define TM_NT_HWTYPE_NULL       3

/* TCP state */
#define TM_NT_TCPS_CLOSED       0
#define TM_NT_TCPS_LISTEN       1
#define TM_NT_TCPS_SYN_SENT     2
#define TM_NT_TCPS_SYN_RECEIVED 3
#define TM_NT_TCPS_ESTABLISHED  4
#define TM_NT_TCPS_CLOSE_WAIT   5
#define TM_NT_TCPS_FIN_WAIT_1   6
#define TM_NT_TCPS_CLOSING      7
#define TM_NT_TCPS_LAST_ACK     8
#define TM_NT_TCPS_FIN_WAIT_2   9
#define TM_NT_TCPS_TIME_WAIT    10
#define TM_NT_TCPS_INVALID      20 /* invalid state */

/* information of a ARP entry */
typedef struct tsNtArpEntry
{
    struct sockaddr_storage ntArpSockAddr;
    ttUser8Bit              ntArpHwAddress[tm_align(TM_NT_MAX_PHYS_ADDR)];
    int                     ntArpHwLength;
    int                     ntArpHwType;
    ttUserInterface         ntArpInterfaceHandle;
    ttUser32Bit             ntArpOwnerCount;
    ttUser8Bit              ntArpDeviceName
                            [tm_align(TM_NT_MAX_DEVICE_NAME)];
    ttUser8Bit              ntArpNudState;
    ttUser32Bit             ntArpTtl;
    ttUser8Bit              filler[3];
} ttNtArpEntry;
typedef ttNtArpEntry TM_FAR * ttNtArpEntryPtr;

/* information of a routing entry */
typedef struct tsNtRteEntry
{
    struct sockaddr_storage ntRteDestSockAddr;
    ttUser8Bit              ntRtePrefixLength;
    ttUser32Bit             ntRteOwnerCount;
    ttUser32Bit             ntRteMhomeIndex;
    char                    ntRteDeviceName
                            [tm_align(TM_NT_MAX_DEVICE_NAME)];
    ttUserInterface         ntRteInterfaceHandle;
    struct sockaddr_storage ntRteGwSockAddr;
    ttUser8Bit              ntRteHwAddress
                            [tm_align(TM_NT_MAX_PHYS_ADDR)];
    int                     ntRteHwLength;
    ttUser8Bit              ntRteClonePrefixLength;
    ttUser32Bit             ntRteMtu;
    ttUser32Bit             ntRteHops;
    ttUser32Bit             ntRteTtl;
    ttUser16Bit             ntRteFlags;
} ttNtRteEntry;
typedef ttNtRteEntry TM_FAR * ttNtRteEntryPtr;

/* information for a TCP socket entry */
typedef struct tsNtTcpEntry
{
    ttUser16Bit             ntTcpSockDesc;
    ttUser32Bit             ntTcpBytesInRecvQ;
    ttUser32Bit             ntTcpBytesInSendQ;
    ttUser32Bit             ntTcpRecvQSize;
    ttUser32Bit             ntTcpSendQSize;
/*
 * V4 mapped address for a socket entry is stored as a V4 address
 * in ntTcpEntryPtr->ntTcpLocalSockAddr
 */
    struct sockaddr_storage ntTcpLocalSockAddr;
    ttUser16Bit             ntTcpOwnerCount;
/*
 * V4 mapped address for a socket entry is stored as a V4 address
 * in ntTcpEntryPtr->ntTcpPeerSockAddr
 */
    struct sockaddr_storage ntTcpPeerSockAddr;
    ttUser16Bit             ntTcpFlags;
    ttUser32Bit             ntTcpRto;
    ttUser32Bit             ntTcpReXmitCnt;
    ttUser32Bit             ntTcpSndUna;
    ttUser32Bit             ntTcpIss;
    ttUser32Bit             ntTcpSndNxt;
    ttUser32Bit             ntTcpMaxSndNxt;
    ttUser32Bit             ntTcpIrs;
    ttUser32Bit             ntTcpSndWL1;
    ttUser32Bit             ntTcpSndWL2;
    ttUser32Bit             ntTcpMaxSndWnd;
    ttUser32Bit             ntTcpRcvNxt;
    ttUser32Bit             ntTcpRcvWnd;
    ttUser32Bit             ntTcpRcvAdv;
    ttUser32Bit             ntTcpCwnd;
    ttUser32Bit             ntTcpSsthresh;
    ttUser16Bit             ntTcpBackLog;
    ttUser8Bit              ntTcpDupAck;
    ttUser8Bit              ntTcpAcksAfterRexmit;
    ttUser8Bit              ntTcpState;
    ttUser8Bit              ntTcpFiller[3];
} ttNtTcpEntry;
typedef ttNtTcpEntry TM_FAR * ttNtTcpEntryPtr;

/* information for a UDP socket entry */
typedef struct tsNtUdpEntry
{
    ttUser16Bit             ntUdpSockDesc;
    ttUser32Bit             ntUdpBytesInRecvQ;
    ttUser32Bit             ntUdpBytesInSendQ;
    ttUser32Bit             ntUdpRecvQSize;
    ttUser32Bit             ntUdpSendQSize;
/*
 * V4 mapped address for a socket entry is stored as a V4 address
 * in ntUdpEntryPtr->ntTcpLocalSockAddr
 */
    struct sockaddr_storage ntUdpLocalSockAddr;
    ttUser16Bit             ntUdpOwnerCount;
} ttNtUdpEntry;
typedef ttNtUdpEntry TM_FAR * ttNtUdpEntryPtr;




/* address configuration type */
#define TM_NT_ADDR_CONF_TYPE_NONE     0
#define TM_NT_ADDR_CONF_TYPE_MANUAL   1
#define TM_NT_ADDR_CONF_TYPE_AUTO     2
#define TM_NT_ADDR_CONF_TYPE_PROXY    3

/* IPV4 address configuration status */
#define TM_NT_ADDR_STATUS_UNKOWN      0
#define TM_NT_ADDR_STATUS_CONFIGURED  1
#define TM_NT_ADDR_STATUS_CONFIGURING 2
#define TM_NT_ADDR_STATUS_DEPRECATED  3

/* device status */
#define TM_NT_DEVICE_STATUS_CLOSED      0
#define TM_NT_DEVICE_STATUS_OPEN        1
#define TM_NT_DEVICE_STATUS_CONNECTING  2
#define TM_NT_DEVICE_STATUS_CONNECTED   3

/* information for a IPv4 address configuration entry */
typedef struct tsNtAddrEntry
{
    struct in_addr      ntAddrAddr;
    ttUser8Bit          ntAddrPrefixLen;
    ttUser8Bit          ntAddrStatus;
    ttUser8Bit          ntAddrMHome;
    ttUser8Bit          ntAddrType;
} ttNtAddrEntry;
typedef ttNtAddrEntry TM_FAR * ttNtAddrEntryPtr;


typedef struct tsNtDevEntry
{
    char            ntDevName[tm_align(TM_NT_MAX_DEVICE_NAME)];
    char            ntDevPhyAddr[tm_align(TM_NT_MAX_PHYS_ADDR)];
    ttUser8Bit      ntDevPhyAddrLen;
    ttUser8Bit      ntDevStatus;
    ttNtAddrEntry   ntDevAddrConfArray[TM_NT_MAX_IP_PER_IF];
} ttNtDevEntry;
typedef ttNtDevEntry TM_FAR * ttNtDevEntryPtr;


/* uinon of the netstat entries */
typedef union tuNtEntryU
{
    ttNtRteEntry        ntRteEntry;
    ttNtArpEntry        ntArpEntry;
    ttNtUdpEntry        ntUdpEntry;
    ttNtTcpEntry        ntTcpEntry;
    ttNtDevEntry        ntDevEntry;
} ttNtEntryU;
typedef ttNtEntryU TM_FAR * ttNtEntryUPtr;

/* interface for the callback function that handles an entry */
typedef int (* ttNtEntryCBFuncPtr)(
    ttNtEntryUPtr       ntEntryPtr,
    ttUserGenericUnion  genParam1,
    ttUserGenericUnion  genParam2);

/* main entry funtion to get the entries in a netstat table */
TM_PROTO_EXTERN int tfNetStat(
    ttNtTableId         tableId,
    ttNtEntryCBFuncPtr  ntEntryCBFuncPtr,
    ttUserGenericUnion  genParam1,
    ttUserGenericUnion  genParam2);

/* Get the ARP table header as a string. */
TM_PROTO_EXTERN char TM_FAR * tfNtGetRteHeaderStr(
    char TM_FAR *   buffer,
    int  TM_FAR *   sizePtr);

/* Get the routing table header as a string. */
TM_PROTO_EXTERN char TM_FAR * tfNtGetArpHeaderStr(
    char TM_FAR *   buffer,
    int  TM_FAR *   sizePtr);

/* Get the TCP socket table header as a string. */
TM_PROTO_EXTERN char TM_FAR * tfNtGetTcpHeaderStr(
    char TM_FAR *   buffer,
    int  TM_FAR *   sizePtr);

/* Get the UDP socket table header as a string. */
TM_PROTO_EXTERN char TM_FAR * tfNtGetUdpHeaderStr(
    char TM_FAR *   buffer,
    int  TM_FAR *   sizePtr);


TM_PROTO_EXTERN char TM_FAR * tfNtGetDeviceHeaderStr(
    char TM_FAR *   buffer,
    int  TM_FAR *   sizePtr);

TM_PROTO_EXTERN char TM_FAR * tfNtGetAddrConfHeaderStr(
    char TM_FAR *   buffer,
    int  TM_FAR *   sizePtr);

/* print the ARP entry as s string to the user provided buffer*/
TM_PROTO_EXTERN char TM_FAR * tfNtArpEntryToStr(
    ttNtArpEntryPtr ntArpEntryPtr,
    char TM_FAR *   buffer,
    int  TM_FAR *   sizePtr);

/* print the route entry as s string to the user provided buffer*/
TM_PROTO_EXTERN char TM_FAR * tfNtRteEntryToStr(
    ttNtRteEntryPtr ntRteEntryPtr,
    char TM_FAR *   buffer,
    int  TM_FAR *   sizePtr);

/* print the TCP entry as s string to the user provided buffer*/
TM_PROTO_EXTERN char TM_FAR * tfNtTcpEntryToStr(
    ttNtTcpEntryPtr ntTcpEntryPtr,
    char TM_FAR *   buffer,
    int  TM_FAR *   sizePtr);

/* print the UDP entry as s string to the user provided buffer*/
TM_PROTO_EXTERN char TM_FAR * tfNtUdpEntryToStr(
    ttNtUdpEntryPtr ntUdpEntryPtr,
    char TM_FAR *   buffer,
    int  TM_FAR *   sizePtr);



/* print the device entry as s string to the user provided buffer*/
TM_PROTO_EXTERN char TM_FAR * tfNtDeviceEntryToStr(
    ttNtDevEntryPtr ntDevEntryPtr,
    char TM_FAR *   buffer,
    int  TM_FAR *   sizePtr);


/* print the IPv4 addr conf entry as s string to the user provided buffer */
TM_PROTO_EXTERN char TM_FAR * tfNtAddrConfEntryToStr(
    ttNtAddrEntryPtr ntAddrEntryPtr,
    char TM_FAR *    buffer,
    int  TM_FAR *    sizePtr);

#endif /* TM_USE_NETSTAT */

/* size in bytes of mobile cookies in the HoTI and CoTI messages */
#define TM_6_MIP_COOKIE_LEN     8
/* size in bytes of the keygen tokens in the HoT and CoT messages */
#define TM_6_MIP_KEYGEN_LEN     8



/* WIN32 NIC media type */
typedef enum _NDIS_MEDIUM_TYPE {
    NdisMedium802_3,
    NdisMedium802_5,
    NdisMediumFddi,
    NdisMediumWan,
    NdisMediumLocalTalk,
    NdisMediumDix,            /* defined for convenience, not a real medium */
    NdisMediumArcnetRaw,
    NdisMediumArcnet878_2,
    NdisMediumAtm,
    NdisMediumWirelessWan,
    NdisMediumIrda,
    NdisMediumMax             /* Not a real medium, defined as an upper-bound */
} NDIS_MEDIUM_TYPE;

/* WIN32 NIC Hardware status codes (OID_GEN_HARDWARE_STATUS). */
typedef enum _NDIS_HW_STATUS {
    NdisHardwareStatusReady,
    NdisHardwareStatusInitializing,
    NdisHardwareStatusReset,
    NdisHardwareStatusClosing,
    NdisHardwareStatusNotReady
} NDIS_HW_STATUS;


/* descriptive information of an Win32 NIC, for testing and demo */
#define TM_WIN32_NIC_DRIVE_DESC_LEN     256
#define TM_WIN32_NIC_NAME_LEN           256
typedef struct tsWin32NicInfo
{
    char                name[TM_WIN32_NIC_NAME_LEN * 2]; /* WCHAR */
    char                driverDesc[TM_WIN32_NIC_DRIVE_DESC_LEN];
    int                 nameLen;
    NDIS_HW_STATUS      status;
    NDIS_MEDIUM_TYPE    type;
    int                 speed;  /* bps */
    char                phyAddr[tm_align(TM_ETHERNET_PHY_ADDR_LEN)];
    int                 ipAddr; /* Windows assigned IP address */
    int                 netMask;/* Windows assigned netmask */
} ttWin32NicInfo;

/* Get the information of a Win32 NIC by index */
TM_PROTO_EXTERN int tfWin32GetNicInfo(ttWin32NicInfo TM_FAR * infoPtr, int index);

/* Get the number of NICs */
TM_PROTO_EXTERN int tfWin32GetNicCount(void);

/* Check if the Ethernet link is connected */
TM_PROTO_EXTERN int tfWin32GetConnectStatus(ttUserInterface interfaceHandle);


/*
 * The specified protocol should be used when authenticating a peer.  Must be
 * one of:TM_EAP_IDENTITY or TM_EAP_MD5_CHALLENGE
 */
#define TM_EAP_USE_PROTO         1

/*
 * Sets the maximum length of an EAP session in seconds.  If the session has
 * not completed in this time, authentication has failed.
 */
#define TM_EAP_SESSION_TIMEOUT   2

/* Sets the Identity Request retransmission time in seconds. */
#define TM_EAP_IDENT_TIMEOUT     3

/*
 * Sets the maximum number of Identity Requests that will be sent without a
 * response.
 */
#define TM_EAP_IDENT_RETRIES     4

/* Identity of local peer. */
#define TM_EAP_IDENT_STR         5

/* Sets the MD5-Challenge retransmission time in seconds. */
#define TM_EAP_MD5_TIMEOUT       6

/*
 * Sets the maximum number of MD5-Challenge Requests that will be sent without
 * a response.
 */
#define TM_EAP_MD5_RETRIES       7

/* Sets the MD5-Challenge username to be used when authenticating to a peer. */
#define TM_EAP_MD5_USERNAME      8

/* Sets the MD5-Challenge password to be used when authenticating to a peer. */
#define TM_EAP_MD5_PASSWORD      9

/* Sets the MD5-Challenge name to be used when acting as an authenticator. */
#define TM_EAP_MD5_AUTHNAME      10

/*
 * The specified EAP authentication protocol is required - the authenticator
 * must use this protocol when authenticating the local host.  Must be one
 * of:TM_EAP_IDENTITYTM_EAP_MD5_CHALLENGE
 */
#define TM_EAP_REQ_PROTO         11

#define TM_EAP_IDENTITY       1
#define TM_EAP_MD5_CHALLENGE  4






#ifdef __cplusplus
}
#endif

#endif /* _TRSOCKET_H_ */

/* Processed by Treck Shipping Tool v2.37 
 * 2004/11/17 12:28:54      For: Marvell
 * File Name: trsocket.h    File Revision: 4.1 
 * Project Name: tcpip.pj   Project Release: 4.1.2.1
 * Input checksum: 35936    Output checksum: 42895
 */
