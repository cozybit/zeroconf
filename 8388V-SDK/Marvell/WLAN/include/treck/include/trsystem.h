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
 * Description:     System Description Macro's
 * This file is intended for the user to modify for their hardware/os platform
 * Filename:        trsystem.h
 * Author:          Odile & Paul
 * Date Created:    9/22/97
 * $Source: include/trsystem.h $
 *
 * Modification History
 * $Revision: 4.1 $
 * $Date: 2004/11/07 21:05:23PST $
 * $Author: xwang $
 * $ProjectName: /home/mks/proj/tcpip.pj $
 * $ProjectRevision: 4.1.2.1 $
 */

#ifndef _TRSYSTEM_H_

#define _TRSYSTEM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* comment out the following line if you do not want to use IPv4 protocols */
#define TM_USE_IPV4

/* uncomment the following line if you want to use IPv6 protocols */
/* #define TM_USE_IPV6 */

/* comment out the following line if you do not want to use PPP protocols */
// #define TM_USE_PPP
    

/*
 * Uncomment the following line, if you want to use the netstat tool for
 * for outputting the routing entries, ARP entries and socket entries
 */
/* #define TM_USE_NETSTAT */

/*
 * Uncomment the following line to enable support for splitting out IPv4
 * and IPv6 traffic in the send path to separate virtual channels to
 * simulate multiple hosts as multiple IP aliases. Note that this is
 * incompatible with the default mode of IPv6 operation, therefore IPv6
 * prefix discovery must be disabled.
 */
/* #define TM_USE_VCHAN */

/*
 * Uncomment the following line to speed up lookup in the recieve-path of IP
 * aliases configured on the interface. This is intended for situations where
 * the user has configured many (i.e. >50) IP aliases on a single interface. 
 */
/* #define TM_OPTIMIZE_MANY_MHOMES */

/*
 * Uncomment the following line for SNMPv1 only.
 * (SNMP option only.)
 */
/* #define TM_SNMP_VERSION 1 */

/*
 * Uncomment the following line for SNMPv1 and SNMPv2c (bilingual).
 * (SNMP option only.)
 */
/* #define TM_SNMP_VERSION 2 */

/*
 * Uncomment the following line for SNMPv1, SNMPv2c and SNMPv3 (trilingual).
 * (SNMP option only.)
 */
/* #define TM_SNMP_VERSION 3 */

/*
 * Uncomment the TM_SNMP_CACHE macro if you want the stack to keep a cache of
 * the UDP sockets, TCP vectors, ARP table, and Routing table for SNMP.
 * (SNMP option only.)
 */
/* #define TM_SNMP_CACHE */

/*
 * Comment out TM_USE_RFC1213_MIB2 if you do not want the deprecated RFC1213
 * MIB2 objects that are designed only for IPv4 to be included in the SNMP
 * agent.
 * This option requires to enable TM_USE_IPV4 and TM_USE_SNMP_CACHE to be fully
 * functional). If you are running the stack in dual mode (IPV4 and IPV6), V6
 * objects will not be displayed in those table.
 * (SNMP option only.)
 */
//#define TM_USE_RFC1213_MIB2

/*
 * Comment out TM_USE_NG_MIB2 to disable the Next Generation MIB2, which
 * consists of tables that support both IPv4 and IPv6 entities.
 * This option requires to enable TM_SNMP_CACHE to be fully functional.
 * (SNMP option only.)
 */
//#define TM_USE_NG_MIB2

/*
 * Uncomment the following line for 8-bit OIDs.
 * (SNMP option only.)
 */
/* #define TM_SNMP_8BIT_SUBIDS */

/*
 * Uncomment the following line 16-bit OIDs.
 * (SNMP option only.)
 */	
/* #define TM_SNMP_16BIT_SUBIDS */

/*
 * TM_IP_FRAGMENT macro. Enable support for IPv4 fragmentation.
 * Comment out to save code space when IPv4 fragmentation is not needed.
 */
#define TM_IP_FRAGMENT

/*
 * TM_IP_FRAGMENT macro. Enable support for IPv4 reassembly.
 * Comment out to save code space when IPv4 reassembly is not needed.
 */
#define TM_IP_REASSEMBLY

/* 
 * This macro is commented out by default.
 * If you define TM_IP_FRAGMENT, and your device driver supports scattered 
 * send, then define TM_IP_FRAGMENT_NO_COPY to avoid an extra internal data
 * copy when an IP datagram is fragmented.
 */
/* #define TM_IP_FRAGMENT_NO_COPY */

/*
 * Uncomment the TM_DISABLE_TELNETD macro, if you want to remove the TELNETD code.
 */
/* #define TM_DISABLE_TELNETD */

/*
 * Uncomment the TM_DISABLE_FTP macro, if you want to remove the FTP
 */
/* #define TM_DISABLE_FTP */

/*
 * Uncomment the TM_DISABLE_FTPD macro, if you want to remove the FTPD code.
 */
/* #define TM_DISABLE_FTPD */

/*
 * Uncomment the TM_DISABLE_TFTP macro, if you want to remove the TFTP
 */
/* #define TM_DISABLE_TFTP */

/*
 * Uncomment the TM_DISABLE_TFTPD macro, if you want to remove the TFTPD code.
 */
/* #define TM_DISABLE_TFTPD */

/*
 * Uncomment the TM_DISABLE_TCP macro, if you want to remove the TCP code.
 */
/* #define TM_DISABLE_TCP */

/*
 * Uncomment the TM_DISABLE_TCP_SACK macro, if you want to remove the TCP
 * Selective Acknowlegment Options (RFC2018) code.
 */
/* #define TM_DISABLE_TCP_SACK */

/*
 * Define the TM_TCP_FACK macro to enable the Forward Acknowledgement
 * algorithm. It can only be enabled if TM_DISABLE_TCP_SACK is not #define'd.
 * Enabling TM_TCP_FACK allows TCP SACK to retransmit more than one SACK'ed
 * segment (wireless TCP option only).
 */
/* #define TM_TCP_FACK */
    
/*
 * Uncomment the TM_DISABLE_TCP_RFC2414 macro, if you want to disable
 * the TCP Initial Send Window Increase as described in RFC 2414.
 */
/* #define TM_DISABLE_TCP_RFC2414 */

/*
 * Uncomment the TM_DISABLE_TCP_RFC2581 macro, if you want to remove the TCP
 * congestion control update to RFC2001 as described in RFC2581.
 */
/* #define TM_DISABLE_TCP_RFC2581 */

/*
 * Uncomment the TM_DISABLE_TCP_RFC3042 macro, if you want to remove the
 * Limited Transmit Algorithm that enhances TCP's Loss Recovery as described 
 * in RFC3042. 
 */
/* #define TM_DISABLE_TCP_RFC3042 */

/*
 * Comment out the TM_DISABLE_TCP_ACK_PUSH macro, if you want the
 * stack to send an immediate TCP ACK, when receiving a TCP segment with
 * the PUSH bit set.
 * (Important: this macro should be commented out when the peer runs
 *  windows 2000.)
 */
#define TM_DISABLE_TCP_ACK_PUSH

/*
 * Uncomment the TM_DISABLE_PMTU_DISC macro, if you want to disable automatic
 * Path MTU discovery on indirect routes.
 */
/* #define TM_DISABLE_PMTU_DISC */

/*
 * Uncomment the TM_ARP_UPDATE_ON_RECV, if you want to avoid sending an ARP
 * request every 10 minutes on an active ARP entry, i.e. an ARP entry for
 * a destination IP address that the stack is actively sending to.
 * Note that uncommenting out this macro will affect speed and size, since
 * every incoming packet will be checked for an ARP cache match on its
 * destination IP address.
 */
/* #define TM_ARP_UPDATE_ON_RECV */
 
/*
 * Uncomment the TM_TCP_ANVL macro, if you want the stack to strictly follow
 * the RFC recommendations for the TCP protocol, as checked by the ANVL tools.
 */
/* #define TM_TCP_ANVL */

/*
 * Uncomment the TM_USE_TCP_PACKET macro, if you want to modify the
 * TCP behaviour and force TCP to send data on user send packet boundaries.
 * Warning: uncommenting this macro, will make TCP less efficient, and
 * will also disable Path MTU discovery, and TCP Selective Acknowledgements.
 */
/* #define TM_USE_TCP_PACKET */

/*
 * Uncomment the TM_USE_REUSEADDR_LIST macro, if you want to use the
 * SO_REUSEADDR socket level option with setsockopt(). This enables use of
 * the socket reuse address list, i.e. enable binding on the same port using
 * different local IP addresses.
 */
/* #define TM_USE_REUSEADDR_LIST */

/*
 * Uncomment the TM_USE_AUTO_IP macro, if you want to use Auto configuration,
 * or want to add collision detection.
 */
/* #define TM_USE_AUTO_IP */

/*
 * Uncomment TM_USE_DHCP_FQDN to enable the DHCP FQDN option
 */
/* #define TM_USE_DHCP_FQDN */

/*
 * Uncomment the TM_USE_RAW_SOCKET macro, if you want to use raw sockets.
 * (tfRawSocket() need to be called, instead of socket())
 */
#define TM_USE_RAW_SOCKET

/*
 * Uncomment the TM_RAW_SOCKET_INPUT_ICMP_REQUESTS, if you want ICMP echo
 * requests, and ICMP address mask requests to be delivered to a raw ICMP
 * socket.
 */
#define TM_RAW_SOCKET_INPUT_ICMP_REQUESTS

/*
 * Uncomment TM_PING_REPLY_BROADCAST if you want ICMP replies to be sent
 * when the requested destination is a broadcast or a multicast address.
 */
/* #define TM_PING_REPLY_BROADCAST */

/*
 * Uncomment TM_PING_REPLY_MULTICAST if you want ICMP replies to be sent
 * when the requested destination is a multicast address.
 * (not needed if TM_PING_REPLY_BROADCAST is enabled)
 */
/* #define TM_PING_REPLY_MULTICAST */

/*
 * Uncomment the TM_IGMP macro, if you have purchased the IGMP protocol
 */
#define TM_IGMP

/* 
 * PPP options 
 */
/*
 * Uncomment the TM_PPP_LQM macro, if you want PPP Link Quality Monitoring
 */
/* #define TM_PPP_LQM */

/*
 * Uncomment the TM_USE_IPHC to enable IP header compression (RFC-2507) with
 * PPP.
 */
/* #define TM_USE_IPHC */

/*
 * Uncomment the TM_USE_EAP to enable Extensible Authentication Protocol with
 * PPP.
 */
/* #define TM_USE_EAP */

/* define the following macro to use MSCHAP */
/*#define TM_USE_PPP_MSCHAP*/

/*
 * NAT Options
 */

/* Enable NAT progress display with TM_ERROR_LOGGING, TM_DEBUG_LOGGING */
/* and/or TM_TRACE_LOGGING.  (TM_NAT_TRACE used to do this.)           */

/* To enable tfNatDump(), which can display a list of NAT triggers,    */
/* define TM_DEBUG_LOGGING in this file (TM_NAT_DUMP used to do this)  */

/* Discard packets that NAT does not transform */
#define TM_NAT_STRICT

/*
 * Uncomment the TM_USE_HTTPD to enable HTTPD (web server)
 */
/* #define TM_USE_HTTPD */

/*
 * Uncomment the TM_USE_HTTPD_CGI to enable CGI supprt of HTTPD (web server)
 */
/* #define TM_USE_HTTPD_CGI */
    
/*
 * Uncomment the TM_USE_POP3 to enable POP3
 */
/* #define TM_USE_POP3 */

/*
 * Uncomment the TM_USE_SMTP to enable SMTP
 */
/* #define TM_USE_SMTP */

/*
 * Uncomment the TM_MOBILE_IP4 macro, if you have purchased the Mobile IPv4
 * option.
 */
/* #define TM_MOBILE_IP4 */

/*
 * IPsec Option
 * Uncomment the TM_USE_IPSEC macro, if you have purchased the IPSEC option.
 */
/*#define TM_USE_IPSEC */

/*
 * IKE Option
 * Uncomment the TM_USE_IKE macro, if you are going to use KEY management rather
 * than manual keying
 */
/*#define TM_USE_IKE */

/*
 * SSL Client Option
 * Uncomment the TM_USE_SSL_CLIENT macro, if you are going to use SSL client
 * functionality
 */
/*#define TM_USE_SSL_CLIENT */

/*
 * SSL Server Option
 * Uncomment the TM_USE_SSL_SERVER macro, if you are going to use SSL server
 * functionality
 */
/*#define TM_USE_SSL_SERVER */

/*
 * SSL Debuging option
 * Uncomment the TM_SSL_DEBUG macro, if you are going to debug SSL 
 * encrypt/decrypt, MAC, and padding
 */
/*#define TM_SSL_DEBUG */

/*
 * Uncomment the TM_DEV_SEND_OFFLOAD macro, if you want to use the
 * device driver TCP segmentation/checksum offload capabilities.
 */
/* #define TM_DEV_SEND_OFFLOAD */

/*
 * Uncomment the TM_DEV_RECV_OFFLOAD macro, if you want to use the
 * device driver recv packet checksum offload capabilities.
 */
/* #define TM_DEV_RECV_OFFLOAD */

/* 
 * Uncomment the TM_USE_DRV_ONE_SCAT_SEND macro, if you want to use
 * a single call to the device driver, passing the packet handle, even
 * when sending a frame with scattered buffers. Note that to enable 
 * this feature, this macro needs to be added, and tfUseInterfaceOneScatSend
 * needs to be called on the interface that supports it.
 */
/* #define TM_USE_DRV_ONE_SCAT_SEND */

/* 
 * Uncomment the TM_USE_DRV_SCAT_RECV macro, if you want to allow
 * the device driver recv function to pass back a frame to the stack
 * in scattered buffers ("Gather Read"). Note that to enable this feature, 
 * this macro needs to be added, and  tfUseInterfaceScatRecv needs to be 
 * called on the interface that supports it.
 */
/* #define TM_USE_DRV_SCAT_RECV */

/*
 * Uncommment TM_NEED_ETHER_32BIT_ALIGNMENT and uncomment TM_USE_DRV_SCAT_RECV,
 * without using tfUseInterfaceScatRecv(),
 * if you want tfRecvInterface to make the TCP/IP header aligned on a
 * 4-byte boundary, if it is not aligned coming from a single buffer in
 * an Ethernet device driver.
 */
/* #define TM_NEED_ETHER_32BIT_ALIGNMENT */

/*
 * Uncomment the TM_LOOP_TO_DRIVER, if you want to loop back all the way
 * to the driver when sending to an Ethernet interface IP address.
 */
/* #define TM_LOOP_TO_DRIVER */

/*
 * Undefine/delete the TM_INDRV_INLINE_SEND_RECV macro to test intra 
 * machine driver with a separate recv task. In that case the intra machine 
 * received data will no longer be processed in the send path, but will be 
 * processed when tfRecvInterface/tfRecvScatInterface is called. In the
 * examples directory, the txscatlp.c, and txscatdr.c modules contain sample
 * code that uses this feature.
 */
#define TM_INDRV_INLINE_SEND_RECV

/*
 * Uncomment the TM_USE_INDRT, if you want to use the GHS INDRT debugging
 * tools . You also need to call tfUseIndrt() on the Ethernet interface
 * you want the INDRT debug messages to be sent and received on.
 */
/* #define TM_USE_INDRT */

/*
 * Uncomment the TM_MULTIPLE_CONTEXT macro, if you want to use multiple
 * instances of the Treck stack.
 */
/* #define TM_MULTIPLE_CONTEXT */

/*
 * By default the prototypes in the header files are not declared extern.
 * If your linker dictates that they should be declared extern, then
 * uncomment the following macro.
 */

/* #define TM_PROTO_EXTERN extern */

/*
 * Uncomment the TM_DISABLE_ANSI_LINE_FILE macro if your compiler does not
 * support the ANSI __LINE__ and __FILE__ macros.
 * The tm_assert() macro will use the ANSI __LINE__ and __FILE__ macros (if
 * available) to print the source line number and source file name identifying
 * where in the code an assertion failure occurred for debugging purposes.
 */
/* #define TM_DISABLE_ANSI_LINE_FILE */

/* Uncomment the TM_PC_LINT macro, if you are using PC-LINT */
/*#define TM_PC_LINT */


/*
 * Thread stop is used to stop a thread which has had an unrecoverable error
 * by default defined to be a forever loop
 *
 * Unrecoverable errors dictate that the system MUST be reset!
 * You can define this to the reset for your board
 */

/* #define tm_thread_stop rtosReset() */

/*
 * Define the TM_PING_MILLISECONDS_INTERVAL macro, if you want the
 * pingInterval parameter passed to the obsolete tfPingOpenStart API
 * to be in milliseconds rather than in seconds.
 */
/* #define TM_PING_MILLISECONDS_INTERVAL */


/*
 * Performance Macros
 */

/*
 * If we are using ethernet, we can use it inline to increase speed.
 */
/* #define TM_BYPASS_ETHER_LL */

/*
 * Optimize for speed
 */
/* #define TM_OPTIMIZE_SPEED */

/*
 * Optimize for size
 */
/* #define TM_OPTIMIZE_SIZE */

/*
 * If you have only a single interface, and you only configure a single IP
 * address on that interface (i.e. multi-homing is not required), then
 * uncomment the TM_SINGLE_INTERFACE_HOME macro to reduce code size.
 *
 * Warning: Defining this will prevent addition of the loop back device.
 * 1. Packets sent to the single interface IP address will be sent all the
 *    way to the driver, instead of being loop back by the stack.
 * 2. Packets cannot be sent to the IP loop back address, 127.0.0.1.
 */
/* #define TM_SINGLE_INTERFACE_HOME */

/*
 * If you have only a single interface, and you are using IPv6, then
 * uncomment the TM_SINGLE_INTERFACE macro to reduce code size. Note that
 * all IPv6 interfaces are multi-homed, so you cannot use the
 * TM_SINGLE_INTERFACE_HOME macro with IPv6.
 */
/* #define TM_SINGLE_INTERFACE */

/*
 * User defined checksum routines.
 * One of either TM_USER_OCS or TM_CUSTOM_OCS may be defined.
 * 1. If neither is defined, the stack will use a standard C implementation of
 *    the checksum or include a CPU-specific implementation if the user has
 *    defined an additional CPU specific macro such as TM_BORLAND_ASM_OCS,
 *    TM_GNUXSCALE_ASM_OCS, etc.
 * 2. TM_USER_OCS will cause the stack to include the file ocksum/trcsuser.c,
 *    which must be created by the user and must contain the function
 *    tfPacketChecksum().
 * 3. TM_CUSTOM_OCS will cause the stack to not compile in any checksum
 *    routine, meaning the user must link in a module containing the function
 *    tfPacketChecksum().
 *
 *    See the file trpkt.c for the standard implementation of this function.
 */
/* #define TM_USER_OCS */
/* #define TM_CUSTOM_OCS */

/* If you have an different keyword for interrupt, then #define TM_INTERRUPT
   here to be that keyword
*/
/* #define TM_INTERRUPT interrupt */

/* !! NOTE about TM_GLOBAL_QLF and TM_GLOBAL_NEAR: *
 * 
 * Only one of them can be redefined to have a value, otherwise compiler
 * errors will occur
 */

/* Default qualifier for global variables. If you would like to put all global
 * variables into one memory type (e.g. far, huge, near), then #define
 * TM_GLOBAL_QLF here to be that qualifier and leave TM_GLOBAL_NEAR undefined
 */
/* #define TM_GLOBAL_QLF */

/* Default qualifier for frequently used global variables.
 * If you would like to put the frequently used global variables into 
 * one memory type (e.g. near) and the rest of them into a different
 * memory type (e.g. far, huge), then #define the memory type for frequently
 * used global variables here as TM_GLOBAL_NEAR and leave TM_GLOBAL_QLF
 * undefined
 */
/* #define TM_GLOBAL_NEAR */


/* Default qulifier for constants. If you would like to put all constants
 * into one memory type (e.g. far, huge, near), the #define TM_CONST_QLF
 * here to be that qualifier.
 */
/* #define TM_CONST_QLF */

/*
 * Uncomment the TM_USE_SHEAP macro if your RTOS does not provide heap
 * management routines, or if you do not want the Treck stack to allocate its
 * blocks of memory from the RTOS heap. When TM_USE_SHEAP is defined, the stack
 * allocates its blocks of memory from the Treck simple heap static array.
 */
/* #define TM_USE_SHEAP */

/*
 * Simple heap size (used by simple heap allocation routines in trsheap.c)
 * This is used by tfSheapMalloc() and tfSheapFree() provided in
 * trsheap.c, and called by tfKernelMalloc() and tfKernelFree() when
 * the RTOS does not provide heap support, and you do not want to use
 * C library malloc and free.
 * This is not documented because it is an OS supplement.
 * On a DSP platform, the heap size should be specified in words; on all
 * other targets the heap size should be specified in bytes.
 */
/*#define TM_SHEAP_SIZE TM_UL(48)*TM_UL(1024) */

/*
 * Minimum block size in bytes when using the simple heap, either when
 * allocating a new block, or when refragmenting a freed block. 
 * If not defined here, default value is 8 if TM_ERROR_CHECKING is
 * defined, 4 otherwise.
 */
/* #define TM_SHEAP_MIN_BLOCK_SIZE 16 */

/*
 * Uncomment the TM_SHEAP_MARK_MEMORY macro, so that simple heap
 * initial memory is filled with 0xFE, so that allocated simple heap memory
 * is filled with 0xA0, and that, freed simple heap memory is filled with 0xFA.
 * This helps check whether memory is accessed after having been freed.
 */
#define TM_SHEAP_MARK_MEMORY

/*
 * Uncomment the TM_DYNAMIC_CREATE_SHEAP macro if you want the Treck simple
 * heap to be dynamically allocated by calling tfKernelSheapCreate rather
 * than implemented as a static array. It is usually necessary to define
 * TM_DYNAMIC_CREATE_SHEAP, if TM_SHEAP_NUM_PAGE is bigger than 1 (see below).
 */
/* #define TM_DYNAMIC_CREATE_SHEAP */

/*
 * Number of memory pages. Instead of a unique array, the simple heap can be
 * divided into several ones. Uncomment TM_SHEAP_NUM_PAGE and indicate the
 * number of pages if you are using the simple heap with a system that has
 * paged memory and that one page is too small to hold the whole simple heap
 * memory (static or preallocated by tfKernelSheapCreate).
 * In this case the simple heap will be divided in several non-contiguous
 * pages. If you do define TM_SHEAP_NUM_PAGE to a number bigger than 1,
 * then you will likely need to define TM_DYNAMIC_CREATE_SHEAP also.
 * The default for TM_SHEAP_NUM_PAGE is 1. Each page of memory must
 * have the same size. The page size is TM_SHEAP_SIZE/TM_SHEAP_NUM_PAGE.
 * If the page size is lower or equal to 4Kb (TM_BUF_Q6_SIZE), you cannot
 * use dynamic memory and must define TM_DISABLE_DYNAMIC_MEMORY.
 */
/* #define TM_SHEAP_NUM_PAGE 8 */

/*
 * Uncomment TM_DISABLE_DYNAMIC_MEMORY if you need to save ROM and RAM space.
 * Note that this will disable the Treck internal recycled buffer lists, which
 * is part of the "Treck Learn" feature. The system will require less
 * memory, but will be slower, because the system will not be able to take
 * advantage of the "Treck Learn" feature.
 */
/* #define TM_DISABLE_DYNAMIC_MEMORY */

/* Enable only one of the following to choose the file system */
/* #define TM_USE_DOS_FS */
/* #define TM_USE_RAM_FS */
/* #define TM_USE_ROM_FS */

/*
 * RTOS/Kernel Macros
 */
/* #define TM_TRECK_DEMO */
/* #define TM_AUTO_TEST */

/* #define TM_KERNEL_WIN32_X86 */
/* #define TM_KERNEL_VISUAL_X86 */
/* #define TM_KERNEL_LINUX_APP */
/* #define TM_KERNEL_ELX_86 */
/* #define TM_KERNEL_UCOS_CPU32 */
/* #define TM_KERNEL_UCOS_X86 */
/* #define TM_KERNEL_UCOS_PPC */
/* #define TM_KERNEL_UCOS_MIPS */
/* #define TM_KERNEL_AMX_CPU32 */
/* #define TM_KERNEL_AMX_X86 */
/* #define TM_KERNEL_DOS_X86 */
/* #define TM_KERNEL_THREADX_ARM7 */
/* #define TM_KERNEL_THREADX_CPU32 */
/* #define TM_KERNEL_NONE_EZ80 */
/* #define TM_KERNEL_RZK_EZ80 */
/* #define TM_KERNEL_UCOS_XSCALE */
/* #define TM_KERNEL_NONE_H8S */
/* #define TM_KERNEL_UCOS_SH2 */
/* #define TM_KERNEL_CMX_C16X */
/* #define TM_KERNEL_REALOS_FR */
/* #define TM_KERNEL_TI_DSP_BIOS */
/* #define TM_KERNEL_QUADROS */

/*
 * Do we have seperate tasks for processing receive or send complete
 */

/*
 * Received packets are processed in a seperate Receive task
 * This all depends on how we setup our device driver and if we
 * are using mode 2 or mode 4 (see below)
 */
/* #define TM_TASK_RECV */

/*
 * By default, packets are sent in the context of the application
 * sending the data, or of the timer task for retransmission of data,
 * or of the receive task for TCP acks, or PING echo replies for
 * example. If you wish to use a separate transmit task to
 * send the data to the driver, then define TM_TASK_XMIT.
 */
/* #define TM_TASK_XMIT */

/*
 * When a send complete occurs, we process it in a seperate task
 * This all depends on how we setup our device driver and if we
 * are using mode 2 or mode 4 (see below)
 */
/* #define TM_TASK_SEND */

/*
 * Device driver macros
 */

/* #define TM_QUICC */


/*
 * Compiler macros to take advantage on inline assembly (when needed)
 */

/* #define TM_COMPILER_SDS */     /* The SDS Crosscode Compiler for 68K */
/* #define TM_COMPILER_DDI_PPC */ /* The Diab Data Compiler for the PPC */
/* #define TM_COMPILER_GHS_PPC */ /* The Green Hills Compiler for the PPC */
/* #define TM_COMPILER_MRI_68K */ /* The MRI Compiler for 68K           */
/* #define TM_COMPILER_GHS_ARM */ /* The Green Hills Compiler for the ARM */
/* #define TM_COMPILER_GHS_68K */ /* The Green Hills Compiler for the 68K */
/* #define TM_COMPILER_GNU_PPC */ /* GNU GCC Compiler for the PPC */
/* #define TM_COMPILER_CW_5282 */ /* Metrowerks CodeWarrior for Motorola 5282*/

/* 
 * Define this macro, if compiler cannot handle nested braces
 * in the constant initialization of in6_addr structure 
 * (trsocket.h/trglobal.h/trinit.c)
 */
/* #define TM_IN6ADDR_NO_NESTED_BRACES */

/*--------------------------------------------------------------------------*/

/*
 * The following macros are primitives that can be turned on/off to suite
 * your design (use these if you do NOT use the RTOS macros)
 */

/*
 * Processor Macros
 */
/* #define TM_EZ80 */
/* #define TM_INTEL_X86 */
/* #define TM_INTEL_XSCALE */
/* #define TM_MOTOROLA_CPU32 */
/* #define TM_MOTOROLA_68K */
/* #define TM_MOTOROLA_PPC */
/* #define TM_TMS320_C3 */
/* #define TM_TMS320_C3_BIG_ENDIAN */ /* For TI C3 like DSP with byte order reversed */
/* #define TM_TMS320_C5 */
/* #define TM_TMS320_C6 */

/*
 * Word Order
 */

#define TM_LITTLE_ENDIAN

/* To perform internal checking for the Treck Stack */
//#define TM_ERROR_CHECKING

/*
 * Number of milliseconds per tick
 */
/* #define TM_TICK_LENGTH  10 */

/*
 * Mode 1:
 * Treck TCP/IP is used without an RTOS or Kernel
 */
/* #define TM_TRECK_NO_KERNEL */

/*
 * Mode 2:
 * Treck TC/IP is used as a shared library on a non-preemptive RTOS
 */
/* #define TM_TRECK_NONPREEMPTIVE_KERNEL */

/*
 * Mode 3:
 * Treck TCP/IP is used as an independent task on an RTOS
 * (preemptive or nonpreemtive)
 */
/* #define TM_TRECK_TASK */

/*
 * Mode 4:
 * Treck TCP/IP is used as a shared library on a premptive kernel
 */
#define TM_TRECK_PREEMPTIVE_KERNEL

#ifdef  TM_TRECK_DEMO
/*
 * Predefined Kernel Macros
 */
#define TM_PROBE_INTERVAL   100
#define TM_MAX_PROBE        2
#define TM_ERROR_CHECKING
#define TM_TRACE_LOGGING
#define TM_ERROR_LOGGING
#define TM_USE_REUSEADDR_LIST
#define TM_LOOP_TO_DRIVER
#define TM_KERNEL_WIN32_X86
#define TM_USE_DOS_FS
#define TM_USE_NETSTAT
#define TM_USE_IPV4
#ifndef TM_USE_RAM_FS /* not enough room in RAM FS for pcap file */
#define TM_USE_SNIFF
#endif /* TM_USE_RAM_FS */
#define TM_APP_CREATE_TIMER_THREAD
#define TM_USE_TEST_POINTS
#ifdef _DEBUG
#define TM_IKE_PHASE1_NULL_ENCRYPT
#define TM_IKE_PHASE1_REKEYING_TEST
#define TM_IKE_DEBUG
#else /* _DEBUG */
#ifdef TM_ERROR_CHECKING
/* undef TM_ERROR_CHECKING so that we don't generate memoryleak.dat */
#undef TM_ERROR_CHECKING
#endif /* TM_ERROR_CHECKING*/
#endif /* _DEBUG */
#define TM_SNMP_VERSION     2
#define TM_SNMP_CACHE
#endif /* TM_TRECK_DEMO */

#ifdef  TM_AUTO_TEST
#define TM_LOCK_NEEDED
#define TM_TRACE_LOGGING
#define TM_SNMP_CACHE
#define TM_MULTIPLE_CONTEXT
#define TM_KERNEL_VISUAL_X86
#define TM_USE_NETSTAT
#ifndef TM_USE_RAM_FS /* not enough room in RAM FS for pcap file */
#define TM_USE_SNIFF
#endif /* TM_USE_RAM_FS */
#define TM_USE_TEST_POINTS
#define TM_IKE_PHASE1_REKEYING_TEST
#define TM_USE_RAW_SOCKET
#endif /* TM_AUTO_TEST */

#ifdef  TM_KERNEL_REALOS_FR
/*
 * REALOS/FR
 * FUJITSU iTRON(ver3.0) for the MBxxxxx
 * It runs on a Fujitsu MBxxxxx platform
 */
#define TM_BIG_ENDIAN                      /* Use CPU Endian */
#define TM_TRECK_PREEMPTIVE_KERNEL         /* REAL/OS is Pre-Emptive Kernel */
#define TM_TICK_LENGTH  100                /* number of milliseconds per tick */
/*#define TM_BYPASS_ETHER_LL*/             /* Use Ethernet Module */
#define TM_TASK_RECV                       /* Use Recieve Task */
#define TM_USE_SHEAP                       /* Use Simple Heap Memory */
#ifdef  TM_USE_SHEAP
/* Simple Heap Memory Size is 256Kbyte */   
#define TM_SHEAP_SIZE TM_UL(256)*TM_UL(1024) 
#endif /* TM_USE_SHEAP */
#endif /* TM_KERNEL_REALOS_FR */

#ifdef TM_KERNEL_ELX_86
/*
 * ELX-86
 * This Kernel runs Treck TCP/IP as it's own task
 * It runs on a Intel x86 platform in Real mode
 * It also uses far memory(heap)
 */
#define TM_TRECK_TASK
#define TM_TICK_LENGTH  100 /* number of milliseconds per tick */
#define TM_INTEL_X86
#define TM_FAR far
/*
 * Define the ELX movsb for high speed copy
 */
void movsb(void TM_FAR *dest, const void TM_FAR *src, unsigned int len);
/* Use movsb for our tm_bcopy */
#define tm_bcopy(a,b,c) \
        movsb((void TM_FAR *)(b),(const void TM_FAR *)(a),(unsigned int)(c))

#endif /* TM_KERNEL_ELX_86 */

#ifdef TM_KERNEL_UCOS_XSCALE_RTASK
/*
 * uC/OS for the XSCALE Core of INTEL processors
 * uC/OS is preemptive and we use Treck TCP/IP as a shared library
 */
#define TM_KERNEL_UCOS_XSCALE
#define TM_LITTLE_ENDIAN
#define TM_USE_DRV_ONE_SCAT_SEND
#define TM_DEV_SEND_OFFLOAD
#define TM_DEV_RECV_OFFLOAD

#define TM_TICK_LENGTH  5 /* number of milliseconds per tick */
#define TM_OPTIMIZE_SPEED
/*
 * TM_ETHER_HW_ALIGN specifies what type of boundary (4 byte, 16 byte, etc)
 * the ethernet buffer is aligned on.  This is useful for devices that require
 * a DMA'ed recieve buffer to be aligned on a certain boundary.
 */
#define TM_ETHER_HW_ALIGN TM_UL(16) /* The XSCALE requires 16-byte alignment */

#define TM_BYPASS_ETHER_LL
/*
 * large heap size because of the large numbers of driver receive and
 * transmit descriptors.
 */
#define TM_SHEAP_SIZE TM_UL(512)*TM_UL(1024)
#define TM_USE_SHEAP

#endif /* TM_KERNEL_UCOS_XSCALE_RTASK */

#ifdef TM_KERNEL_UCOS_XSCALE
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_TASK_RECV
#endif /*TM_KERNEL_UCOS_XSCALE */

#ifdef TM_KERNEL_UCOS_PPC_RTASK
/*
 * uC/OS for the PPC Core of the Motorola processors
 * PPC includes the MPC860
 * uC/OS is preemptive and we use Treck TCP/IP as a shared library
 */
#define TM_KERNEL_UCOS_PPC
#define TM_TASK_RECV
#endif

#ifdef TM_KERNEL_UCOS_PPC
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_TICK_LENGTH  10 /* number of milliseconds per tick */
#define TM_MOTOROLA_PPC

/*
 * TM_ETHER_HW_ALIGN specifies what type of boundary (4 byte, 16 byte, etc)
 * the ethernet buffer is aligned on.  This is useful for devices that require
 * a DMA'ed recieve buffer to be aligned on a certain boundary.
 */
#define TM_ETHER_HW_ALIGN TM_UL(16) /* The MPC860 requires 16-byte alignment */

#define TM_BYPASS_ETHER_LL
/*
 * large heap size because of the large numbers of driver receive and
 * transmit descriptors.
 */
#define TM_SHEAP_SIZE TM_UL(256)*TM_UL(1024)
#define TM_USE_SHEAP
#endif /*TM_KERNEL_UCOS_PPC */

#ifdef TM_KERNEL_NP_UCOS_PPC_RTASK
/*
 * uC/OS for the PPC Core of the Motorola processors
 * PPC includes the MPC860
 * uC/OS is NON-Preemptive and we use Treck TCP/IP as a shared library
 */
#define TM_KERNEL_NP_UCOS_PPC
#define TM_TASK_RECV
#endif

#ifdef TM_KERNEL_NP_UCOS_PPC
#define TM_TRECK_NONPREEMPTIVE_KERNEL
#define TM_TICK_LENGTH  10 /* number of milliseconds per tick */
#define TM_MOTOROLA_PPC

/*
 * TM_ETHER_HW_ALIGN specifies what type of boundary (4 byte, 16 byte, etc)
 * the ethernet buffer is aligned on.  This is useful for devices that require
 * a DMA'ed recieve buffer to be aligned on a certain boundary.
 */
#define TM_ETHER_HW_ALIGN TM_UL(16) /* The MPC860 requires 16-byte alignment */
#define TM_BYPASS_ETHER_LL
/*
 * large heap size because of the large numbers of driver receive and
 * transmit descriptors.
 */
#define TM_SHEAP_SIZE TM_UL(256)*TM_UL(1024)
#define TM_USE_SHEAP
#endif /*TM_KERNEL_NP_UCOS_PPC */

#ifdef TM_KERNEL_NONE_H8S
/*
 * H8S, no kernel
 * CPU: H8S/2674R
 */
#define TM_HITACHI_H8S
#define TM_FAR
#define TM_TICK_LENGTH  10 /* number of milliseconds per tick */
#define TM_BYPASS_ETHER_LL
#define TM_USE_DRV_ONE_SCAT_SEND
#define TM_LOOP_TO_DRIVER
#define TM_USE_SHEAP
#define TM_SHEAP_SIZE TM_UL(128)*TM_UL(1024)
#define TM_TRECK_NO_KERNEL
#define TM_ETHER_POLLING_MODE
#define TM_INTERRUPT
#define TM_IGMP
#endif /*TM_KERNEL_NONE_H8S */

#ifdef TM_KERNEL_UCOS_RTASK
/*
 * uC/OS for the CPU32 Core of the Motorola 68K processors
 * CPU32 includes 68332 and 68360
 * uC/OS is preemptive and we use Treck TCP/IP as a shared library
 */
#define TM_KERNEL_UCOS_CPU32
#define TM_TASK_RECV
#endif

#ifdef TM_KERNEL_UCOS_CPU32
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_TICK_LENGTH  10 /* number of milliseconds per tick */
#define TM_MOTOROLA_CPU32
#ifdef TM_QUICC
#ifndef TM_SOC_SEND_Q_BYTES
/* Default send queue size in bytes */
#define TM_SOC_SEND_Q_BYTES 16384
#endif /* TM_SOC_SEND_Q_BYTES */

#define TM_BYPASS_ETHER_LL
#endif /* TM_QUICC */

/*
 * large heap size because of the large numbers of driver receive and
 * transmit descriptors.
 */
#define TM_SHEAP_SIZE TM_UL(256)*TM_UL(1024)
#define TM_USE_SHEAP
#endif /*TM_KERNEL_UCOS_CPU32 */

#ifdef TM_KERNEL_NP_UCOS_RTASK
/*
 * Non Preemptive uC/OS for the CPU32 Core of the
 * Motorola 68K processors
 * CPU32 includes 68332 and 68360
 * uC/OS is non-preemptive and we use Treck TCP/IP as a shared library
 */
#define TM_KERNEL_NP_UCOS_CPU32
#define TM_TASK_RECV
#endif

#ifdef TM_KERNEL_NP_UCOS_CPU32
#define TM_TRECK_NONPREEMPTIVE_KERNEL
#define TM_TICK_LENGTH  10 /* number of milliseconds per tick */
#define TM_MOTOROLA_CPU32
#ifdef TM_QUICC
#ifndef TM_SOC_SEND_Q_BYTES
/* Default send queue size in bytes */
#define TM_SOC_SEND_Q_BYTES 16384
#endif /* TM_SOC_SEND_Q_BYTES */

#define TM_BYPASS_ETHER_LL
#endif /* TM_QUICC */
/*
 * large heap size because of the large numbers of driver receive and
 * transmit descriptors.
 */
#define TM_SHEAP_SIZE TM_UL(256)*TM_UL(1024)
#define TM_USE_SHEAP
#endif /*TM_KERNEL_NP_UCOS_CPU32 */

#ifdef TM_KERNEL_AMX_CPU32
#define TM_TASK_RECV
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_TICK_LENGTH  10 /* number of milliseconds per tick */
#define TM_MOTOROLA_CPU32
#ifdef TM_QUICC            /* defined in sds360cc.bat */
#ifndef TM_SOC_SEND_Q_BYTES
/* Default send queue size in bytes */
#define TM_SOC_SEND_Q_BYTES 16384
#endif /* TM_SOC_SEND_Q_BYTES */

#define TM_BYPASS_ETHER_LL
#endif /* TM_QUICC */

/*
 * large heap size because of the large numbers of driver receive and
 * transmit descriptors.
 */
#define TM_SHEAP_SIZE TM_UL(256)*TM_UL(1024)
/* Use AMX memory pool code. Do not use our SHEAP (Simple HEAP) */
#endif /*TM_KERNEL_AMX_CPU32 */


#ifdef TM_KERNEL_AMX_X86
#define TM_TASK_RECV
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_TICK_LENGTH  55 /* number of milliseconds per tick */
#define TM_INTEL_X86
#endif /*TM_KERNEL_AMX_X86 */


#ifdef TM_KERNEL_DOS_X86
#define TM_TICK_LENGTH 55
#define TM_TRECK_NO_KERNEL
#define TM_INTEL_X86
/* #define TM_USE_SHEAP */
/* #define TM_SHEAP_SIZE TM_UL(54)*TM_UL(1024) */
/* #define TM_BYPASS_ETHER_LL */
#define TM_FAR far
#endif /* TM_KERNEL_DOS_X86 */


#ifdef TM_KERNEL_UCOS_X86
/*
 * uC/OS for the Intel Family
 * uC/OS is preemptive and we use Treck TCP/IP as a shared library
 */
#define TM_TICK_LENGTH  55 /* number of milliseconds per tick */
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_TASK_RECV
#define TM_INTEL_X86
#define TM_OPTIMIZE_SIZE
#define TM_BYPASS_ETHER_LL
#define TM_SHEAP_SIZE TM_UL(29)*TM_UL(1024)
#define TM_USE_SHEAP
#define TM_FAR far
#endif /*TM_KERNEL_UCOS_X86 */

#ifdef TM_KERNEL_WIN32_X86
/*
 * Windows 32 for the x86 family, 
 * Treck runs as multi-thread windows application.
 */
#define TM_USE_DOS_FS
#define TM_RT_TIMER_RESOLUTION TM_UL(1000) /* interop test: 1 sec resolution */
#define TM_TICK_LENGTH  10 /* number of milliseconds per tick */
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_TASK_RECV
#define TM_LITTLE_ENDIAN
#define TM_X86_ASM_SWAP
#define TM_OPTIMIZE_SPEED
#define TM_BYPASS_ETHER_LL
#define TM_SHEAP_SIZE   TM_UL(512)*TM_UL(1024)
#define TM_USE_SHEAP
/* To check for memory corruption */
#define TM_SHEAP_MARK_MEMORY
/* To check for end of allocated memory overwrite */
#define TM_DISABLE_DYNAMIC_MEMORY
#define TM_BSD_CLASH
#define TM_FAR
#define TM_USE_CLIB
#undef TM_INDRV_INLINE_SEND_RECV 
#endif /* TM_KERNEL_WIN32_X86 */

 
#ifdef TM_KERNEL_VISUAL_X86
/*
 * Loop back testing using Microsoft Visual C++
 */
#define TM_USE_DOS_FS
#define TM_FAR
#define TM_TICK_LENGTH  10 /* number of milliseconds per tick */
#define TM_TRECK_TASK /* Turbo Treck run in a single task */
#define TM_OPTIMIZE_SPEED
#define TM_LITTLE_ENDIAN
#define TM_X86_ASM_SWAP
#define TM_BYPASS_ETHER_LL
#define TM_SHEAP_SIZE   TM_UL(256)*TM_UL(1024)
#define TM_USE_SHEAP
/* To check for memory corruption */
#define TM_SHEAP_MARK_MEMORY
/* To check for end of allocated memory overwrite */
#define TM_DISABLE_DYNAMIC_MEMORY
#define TM_BSD_CLASH
#define TM_USE_CLIB
/*
 * To use interface driver instead of loop back driver, when
 * sending to our own IP address using the Ethernet link layer.
 */ 
#define TM_LOOP_TO_DRIVER 
#define TM_USE_DRV_ONE_SCAT_SEND /* Allow more efficient driver send */
/* Allow incoming scattered device driver recv data */
#define TM_USE_DRV_SCAT_RECV 
/* 
 * More efficient IP fragmentation when device driver supports scattered
 * send
 */
#define TM_IP_FRAGMENT_NO_COPY 
#endif /* TM_KERNEL_VISUAL_X86 */

/* Linux Raw_socket Driver */
/* #define TM_KERNEL_LINUX_APP */

#ifdef TM_KERNEL_LINUX_APP
/* For swap macro definitions, Linux socket calls */
#define TM_KERNEL_LINUX
#define TM_INTEL_GNU_ASM_SWAP
#define TM_FAR
#define TM_TICK_LENGTH        10 /* number of milliseconds per tick */
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_OPTIMIZE_SPEED
#define TM_TASK_RECV
#define TM_LITTLE_ENDIAN
#define TM_BYPASS_ETHER_LL
#define TM_SHEAP_SIZE   TM_UL(256)*TM_UL(1024)
#define TM_USE_SHEAP
#define TM_DYNAMIC_CREATE_SHEAP
#define TM_BSD_CLASH
/* following define is for SNMPD Trace*/
#define TM_DEBUG_LOGGING
#define TM_TRACE_LOGGING
#define TM_ERROR_LOGGING
#define TM_DEF_MAX_LOG_MSG_LEN 4096
#define TM_DEF_NUM_LOG_MSGS    10
#endif /* TM_KERNEL_LINUX_APP */

#ifdef TM_KERNEL_NP_NUCLEUSRTX_C3X
/*
 * Nucleus RTX for the TI TMS320C3x Family
 * Running Nucleus RTX in non-preemptive mode
 */
#define TM_TRECK_NONPREEMPTIVE_KERNEL
#define TM_TICK_LENGTH  10
#define TM_BYPASS_ETHER_LL
#endif /* TM_KERNEL_NUCLEUSRTX_C3X */

#ifdef TM_KERNEL_NONE_ARM7
/*
 * ARM7 processor, no RTOS
 */
#define TM_ARM7
#define TM_TICK_LENGTH 10
#define TM_USE_SHEAP
#define TM_SHEAP_SIZE TM_UL(128)*TM_UL(1024)
#define TM_TRECK_NO_KERNEL
#define TM_BYPASS_ETHER_LL
#endif /* TM_KERNEL_NONE_ARM7 */

#define TM_KERNEL_THREADX_ARM7 // !!!!!!!!!!!!!! ON HERE 

#ifdef TM_KERNEL_THREADX_ARM7
/*
 * ThreadX for the ARM7
 */
#define TM_ARM7
#define TM_TICK_LENGTH 10
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_TASK_RECV
#define TM_BYPASS_ETHER_LL
#endif /* TM_KERNEL_THREADX_ARM7 */

#ifdef TM_KERNEL_THREADX_CPU32
/*
 * ThreadX for the Motorola CPU32 core (68332 & 68360)
 */
#define TM_MOTOROLA_CPU32
#define TM_TICK_LENGTH 10
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_TASK_RECV
#define TM_BYPASS_ETHER_LL
#endif /* TM_KERNEL_THREADX_CPU32 */

#ifdef TM_KERNEL_QUADROS
/*
 * Quadros for Motorola Coldfire
 */
#define TM_TICK_LENGTH 10
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_TASK_RECV
#define TM_BYPASS_ETHER_LL
#define TM_OPTIMIZE_SPEED
#define TM_OPTIMIZE_SIZE
#define TM_USE_NETSTAT
#define TM_SNMP_MIB
#define TM_ARP_UPDATE_ON_RECV
#define TM_USE_REUSEADDR_LIST
#define TM_USE_RAW_SOCKET
#define TM_IGMP
#define TM_USE_CLIB
#define TM_USE_SHEAP
#define TM_SHEAP_PAGE_SIZE  (TM_UL(64)*TM_UL(1024))
#define TM_SHEAP_SIZE  TM_SHEAP_PAGE_SIZE /* in bytes */
#define TM_SHEAP_NUM_PAGE   (TM_SHEAP_SIZE/TM_SHEAP_PAGE_SIZE)
#define TM_SHEAP_MIN_BLOCK_SIZE 16 /* in bytes */
#define TM_DYNAMIC_CREATE_SHEAP
#define TM_ETHER_HW_ALIGN       16 /* ethernet receive buffer alignment */
#endif /* TM_KERNEL_QUADROS */
  
#ifdef TM_KERNEL_TI_DSP_BIOS
/*
 * TI DSP BIOS 
 */
#define TM_TICK_LENGTH 10
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_TASK_RECV
#define TM_TASK_XMIT
#define TM_USE_CLIB
#define TM_OPTIMIZE_SIZE
#define TM_OPTIMIZE_SPEED
#define TM_BYPASS_ETHER_LL
#define TM_IP_FRAGMENT_NO_COPY
/* Default recv queue size in bytes */
#define TM_SOC_RECV_Q_BYTES  16384
/* Default send queue size in bytes */
#define TM_SOC_SEND_Q_BYTES  16384
#endif /* TM_KERNEL_TI_DSP_BIOS */

#ifdef TM_KERNEL_NONE_EZ80
/*
 * EZ80 evaluation board, no kernel
 */
#define TM_EZ80
#define TM_USE_SHEAP
#define TM_SHEAP_SIZE TM_UL(128)*TM_UL(1024)
#define TM_TRECK_NO_KERNEL
#define TM_BYPASS_ETHER_LL
#define TM_Z80L92_ASM_OCS
#endif /* TM_KERNEL_NONE_EZ80 */

#ifdef TM_KERNEL_RZK_EZ80
/*
 * EZ80 evaluation board, RZK(RTOS)
 */
#define TM_EZ80
#define TM_USE_SHEAP
#define TM_SHEAP_SIZE TM_UL(128)*TM_UL(1024)
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_TASK_RECV
#define TM_Z80L92_ASM_OCS
#define TM_USE_KERNEL_CRITICAL_STATUS

#ifndef TM_USER_KERNEL_CRITICAL_STATUS_TYPE
#define TM_USER_KERNEL_CRITICAL_STATUS_TYPE
typedef unsigned int ttUserKernelCriticalStatus;
#endif /* TM_USER_KERNEL_CRITICAL_STATUS_TYPE */

extern unsigned int RZKDisableInterrupts(void);
extern void RZKEnableInterrupts(unsigned int interruptsMask);

#define tm_kernel_set_critical \
            tvKernelCriticalStatus = RZKDisableInterrupts()
#define tm_kernel_release_critical \
            RZKEnableInterrupts(tvKernelCriticalStatus)

#endif /*TM_KERNEL_NONE_EZ80 */


#ifdef TM_KERNEL_UCOS_MIPS
/*
 * MIPS4kc, Malta evaluation board, ucos kernel
 * gnu tools: sde-mips
 */
#define TM_KERNEL_MIPS4k
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_TASK_RECV
#define TM_TICK_LENGTH        10 
#define TM_GNUMIPS4K_ASM_CRITICAL
/* define ENDIAN in makefile for little and big */
#define TM_BYPASS_ETHER_LL
#define TM_SHEAP_SIZE   TM_UL(256)*TM_UL(1024)
#define TM_USE_SHEAP
#define printf tfMipsPrintf
#endif /* TM_KERNEL_UCOS_MIPS */


#ifdef TM_KERNEL_UCOS_SH2
/*
 * HITACHI SH2 Processor - uCOS-II ( big endian )
 *  SH7616 (SH2-DSP) evaluation board.
 */
#define TM_HITACHI_SH2
#define TM_TASK_RECV
#define TM_TASK_SEND
#define TM_TASK_XMIT
#define TM_TICK_LENGTH  10   /* number of milliseconds per tick */
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_OPTIMIZE_SPEED
#define TM_BYPASS_ETHER_LL
#define TM_USE_SHEAP
#define TM_SHEAP_SIZE ( 128UL * 1024UL )
#define TM_USE_DRV_ONE_SCAT_SEND
#endif /* TM_KERNEL_UCOS_SH2 */

#ifdef TM_KERNEL_CMX_C16X
/*
 * Infineon  C166 Processor - CMX RTOS
 * Vorne 87AL board. 
 */
#define TM_INFINEON_C166
#define TM_TICK_LENGTH 10
#define TM_TRECK_PREEMPTIVE_KERNEL
#define TM_BYPASS_ETHER_LL
#define TM_DISABLE_TCP_SACK
#define TM_USE_DRV_ONE_SCAT_SEND
#define TM_OPTIMIZE_SPEED

#define TM_TASK_RECV
/* #define TM_TASK_XMIT */

/* Dymanically allocate Sheap as pages of 16kbytes */
#define TM_USE_SHEAP
#define TM_DYNAMIC_CREATE_SHEAP
#define TM_SHEAP_SIZE       (TM_UL(128)*TM_UL(1024))
#define TM_SHEAP_PAGE_SIZE  (TM_UL(64)*TM_UL(1024))
#define TM_SHEAP_NUM_PAGE   (TM_SHEAP_SIZE/TM_SHEAP_PAGE_SIZE)

#define TM_FAR huge
#define TM_GLOBAL_QLF huge
#define TM_CONST_QLF huge
#define TM_INTERRUPT

#ifdef TM_ERROR_CHECKING
#undef TM_ERROR_CHECKING
#endif

#define TM_SINGLE_INTERFACE_HOME

/*inlcude the KEIL compiler library that supports huge argement type */
#include "trkeil.h"
#include <string.h>
#include <ctype.h>

#endif /* TM_KERNEL_CMX_C16X */

#ifdef TM_INTEL_X86
/*
 * Intel x86
 */
#define TM_LITTLE_ENDIAN
#ifdef __BORLANDC__
/* In line assembly critical section */
#define TM_X86_ASM_CRITICAL
/* In line network byte ordering */
#define TM_X86_ASM_SWAP
/* Use assembly checksum routine */
#define TM_BORLAND_ASM_OCS
#endif /* __BORLANDC__ */
#ifdef _MSC_VER
/* In line assembly critical section */
#define TM_X86_ASM_CRITICAL
/* In line network byte ordering */
#define TM_X86_ASM_SWAP
#endif /* _MSC_VER */
#endif /* TM_INTEL_X86 */

#ifdef TM_INTEL_XSCALE
/*
 * Intel XScale (80200)
 */
#define TM_LITTLE_ENDIAN
/* Use assembly checksum routine */
#define TM_GNUXSCALE_ASM_OCS
#endif /* TM_INTEL_XSCALE */

#ifdef TM_MOTOROLA_PPC
/*
 * Motorola Power PC Core
 */
#undef TM_LITTLE_ENDIAN
#ifdef TM_COMPILER_DDI_PPC
/* In line assembly critical section */
#define TM_DDIPPC_ASM_CRITICAL
#define TM_DDIPPC_ASM_OCS
#endif /* TM_COMPILER_DDI_PPC */

#ifdef TM_COMPILER_GHS_PPC
#define TM_GHSPPC_ASM_CRITICAL
#define TM_GHSPPC_ASM_OCS
#endif /* TM_COMPILER_GHS_PPC */

#ifdef TM_COMPILER_GNU_PPC
#define TM_GNUPPC_ASM_CRITICAL
#define TM_GNUPPC_ASM_OCS
#endif /* TM_COMPILER_GNU_PPC */
#endif /* TM_MOTOROLA_PPC */

#ifdef TM_MOTOROLA_CPU32
/*
 * Motorola CPU32 Core (68332 & 68360)
 */
#undef TM_LITTLE_ENDIAN
#ifdef TM_COMPILER_SDS
/* In line assembly critical section */
#define TM_SDS68K_ASM_CRITICAL
#define TM_SDS68K_ASM_OCS
#endif /* TM_COMPILER_SDS */

#ifdef TM_COMPILER_MRI
#define TM_MRI_ASM_CRITICAL
#endif /* TM_COMPILER_MRI */

#endif /* TM_MOTOROLA_CPU32 */

#ifdef TM_COMPILER_CW_5282
/* Metrowerks CodeWarrior for Motorola 5282 (Coldfire) */
#define TM_CW5282_ASM_OCS
#endif /* TM_COMPILER_CW_5282 */

#ifdef TM_MOTOROLA_68K
/*
 * Motorola 68000 (EC) Core
 */
#undef TM_LITTLE_ENDIAN

#ifdef TM_COMPILER_SDS
#define TM_SDS68K_ASM_CRITICAL
#endif /* TM_COMPILER_SDS */

#ifdef TM_COMPILER_MRI
#define TM_MRI_ASM_CRITICAL
#endif /* TM_COMPILER_MRI */

#endif /* TM_MOTOROLA_68K */

#ifdef TM_HITACHI_H8S
/*
 * Hitachi H8S
 */
#undef TM_LITTLE_ENDIAN
#endif /* TM_HITACHI_H8S */

#ifdef TM_HITACHI_SH2
/*
 * Hitachi SH2
 */
#define TM_BIG_ENDIAN
#endif /* TM_HITACHI_SH2 */


#ifdef TM_INFINEON_C166
/*
 * TM_INFINEON_C166
 */
#define TM_LITTLE_ENDIAN
#define TM_C166_OCS
#endif /* TM_INFINEON_C166 */


#ifdef TM_ARM7
/*
 * ARM7 Core
 */
#define TM_LITTLE_ENDIAN

#ifdef TM_COMPILER_GHS_ARM
#define TM_GHSARM7_ASM_CRITICAL
#define TM_GHSARM7_ASM_OCS
#endif /* TM_COMPILER_GHS_ASM */

#endif /* TM_ARM7 */




/*
 * Texas Instruments TMS320C5x Core
 */

#ifdef TM_TMS320_C6
/*
 * Texas Instruments TMS320C6000 core
 */
#define TM_USER_32BIT_TYPE
typedef unsigned int ttUser32Bit;
typedef signed int   ttUserS32Bit;

#define TM_UL(x) x##U
#define TM_L(x) x

#define TM_TI_C6000_ASM_OCS
#endif /* TM_TMS320_C6 */


#ifdef TM_EZ80
/*
 * Zilog eZ80 processor
 */
#define TM_FAR
#define TM_TICK_LENGTH  55 /* number of milliseconds per tick */
#define TM_LITTLE_ENDIAN
#endif /* TM_EZ80 */

#ifndef TM_TICK_LENGTH
#define TM_TICK_LENGTH 0
#endif /* TM_TICK_LENGTH */




/*
 * For phase 1 negotiation, Elmic IKE supports digital signature
 * authentication method. It includes RSA/DSA algorithm, PEM/DER 
 * certification, X509v3 and ASN1 format, multi CA and CRL. TM_USE_PKI
 * macro shoule be defined in order to use this part. Whenever you define
 * TM_USE_SSL_CLIENT or TM_USE_SSL_SERVER, this macro is automatically
 * defined
 */
/* #define TM_USE_PKI */






/*
 * SSL TLS options
 */
/* Uncomment the following macro if user doesn't want SSL 3.0 */
#define TM_USE_SSL_VERSION_30

/* Uncomment the following macro if user doesn't want SSL 3.1, i.e TLS 1.0 */
#define TM_USE_SSL_VERSION_31

/* uncomment the following macro if you don't want to accept SSL 2.0 hello */
#define TM_SSL_ACCEPT_20_CLIENTHELLO
/* If the following macro is defined, upon receiving close_notify, we will
 * immediately flush our send queue and send back close_notify to the peer,
 * and shutdown SSL send path. If this macro is not defined, it is up to the 
 * user to call tfSslConnectUserClose or tfClose to shutdown the SSL send 
 * path. Note that, if user doesn't call anything, the SSL send path is 
 * still ON until the socket is closed.
 */
#define TM_SSL_IMMEDIATE_CLOSE_NOTIFY

/* The minimum user data bytes to TRIGGER SSL send. Data less than this size 
 * will not trigger SSL send, instead, it will be queued.  Once SSL send
 * has been triggered up, all pending user data will be sent out and after
 * that SSL waits for the next trigger event to send again.
 */
#define TM_SSL_SEND_DATA_MIN_SIZE                200

/* The maximum value of user data in one single SSL record. By RFC, this value
 * is 16384. However, considering that peer may not have enough receive buffer
 * to queue, we use the following value (8000). So if user wants to send more 
 * than 8000 bytes of data, see 8100 bytes, we have to cut it into two SSL 
 * records, one contains 8000 user data bytes, another one contains 100 user 
 * data bytes (Note that even if 100 is less than TM_SSL_SEND_DATA_MIN_SIZE, 
 * we still send it out, because we used 8100 bytes to trigger up SSL send.)
 */
#define TM_SSL_SEND_DATA_MAX_SIZE                8000 

/* uncomment the following macro, if you don't want to use any ephemeral
 * Diffie-Hellman key exchange method. For example, if your SSL cipher suite
 * is something like SSL_RSA_XXXX or TLS_RSA_XXXX (The most popular case in 
 * real world), you don't need this macro.
 */
#define TM_SSL_USE_EPHEMERAL_DH

/* uncomment the following macro if you don't want do mutual authentication*/
#define TM_SSL_USE_MUTUAL_AUTH

/* If you don't define TM_EXPORT_RESTRICT, you may want to reject any 
 * exportable cipher suites by defining the following macro. Note that, 
 * by defining the following macro, you get higher security because you
 * reject any exportable cipher suite, the cost is that you may not be 
 * able to negotiation with clients which offer exportable cipher suites
 * only. If you do define TM_EXPORT_RESTRICT, this macro has no effect
 * to the behavior. Default not to be defined
 */
/*#define TM_SSL_REJECT_EXPORTABLE_CIPHER*/

/* uncomment the following macro only if 1) you want to negotiate EDH_DSS_XXX
 * cipher 2) negotiate with Netscape only 3) in SSL version 3.0 version only. 
 * If any of these three conditions is false, don't uncomment this macro.  
 * The description of how to encode DSS signature in the SSLv3 specification 
 * is insufficiently specific and Netscape interprets it differently from most
 * other vendors. Instead of DER encoding r and s, Netscape merely 
 * concatenates them into a single 40-byte field. Thus, although Netscape 
 * implements DSS for client authentication, it is not interoperable with 
 * other implementations. Despite widespread agreement on the "right thing" 
 * Netscape has refused to change their implementation, claiming that it 
 * complies with the SSL v3 specification and citing installed base. The TLS
 * specification clears up this issue; ALL TLS implementations must DER 
 * encode DSS signature 
 */
/* #define TM_SSL30_DSSSIGN_NETSCAPE_FORMAT */

/*The following Diffie-Hellman parameters take effect only if Ephemeral Diffie
 * hellman is enabled and is chosen as the key exchange method.
 */

#ifndef TM_EXPORT_RESTRICT
/* if you don't have export restrict, we use OAKLEY group 1 prime
 * (768 bits)if Ephemeral Diffie-Hellman is going to be used.
 */
#define TM_SSL_DHE_PRIME_GROUP             TM_DHGROUP_1
#else /* TM_EXPORT_RESTRICT */
/* If you have export restrict, we use 520 bits prime */
#define TM_SSL_DHE_PRIME_GROUP             TM_DHGROUP_EXPORT
#endif /* TM_EXPORT_RESTRICT */

/* Diffie-Hellman generator is 2 */
#define TM_SSL_DHE_GENERATOR               TM_DH_GENERATOR_2

/* Diffie-Hellman private secret default size*/
#define TM_SSL_DHSECRET_DEFAULT_SIZE       32


/*
 * Uncomment the following line, if you want to use C library routines,
 * instead of the equivalent library routines provided by the Turbo Treck 
 * stack.
 */
//#define TM_USE_CLIB

#ifdef TM_USE_CLIB
/* NOTE: you may need to modify the following, if you #define TM_USE_CLIB
 *
 * For isupper, tolower ... */
#include <ctype.h>
/* For memcpy, strchr ...   */
#include <string.h>
/* For printf, sprintf...   */
#include <stdio.h>
/* For strtoul... */
#include <stdlib.h>
#endif /* TM_USE_CLIB */


/* 
 * if TM_USE_SNIFF is defined, interface drivers will dump the packets to the
 * file system in libpcap format, to be opened by tcpdump, windump, ethereal, 
 * or other libpcap file readers.
 */
#ifndef TM_USE_RAM_FS /* not enough room in RAM FS for pcap file */
/* #define TM_USE_SNIFF */
#endif /* TM_USE_RAM_FS */

#ifdef __cplusplus
}
#endif

#define printf(x)  /* gone !!!!!!! */

#define tm_bcopy(a, b, c) marvell_bcopy(a, b, c)

extern void (*marvell_bcopy)(const void * sourcePtr,
                             void * destinationPtr,
			     unsigned int length);

#define tm_bzero(a, b) marvell_bzero(a, b)

extern void (*marvell_bzero)(const void * bufferPtr, unsigned int length);

#endif /* _TRSYSTEM_H_ */

/* Processed by Treck Shipping Tool v2.37 
 * 2004/11/17 12:28:54      For: Marvell
 * File Name: trsystem.h    File Revision: 4.1 
 * Project Name: tcpip.pj   Project Release: 4.1.2.1
 * Input checksum: 40486    Output checksum: 31247
 */
