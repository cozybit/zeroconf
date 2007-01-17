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
 * Description: Global Definitions for trinit.c.
 * Filename: trglobal.h
 * Author: Odile & Paul
 * Date Created:
 * $Source: include/trglobal.h $
 *
 * Modification History
 * $Revision: 4.1 $
 * $Date: 2004/11/07 21:05:13PST $
 * $Author: xwang $
 * $ProjectName: /home/mks/proj/tcpip.pj $
 * $ProjectRevision: 4.1.2.1 $
 */

#ifndef _TRGLOBAL_H_

#define _TRGLOBAL_H_

#ifdef __cplusplus
extern "C" {
#endif


/* 
 * All global variables are either in ttGlobal (true global), or in 
 * ttContext (per context variables). If user does not use multiple context
 * context variables are true global variables, but are still kept in separate
 * ttContext structure.
 */
typedef struct tsGlobal
{
#if ( (defined(TM_LOCK_NEEDED) || defined(TM_PEND_POST_NEEDED)) )
/* Counting semaphores recycle list */
    ttCountSemPtr                       tvCountSemListPtr;
/* Pre-allocated block of counting semaphores */
    ttCountSemPtr                       tvCountSemAllocPtr;
/* used counting semaphores in current block allocation */
    int                                 tvCountSemUsedCount; 
#endif  /* ( (defined(TM_LOCK_NEEDED) || defined(TM_PEND_POST_NEEDED))  */

#ifdef TM_PEND_POST_NEEDED
/* Select pend entry recycle list */
    ttList                              tvSelPendEntryHead;
#ifdef TM_LOCK_NEEDED
/* Lock for select pend entry recycle list */
    ttLockEntry                         tvSelPendEntryLock;
#endif /* TM_LOCK_NEEDED */
#endif /* TM_PEND_POST_NEEDED */

#ifdef TM_ERROR_CHECKING
/* Temporary buffer used by tm_assert() to build the error message */
    char                                tvAssertTempBuf[256];
#endif  /* TM_ERROR_CHECKING */
/* Low Water Mark for notifying device send completes. Accessed in ISR. */
    tt32Bit                             tvNotifySendCompLowWater;


#ifdef TM_USE_SHEAP
/* Simple heap variables */
    ttRawMemoryPtr                      tvSheapFreeListPtr;
    ttRawMemoryPtr                      tvSheapFragmentListPtr;
    tt32Bit                             tvSheapUsedLongs;
#ifdef TM_LOCK_NEEDED
    ttLockEntry                         tvSheapLock;
/* Simple heap counting semaphore for lock/unlock */
    ttCountSem                          tvSheapCountSem; 
/* Simple heap counting semaphore for lock/unlock has been created */
    tt8Bit                              tvSheapCountSemCreated; 
#endif /* TM_LOCK_NEEDED */
#endif /* TM_USE_SHEAP */

#if (defined(TM_USER_PACKET_DATA_ALLOC) && defined(TM_LOCK_NEEDED))
/* enable packet data buffers to be allocated in on-chip RAM */
    ttLockEntry                         tvPacketDataAllocLock;
#endif /* TM_USER_PACKET_DATA_ALLOC and TM_LOCK_NEEDED */

#ifdef TM_ERROR_CHECKING
    tt32Bit                             tvGetRawCount;
    tt32Bit                             tvFreeRawCount;
#ifdef TM_USE_ANSI_LINE_FILE
#if (defined(TM_KERNEL_VISUAL_X86) || defined(TM_KERNEL_WIN32_X86) \
     || defined(TM_MEMORY_LOGGING))
#if (defined(TM_KERNEL_VISUAL_X86) || defined(TM_KERNEL_WIN32_X86))
    FILE *                              tvMemoryLeakDetector;
#endif /* defined(TM_KERNEL_VISUAL_X86) || defined(TM_KERNEL_WIN32_X86) */
#ifdef TM_LOCK_NEEDED
    ttLockEntry                         tvMemoryLeakLock;
#endif /* TM_LOCK_NEEDED */
#endif /* Win32 or TM_MEMORY_LOGGING */
#endif /*  TM_USE_ANSI_LINE_FILE */
#endif /* TM_ERROR_CHECKING */
/*
 * Only used for dynamic memory management.
 */
#ifdef TM_USE_DYNAMIC_MEMORY
/*
 * Raw Memory Queues
 */
    ttRawMemoryPtr                      tvBufQxListPtr;
    ttRawMemoryPtr                      tvBufQ0ListPtr;
    ttRawMemoryPtr                      tvBufQ1ListPtr;
    ttRawMemoryPtr                      tvBufQ2ListPtr;
    ttRawMemoryPtr                      tvBufQ3ListPtr;
    ttRawMemoryPtr                      tvBufQ4ListPtr;
    ttRawMemoryPtr                      tvBufQ5ListPtr;
    ttRawMemoryPtr                      tvBufQ6ListPtr;

/* 
 * Recycled buffer lists.
 */
/* Queue of recycled timers */
    ttRcylPtr                           tvTimListPtr;
/* List of recycled packet buffers */
    ttRcylPtr                           tvPakListPtr;
/* Queue of recycled routing entries / ARP entries */
    ttRcylPtr                           tvRteListPtr;
/* Queue of recycled UDP/ICMP sockets */
    ttRcylPtr                           tvSocListPtr;
/* Queue of recycled TCP sockets/state vectors combination */
    ttRcylPtr                           tvTcpListPtr;


#ifdef TM_LOCK_NEEDED
/*
 * Dynamic Memory Queue Lock
 */
    ttLockEntry                         tvDynMemLockEntry;
#endif /* TM_LOCK_NEEDED */
/* Amount of memory in use by the stack (not recycled, not freed) */
    tt32Bit                             tvMemRecycledBytes;
#endif /* TM_USE_DYNAMIC_MEMORY */

/* RAM file system: RAM drive pointer */
    ttVoidPtr                           tvRamfsPtr;

#ifdef TM_MULTIPLE_CONTEXT
/*
 * The current network ID used for multiple context interface driver
 */
    ttUser32Bit                         tvCurrNetworkId;
#endif /* TM_MULTIPLE_CONTEXT */
#ifdef TM_USE_DOS_FS
/* Dos Root */
    char                       TM_FAR * tvRootDirPtr;
#endif /* TM_USE_DOS_FS */
/*
 * Ethernet global variables
 */
    ttEnetAddressUnion                  tvEtherBroadcast;
} ttGlobal;

typedef ttGlobal TM_FAR * ttGlobalPtr;


/*
 * Per context variables.
 */

typedef struct tsContext
{



#ifdef TM_USE_NETSTAT
/* this structure is 152 bytes, too big to be declared in the statck */
    ttNtEntryU          tvNtEntryU;
#endif /* TM_USE_NETSTAT */





/* when multiple PPP authentication method is used, such as CHAP, 
 * PAP and MSCHAP, we need assign priority value to each method, 
 * so that the authenticator can control the proposal order according
 * to the priority.
 * If MS-CHAP is not used, we always prefer CHAP to PAP
 */
    tt32Bit             tvPppAuthPriority;

#ifdef TM_SNMP_MIB
    ttVoidPtr           tvSnmpdPtr;
#endif /* TM_SNMP_MIB */

#ifdef TM_SNMP_CACHE
    ttSnmpdCacheHead    tvSnmpdCaches[TM_SNMPD_CACHES];
#endif /* TM_SNMP_CACHE */


#if (defined(TM_DEBUG_LOGGING) || defined(TM_TRACE_LOGGING) || \
     defined(TM_ERROR_LOGGING) || defined(TM_MEMORY_LOGGING))
/* support for debug, error and trace logging to circular buffer */
    ttLogCtrlBlkPtr     tvLogCtrlBlkPtr;
#endif

/* lookup table used to optimize storage of device pointer so that it only
   requires a 16-bit SNMP device index (1-based). */
    ttDeviceEntryPtr    tvDevIndexToDevPtr[TM_MAX_NUM_IFS + 1];

#if (defined(TM_OPTIMIZE_MANY_MHOMES) && !defined(TM_SINGLE_INTERFACE_HOME))
/* sort IP addresses to improve performance of IP address lookup in the
   receive-path when many IP aliases are configured on a single interface.
   The SNMP Agent can use this to speed up non-exact match processing
   in tfIpAddrTableEntryGet. */

    ttSortedIpAddrCache tv4SortedIpAddrCache;

#endif /* TM_OPTIMIZE_MANY_MHOMES and not TM_SINGLE_INTERFACE_HOME */

/* 
 * Globals Initialized Flag
 */
    tt32Bit               tvContextGlobalsSet;
/*
 * Timer variables.
 */
/* Control block for managing timer queues */
    ttTimerCtrl           tvTimerCtrl;

/*
 * ARP Resolve Queue
 */
    ttPacketPtr           tvArpResolveQueuePtr;
#ifdef TM_LOCK_NEEDED
    ttLockEntry           tvArpResolveQLock;
    ttLockEntry           tvArpResolveLockEntry;
#endif /* TM_LOCK_NEEDED */

/*
 * ARP resolution
 */

    tt32Bit               tvArpRequestTries;
    tt32Bit               tvArpRetryTime;
    tt32Bit               tvArpQuietTime;


/* 
 * ARP cache
 */

/* ARP entry timeout in milliseconds (600,000) */
    tt32Bit               tvArpTimeout;
/* ARP entries timer resolution in milliseconds (60,000) */
    tt32Bit               tvArpResolution;
/* Maximum number of ARP cache entries */
    int                   tvArpMaxEntries;
/* No socket errorCode */
    int                   tvErrorCode;
/*
 * Proxy ARP table
 */
/* Pointer to first element of proxy Arp table */
    ttProxyEntryPtr       tvProxyHeadPtr;
/* Proxy Lock */
    ttLockEntry           tvProxyLockEntry;


/* array of socket pointers */
    ttSocketEntryPtrPtr   tvSocketArray;
    tt16BitPtr            tvSocketErrorArray;
/* Maximum number of sockets */
    tt32Bit               tvMaxNumberSockets;
/* Number of open sockets */
    tt32Bit               tvNumberOpenSockets;
/* Maximum number of allocated TCP sockets */
    tt32Bit               tvMaxTcpVectAllocCount;
/* Number of allocated TCP sockets */
    tt32Bit               tvTcpVectAllocCount;
/* Last slot used in the socket array */
    tt32Bit               tvSocketSearchIndex;
#ifdef TM_LOCK_NEEDED
/* Socket array lock */
    ttLockEntry           tvSocketArrayLock;
#endif /* TM_LOCK_NEEDED */

/* The root of the user socket table (red-black tree, or linked list) */
    ttSocketEntryPtr      tvSocketTableRootPtr;
#ifdef TM_LOCK_NEEDED
/* Note: this one lock actually locks ALL of the red-black trees,
   connection-oriented and connectionless, for this context. */
    ttLockEntry           tvSocketRootLock;
/* To lock the number of open sockets */
    ttLockEntry           tvSocketOpenLock;
#endif /* TM_LOCK_NEEDED */

/* 
 * Default Socket Queue Sizes
 */
    tt32Bit               tvDefRecvQueueBytes; /* receive queue */
    tt32Bit               tvDefSendQueueBytes; /* send queue */
    tt16Bit               tvDefRecvQueueDgrams; /* receive queue - datagrams */
    tt16Bit               tvDefSendQueueDgrams; /* send queue - datagrams */
/* send OOB data queue */
    tt32Bit               tvDefSendOobQueueBytes;

/* 
 * IP global variables
 */
 
#ifdef TM_IP_REASSEMBLY
/* Ip collection (reassembly) */
/* default fragmentation timer resolution is 1 second */
    tt32Bit               tvFragResolution; /* in milliseconds */
/* Maximum size in bytes of an IP datagram waiting to be reassembled */
    tt32Bit               tvFragEntryMaxSize;
#endif /* TM_IP_REASSEMBLY */

    tt4IpAddress          tvRouterId; /* Router Id */

    ttUserIpForwCBFuncPtr tvIpForwCBFuncPtr;

/* Single routing cache entry for forwarding IP packets */
    ttRtCacheEntry        tvRtIpForwCache;
/*
 * single routing cache entry for sending IP packets (used by non socket
 * IP Send's)
 */
    ttRtCacheEntry        tvRtIpSendCache;
/* Lock entry to access last sent packet routing cache */
    ttLockEntry           tvRtIpSendCacheLock;

#ifdef TM_SINGLE_INTERFACE_HOME
/* Lock entry to access last forwarded packet routing cache */
#ifdef TM_LOCK_NEEDED
    ttLockEntry           tvRtIpForwCacheLock;
#endif /* TM_LOCK_NEEDED */
#else /* !TM_SINGLE_INTERFACE_HOME */
/*
 * Needed also when TM_LOCK_NEEDED not defined, because it is passed as
 * a parameter to tfIpSendPacket() inside tfIpForwardPacket().
 */
    ttLockEntry           tvRtIpForwCacheLock;
#endif /* TM_SINGLE_INTERFACE_HOME */

#ifdef TM_IP_REASSEMBLY
/*
 * IP Reassembly global variables.
 */
/* queue of fragment heads */
    ttIpFHEntryPtr        tvIpFHPtr;
#ifdef TM_LOCK_NEEDED
/*
 * lock for the fragment heads list, and recycle queue of fragment heads
 * Use only one lock for the 2 lists, since the recycle list is always
 * accessed when the fragment head list is accessed.
 */
    ttLockEntry           tvIpFHLock;
#endif /* TM_LOCK_NEEDED */
/* Aging timer for fragments */
    ttTimerPtr            tvIpFHTimerPtr;
#endif /* TM_IP_REASSEMBLY */
 
/* 
 * Pointer to IP decapsulation routine.  Initialized to null by 
 * tfGlobalVarsInit and set by tfUseIpTunneling
 */
    ttIpHandlerFuncPtr    tvIpDecapFuncPtr;

/*
 * BOOTP/DHCP global variables
 */
/* BOOTP/DHCP client socket */
    int                   tvBootSocketDescriptor;
/* BOOTP/DHCP relay agent socket */
    int                   tvBootRelayAgentDescriptor;
/* BOOTP/DHCP Relay variables */
    ttDeviceEntryPtr      tvBootServDevEntryPtr;
    tt4IpAddress          tvBootServIpAddress;

/*
 * Maximum user DHCP entries per device (on top of the TM_MAX_IPS_PER_IF
 * automatic configuration entries.)
 * (Default 0)
 */
    int                   tvMaxUserDhcpEntries;

/*
 * Router global variables
 */
/* RIP socket */
    int                   tvRipSocketDescriptor;
/*
 * Maximum number of routing entries, not including ARP cache entries
 */
    int                   tvRtMaxEntries;
#ifdef TM_ERROR_CHECKING
/* Number of routing entries out of the tree, but not in the recycle queue */
    int                    tvRteLimboCount;
#endif /* TM_ERROR_CHECKING */
/* Number of dynamic routing entries */
    int                    tvRtEntriesCount;

/* Number of ARP entries and cloned multicast entries */
    int                    tvRtArpEntriesCount;

/* Number of IPv4 ARP entries and cloned multicast entries. */
    int                    tv4RtArpEntriesCount;
    
/* Aging Router timer resolution */
    tt32Bit               tvRtTimerResolution;
/* Default RIP/Redirect entry timeout value */
    tt32Bit               tvRtTimeout;

#ifdef TM_PMTU_DISC
/*
 * Default Path MTU host routing entry timeout when the Path MTU estimate
 * is decreased in milliseconds. (10 minutes).
 */
    tt32Bit               tvRtDecreasedMtuTimeout;
/*
 * Default Path MTU host routing entry timeout when a larger path MTU estimate
 * is attempted in milliseconds. (2 minutes).
 */
    tt32Bit               tvRtLargerMtuTimeout;
#endif /* TM_PMTU_DISC */

/*
 * Empty Routing Table:
 * Structure containing three radix root nodes in the Patricia tree.
 * The middle one is the head of the tree. The left one is the root
 * leaf with all bits 0, and the right one is the root leaf with all
 * bits 1 (invalid route entry). (The default gateway in the Patricia
 * tree will be stored as a duplicate of the root left leaf.)
 */
    ttRtRadixHeadEntry     tvRtRadixTreeHead;
/* Point to the top radix node (middle one in tvRtRadixTreeHead) */
    ttRtRadixNodePtr       tvRtTreeHeadPtr;

#ifdef TM_LOCK_NEEDED
/* To lock the tree for read or write access */
    ttLockEntry            tvRtTreeLock;
#endif  

/* Aging route timer */
    ttTimerPtr             tvRtTimerPtr;
    ttTimerPtr             tvRtArpTimerPtr;
/*
 * IGMP global variables
 */
#if (defined(TM_IGMP) && defined(TM_USE_IPV4))
/* Default IGMP V1 Router present timeout in milliseconds (400 seconds) */
    int                   tvIgmpRouterV1Timeout;
/* Default IGMP Unsolicited Report interval (10 secons) */
    int                   tvIgmpUnsolReportIntv;
#endif /* TM_IGMP and TM_USE_IPV4 */

#if (defined(TM_IGMP) || defined(TM_USE_IPV6))
/*
 * Number of Ethernet multicast entries that can be allocated per allocation.
 */
    int                   tvEnetMcastBlockEntries;
/* Maximum number of successive tvEnetMcastBlockEntries allocations */
    int                   tvEnetMcastMaxAllocations;
#endif /* TM_IGMP || TM_USE_IPV6 */
    

/*
 * Raw socket global variables
 */
    ttSocketEntryPtr      tvRawSocketPtr;
    ttLockEntry           tvRawCacheLock;

/*
 * UDP global variable
 */
    ttSocketEntryPtr      tvUdpSocketPtr;
    ttLockEntry           tvUdpCacheLock;

/*
 * TCP global variables
 */
/* TCP timer resolution in milli-seconds */
    tt32Bit               tvTcpResolution;
/* TCP delay ACK timeout in milli-seconds. Disabled is set to zero. */
    tt32Bit               tvTcpDelayAckTime;
 /* ISS increase for each elapsed second */
    tt32Bit               tvTcpIssIncr;
/* Tcp connection timeout in milli-seconds */
    tt32Bit               tvTcpConnTime;
/* Default Tcp retransmission time in milli-seconds */
    tt32Bit               tvTcpDefRtoTime;
/* min Tcp retransmission time in milli-seconds */
    tt32Bit               tvTcpMinRtoTime;
/* max Tcp retransmission time in milli-seconds */
    tt32Bit               tvTcpMaxRtoTime;
/* min Tcp zero window probe time in milli-seconds */
    tt32Bit               tvTcpMinProbeWndTime;
/* max Tcp zero window probe time in milli-seconds */
    tt32Bit               tvTcpMaxProbeWndTime;
/* Max retransmission count */
    int                   tvTcpMaxReXmitCnt;
/*
 * Amount of time to stay in FIN WAIT 2 when socket has been closed
 * (Default 10 minutes)
 */
    int                   tvTcpFinWt2Time; /* in seconds */
/* 2 * Maximum segment life time (2 * 30 seconds) (TIME WAIT time) */
    int                   tvTcp2MSLTime; /* in seconds */
/* Keep alive time (2 hours) in seconds */
    int                   tvTcpKeepAliveTime; /* in seconds */
/* Keep alive Probe Interval/Idle update time (75s) in seconds */
    int                   tvTcpIdleIntvTime; /* in seconds */
/* Keep alive Max Probe count (8) */
    int                   tvTcpKeepAliveProbeCnt;

/* One cache entry for TCP incoming packets */
    ttTcpVectPtr          tvTcpVectPtr;
#ifdef TM_LOCK_NEEDED
/* Lock for the one cache entry for TCP incoming packets */
    ttLockEntry           tvTcpCacheLock;
#endif /* TM_LOCK_NEEDED */
/*
 * One empty TCP state vector pointer (to allow usage of state functions for
 * destination with no TCP state vector)
 */
    ttTcpVectPtr          tvTcpClosedVectPtr;
/*
 * Initial sequence number (incremented by 1 every 4us) and used by
 * the TCP code to initialize the initial sequence number (corresponding
 * to the SYN) of a new connection.
 */
    ttSqn                 tvTcpIss;
/* keep track of time change */
    tt32Bit               tvTcpTimerTime;
/*
 * Device Entries
 */
    ttDeviceEntryPtr      tvDeviceList; /* The first Active Device */
#ifdef TM_LOCK_NEEDED
/* Device List Lock */
    ttLockEntry           tvDeviceListLock;
#endif /* TM_LOCK_NEEDED */
    ttDeviceEntryPtr      tvLoopbackDevPtr; /* loop back device */

    ttLinkLayerEntryPtr   tvLinkLayerListPtr;
#ifdef TM_LOCK_NEEDED
    ttLockEntry           tvLinkLayerListLock;
#endif /* TM_LOCK_NEEDED */
    ttLinkLayerEntryPtr   tvNullLinkLayerHandle; /* Null link layer */

/* Size of trailer to add to tail of outgoing packets.  Initialized to zero */
    ttPktLen              tvPakPktSendTrailerSize;



/* maximum number of timers we execute in a single call to tfTimerExecute,
   a value of 0 means there is no limit */
    int                   tvTimerMaxExecute;


#if (defined(TM_SNMP_MIB) || defined(TM_PPP_LQM))
/*
 * SNMP AGENT MIB variable shared with PPP LQM.
 */
    ttIfStatMib           tvDevIpv4Data;
#endif /* TM_SNMP_MIB || TM_PPP_LQM */
#ifdef TM_SNMP_MIB
/*
 * SNMP AGENT MIB variables
 */
    ttIpMib               tvIpData;
    ttIcmpMib             tvIcmpData;
    ttUdpMib              tvUdpData;
#ifdef TM_USE_TCP
    ttTcpMib              tvTcpData;
#endif /* TM_USE_TCP */
#endif /* TM_SNMP_MIB */


#ifdef TM_LOCK_NEEDED
/*
 * tfVSScanF() lock
 */
/* Proxy Arp lock */
    ttLockEntry           tvScanfLockEntry;
#endif /* TM_LOCK_NEEDED */


/*
 * DNS resolver variables
 */
/* List of DNS servers in order of priority (index 0 is primary, etc) */
    struct sockaddr_storage tvDnsServerList[TM_DNS_MAX_SERVERS];

/* Points to the start of the DNS cache */
    ttDnsCacheEntryPtr    tvDnsCachePtr;

    ttLockEntry           tvDnsLockEntry;
/* Socket used for all DNS resolver activity */
    int                   tvDnsSocket;
/* Number of entries current in the DNS cache */
    int                   tvDnsCacheEntries;
/* indicates blocking mode of resolver: TM_BLOCKING_ON or TM_BLOCKING_OFF */
    int                   tvDnsBlockingMode;
/* Maximum number of entries in the DNS cache.  Settable by user. */
    int                   tvDnsCacheSize;
/* Maximum number of query retransmissions to attempt */
    int                   tvDnsRetries;
/* Time (in seconds) to wait for a response to come back from the DNS server */
    int                   tvDnsTimeout;


/*
 * Variables used for random seed generator.
 */
    tt16Bit               tvVector1;
    tt16Bit               tvVector2;
/*
 * Threshold in bytes below which we try and append to previous send buffer
 * for TCP
 */
    tt16Bit               tvDefSendAppendThreshold;
/*
 * Fraction use of the buffer below which we try and append to previous
 * recv buffer for TCP
 */
    tt16Bit               tvDefRecvAppendFraction;
/*
 * Fraction use of the buffer below which we try and copy to a smaller
 * recv buffer for UDP
 */
    tt16Bit               tvDefRecvCopyFraction;

/* Default Linger time when closing a TCP socket */
    tt16Bit               tvDefLingerTime;

/* Default TCP options: window scale, selective ACK, time stamp options, plus
 * default congestion avoidance
 */
    tt16Bit               tvTcpDefOptions;

/*
 * TCP global variables
 */
    tt16Bit               tvTcpDefMss; /* Default TCP MSS */

/* 
 * ICMP global variables
 */
    tt16Bit               tvIcmpSeqno; /* for request/reply */
/*
 * SLIP global variables
 */
    tt16Bit               tvSlipMtu;
/*
 * BOOTP/DHCP variables.
 */
    tt16Bit               tvBootServMhomeIndex;

#ifdef TM_USE_DHCP_FQDN
/*
 * FQDN structure
 */
    ttFqdn                              tvFqdnStruct;
#ifdef TM_LOCK_NEEDED
/* Lock for select pend entry recycle list */
    ttLockEntry                         tvFqdnStructLock;
#endif /* TM_LOCK_NEEDED */

#endif /* TM_USE_DHCP_FQDN */

/*
 * DNS resolver variables
 */
/* Last request ID to be sent. */
    tt16Bit               tvDnsRequestId;

/* Default socket options */
    tt16Bit               tvSocDefOptions;


#ifdef TM_IP_FRAGMENT
/* Fragment bigger than MTU packets */
    tt8Bit                tvIpFragment;  /* default 0 */
#endif /* TM_IP_FRAGMENT */
#ifdef TM_IP_REASSEMBLY
/* Ip collection (reassembly) */
/* Maximum allowed number of IP datagrams waiting to be reassembled. */
    tt8Bit                tvFragMaxQSize;
/* Actual number of IP datagrams waiting to be reassembled */
    tt8Bit                tvFragQSize;
#endif /* TM_IP_REASSEMBLY */
#if defined(TM_IP_REASSEMBLY) || defined(TM_SNMP_VERSION)
/* IP Reassembly timeout in seconds. Default is 60 */
    tt8Bit                tvFragTtl;
#endif /* TM_IP_REASSEMBLY || TM_SNMP_VERSION */ 

/*
 * ARP cache
 */
/* Whether to store gratuitious ARP cache mappings */
    tt8Bit                tvArpSmart;
/* 
 * IP global variables
 */
/* Host(do not forward) or Router (forward) */
    tt8Bit                tvIpForward; /* 0 means host, 1 router */
/* Forward directed broadcast IP packets (default is 1) */
    tt8Bit                tvIpDbcastForward;
#ifdef TM_IP_MCAST_FORWARD
/* 1 means multicast router */
    tt8Bit                tvIpMcastForward;
#endif /* TM_IP_MCAST_FORWARD */

/* default IP time to live */
    tt8Bit                tvIpDefTtl;
/* Default IP TOS */
    tt8Bit                tvIpDefTos;

/*
 * RIP variables
 */
/* Rip enabled/disabled */
    tt8Bit                tvRipOn;
/*
 * tvRipSendMode has 4 possible values that determine the RIP packets being
 * sent: TM_RIP_NONE (no RIP packets are being sent),
 * TM_RIP_1 (only RIP 1 packets are being sent), TM_RIP_1_COMPAT
 * (default, sending only RIP 2 packets, broadcasting them ),
 * TM_RIP_2 (sending only RIP 2 packets, multicasting them)
 */
    tt8Bit                tvRipSendMode;
/*
 * tvRipRecvMode has 4 possible values that determine the RIP packets being
 * received: TM_RIP_NONE (no RIP packets are being received),
 * TM_RIP_1 (only RIP 1 packets are being received),
 * TM_RIP_2 (only RIP 2 packets are being received),
 * TM_RIP_1|TM_RIP_2 (both RIP 1 and Rip 2 packets are being received)
 */
    tt8Bit                tvRipRecvMode;
/* 
 * ICMP global variables
 */
    tt8Bit                tvIcmpAddrMaskAgent;
/*
 * UDP global variable
 */
/* Enable/disable Udp cheksum. Default is 1 (enabled). */
    tt8Bit                tvUdpChecksumOn;
/* Enable/disable Incoming check on Udp cheksum. Default is 1 (enabled). */
    tt8Bit                tvUdpIncomingChecksumOn;
/* Enable/disable Incoming check on TCP cheksum. Default is 1 (enabled). */
    tt8Bit                tvTcpIncomingChecksumOn;

/*
 * Device entries
 */
/*
 * Number of added interfaces. Adding interfaces increments this value.
 * Initialized to zero. Does not include Loop back interface.
 */
    tt8Bit                tvAddedInterfaces;

/*
 * Number of configured interfaces. Configuring interfaces increments
 * this value, unconfiguring interfaces decrements it. Initialized to zero.
 * Does not include Loop back interface.
 */
    tt8Bit                tvActiveInterfaces;

} ttContext;

typedef ttContext TM_FAR * ttContextPtr;

/*
 * Global variables for all contexts.
 */
/* Current context */
#ifdef TM_MULTIPLE_CONTEXT
TM_EXTERN ttContextPtr          TM_GLOBAL_QLF tvCurrentContext;
#else /* !TM_MULTIPLE_CONTEXT */
TM_EXTERN ttContext             TM_GLOBAL_QLF tvCurrentContextStruct;
#endif /* !TM_MULTIPLE_CONTEXT */

/* Structure containing global variables, other than per context */
TM_EXTERN ttGlobal              TM_GLOBAL_QLF tvGlobals;

/* Pointer to global variables */
TM_EXTERN ttGlobalPtr           TM_GLOBAL_QLF tvGlobalsPtr;

/*
 * (non context) Global variables set or not. Cannot be in ttGlobal, 
 * because it is checked prior to tvGlobalsPtr initialization.
 */
TM_EXTERN tt32Bit               TM_GLOBAL_QLF tvGlobalsSet;

/*
 * Timer global variables
 */
/* 
 * tvTime not declared in ttGlobal structure to save value across reboots 
 * and because can be changed from Timer ISR.
 */
TM_EXTERN tt32Bit               TM_GLOBAL_QLF tvTime;
/* tvTimerTickLength used during timer ISR */
TM_EXTERN unsigned              TM_GLOBAL_QLF tvTimerTickLength;

#ifdef TM_DEMO_TIME_LIMIT
TM_EXTERN tt32Bit               TM_GLOBAL_QLF tvDemoElapsedSec;
TM_EXTERN tt32Bit               TM_GLOBAL_QLF tvDemoElapsedMsec;
#endif /* TM_DEMO_TIME_LIMIT */

#ifdef TM_USE_KERNEL_CRITICAL_STATUS
/* Not in ttGlobal because just used to store/restore critical status */
TM_EXTERN ttUserKernelCriticalStatus tvKernelCriticalStatus;
#endif /* TM_USE_KERNEL_CRITICAL_STATUS */

/*
 * IP header Id incremented every time an IP datagram is sent. Not in
 * ttGlobal, because we want to preserve value across reboots
 */

TM_EXTERN tt16Bit               TM_GLOBAL_QLF tvIpId;
/*
 * Pend/Post global variable. Not in ttGlobal, because can be initialized
 * in tfKernelInitialize().
 */
TM_EXTERN tt8Bit                TM_GLOBAL_QLF tvPendPostEnabled;

#ifdef TM_LOCK_NEEDED
/*
 * lock global variable.
 */
TM_EXTERN tt8Bit                TM_GLOBAL_QLF tvLockEnabled;
#endif /* TM_LOCK_NEEDED */


/* Default file system working direectory has been set */
TM_EXTERN tt8Bit                TM_GLOBAL_QLF tvFSInitWorkingDirSet;
/* Initial File system drive */
TM_EXTERN int                   TM_GLOBAL_QLF tvFSInitDrive;


#ifdef TM_USE_SHEAP
/* 
 * tvSheapPageSize not declared in ttGlobal structure because could
 * be used before tvGlobalsPtr initialization. 
 */
TM_EXTERN tt32Bit               TM_GLOBAL_QLF tvSheapPageSize;
#endif /* TM_USE_SHEAP */

/*
 * Lookup table used to convert prefix length to netmask. Not declared
 * in ttGlobal, because pointer to constant table.
 */
TM_EXTERN tt4ConstIpAddressPtr  TM_GLOBAL_QLF tvRt32ContiguousBitsPtr;

/*
 * Device driver ISR parameters global variable. Not in ttGlobal because
 * accessed from device driver ISR.
 */
TM_EXTERN ttDeviceIsrPara       TM_GLOBAL_QLF tvDeviceIsrPara[
                                                    TM_MAX_NUMBER_DEVICES];

/*
 * Win32 Thread Local Storage index, needed for each thread to store
 * and retriece it's context pointer. Not in ttGlobal, because win32 variable.
 */
#if ( defined(TM_KERNEL_WIN32_X86) && defined(TM_MULTIPLE_CONTEXT) )
TM_EXTERN tt32Bit               TM_GLOBAL_QLF tvTlsIndex;
#endif /* defined(TM_KERNEL_WIN32_X86) && defined(TM_MULTIPLE_CONTEXT) */

/*
 * PPP global variables. Kludge to return a non null pointer to the user.
 * Will always have zero in it. Not declared in ttGlobal, because pointer
 * to a constant.
 */
TM_EXTERN  const char  TM_FAR * TM_GLOBAL_QLF tvChapEmptyString;



#ifdef __cplusplus
}
#endif

#endif /* _TRGLOBAL_H_ */

/* Processed by Treck Shipping Tool v2.37 
 * 2004/11/17 12:28:53      For: Marvell
 * File Name: trglobal.h    File Revision: 4.1 
 * Project Name: tcpip.pj   Project Release: 4.1.2.1
 * Input checksum: 49764    Output checksum: 18468
 */
