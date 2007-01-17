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
 * Description: Type Definitions
 * Filename: trtype.h
 * Author: Odile & Paul
 * Date Created: 9/23/97
 * $Source: include/trtype.h $
 *
 * Modification History
 * $Revision: 4.1 $
 * $Date: 2004/11/07 21:05:25PST $
 * $Author: xwang $
 * $ProjectName: /home/mks/proj/tcpip.pj $
 * $ProjectRevision: 4.1.2.1 $
 */

#ifndef _TRTYPE_H_

#define _TRTYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Standard "C" Types
 */
typedef unsigned char  ttUChar;
typedef unsigned char  tt8Bit;
typedef signed   char  ttSChar;
typedef unsigned short ttUShort;
typedef unsigned short tt16Bit;
typedef signed   short ttS16Bit;
typedef signed   short ttSShort;
typedef ttUser32Bit    ttULong;
typedef ttUser32Bit    tt32Bit;
typedef ttUserS32Bit   ttS32Bit;
typedef ttUserS32Bit   ttSLong;
typedef void           ttVoid;


typedef const tt8Bit   TM_FAR  * ttConst8BitPtr;
typedef tt8Bit         TM_FAR  * tt8BitPtr;
typedef tt16Bit        TM_FAR  * tt16BitPtr;
typedef tt32Bit        TM_FAR  * tt32BitPtr;
typedef const tt32Bit  TM_FAR  * ttConst32BitPtr;
typedef tt8BitPtr      TM_FAR  * tt8BitPtrPtr;
typedef tt16BitPtr     TM_FAR  * tt16BitPtrPtr;
typedef tt32BitPtr     TM_FAR  * tt32BitPtrPtr;

typedef union tuPtr
{
    tt16BitPtr      u16Ptr;
    tt32BitPtr      u32Ptr;
    tt8BitPtr       u8Ptr;
    tt8BitPtrPtr    u8PtrPtr;
    tt16BitPtrPtr   u16PtrPtr;
    tt32BitPtrPtr   u32PtrPtr;
} ttPtrUnion;

typedef struct ts64Bit {
    tt32Bit high;
    tt32Bit low;
} tt64Bit;
typedef struct tsS64Bit {
    ttS32Bit high;
    tt32Bit low;
} ttS64Bit;

typedef tt64Bit        TM_FAR  * tt64BitPtr;
typedef tt64BitPtr     TM_FAR  * tt64BitPtrPtr;

typedef ttULong        TM_FAR  * ttULongPtr;
typedef void           TM_FAR  * ttVoidPtr;
typedef const void     TM_FAR  * ttConstVoidPtr;
typedef char           TM_FAR  * ttCharPtr;
typedef const char     TM_FAR  * ttConstCharPtr;
typedef int            TM_FAR  * ttIntPtr;
typedef unsigned int   TM_FAR  * ttUIntPtr;

typedef ttVoidPtr      TM_FAR  * ttVoidPtrPtr;
typedef ttCharPtr      TM_FAR  * ttCharPtrPtr;


/*
 * Pointer to a function that takes one pointer argument and that returns
 * an int
 */
typedef  int   (TM_CODE_FAR * ttIntGenFuncPtr)(ttVoidPtr argPtr);

/*
 * Pointer to a function that takes one pointer argument and that returns
 * a void
 */
typedef  void  (TM_CODE_FAR * ttVoidGenFuncPtr)(ttVoidPtr argPtr);

/*
 * Pointer to a function that takes one pointer argument and int value and
 * that returns a void
 */
typedef  void  (TM_CODE_FAR * ttVoidIntFuncPtr)(ttVoidPtr argPtr, int intval);

/*
 * Generic Function Ptr to pass between functions when parms are variable
 */
typedef  void  (TM_CODE_FAR * ttVoidFuncPtr)();

/*
 * Generic Parameter Passing Union Typedef
 */
typedef union tuGenericUnion
{
    tt32Bit         gen32bitParm;
    ttS32Bit        genSlongParm;
    ttVoidPtr       genVoidParmPtr;
    tt16Bit         gen16BitParm;
    int             genIntParm;
    tt8Bit          gen8BitParm;
    char            genCharParm;
} ttGenericUnion;

typedef ttGenericUnion TM_FAR * ttGenericUnionPtr;

/* Node descriptor for Generic linked list */
typedef struct  tsNode
{
    struct tsNode TM_FAR * nodePrevPtr;
    struct tsNode TM_FAR * nodeNextPtr;
} ttNode;
typedef ttNode TM_FAR * ttNodePtr;
#define TM_NODE_NULL_PTR (ttNodePtr)0

/* Generic linked list, with count */
typedef struct  tsList
{
    ttNode          listHeadNode;
    unsigned int    listCount;
} ttList;
typedef ttList TM_FAR * ttListPtr;
#define TM_LIST_NULL_PTR (ttListPtr)0 

/* 
 * function type for the list call back function 
 * return TM_8BIT_NO  to ask the Traverser to continue,
 * return TM_8BIT_YES to ask the Traverser to stop 
 * and return the current node
 */
typedef int (TM_CODE_FAR * ttListCBFuncPtr)(
    ttNodePtr       nodePtr, 
    ttGenericUnion  genParam1);

typedef tt32Bit tt4IpAddress;
typedef tt4IpAddress ttIpAddress;

typedef tt16Bit ttIpPort;
typedef tt8Bit  ttEnetAddress[6];

/* Define IpAdress union for suppress the pointer cast error(error 740). */
typedef union tu4IpAddress
{
    tt4IpAddress    addrLong;
    tt16Bit         addrShort[2];
    tt8Bit          addrChar[4];
} tt4IpAddressUnion;

typedef union tuEnetAddress
{
/* To make enuAddress aligned on 16-bit boundary */
    tt16Bit        enuShort;
    ttEnetAddress  enuAddress; /* Ethernet address */
} ttEnetAddressUnion;

typedef ttEnetAddress TM_FAR * ttEnetAddressPtr;

/* Shortened sockaddr_in (with unused part removed) */
typedef struct tsSockAddrIn
{
    u_char          ssin_len;    /* Total Length */
    u_char          ssin_family; /* Address Family AF_xxx */
    u_short         ssin_port;   /* 16bit Port Number in network byte order */
    struct  in_addr ssin_addr;   /* 32bit netid/hostid in network byte order */
} ttSockAddrIn;

typedef  ttSockAddrIn TM_FAR * ttSockAddrInPtr;

typedef ttIpAddress             TM_FAR * ttIpAddressPtr;
typedef tt4IpAddress            TM_FAR * tt4IpAddressPtr;
typedef const tt4IpAddress      TM_FAR * tt4ConstIpAddressPtr;
typedef ttIpPort                TM_FAR * ttIpPortPtr;

/*
 * Socket address union
 */
typedef union tuSockAddrPtr
{
    ttSockAddrIn TM_FAR                 *sockTInPtr;
    struct sockaddr_in TM_FAR           *sockInPtr;
    struct sockaddr TM_FAR              *sockPtr;
    struct sockaddr_storage TM_FAR      *sockNgPtr;
} ttSockAddrPtrUnion;

typedef ttSockAddrPtrUnion TM_FAR * ttSockAddrPtrUnionPtr;


typedef struct tsAncHdr
{
/* a ttNode to link multiple ancillary header together */
    ttNode      ancNode;
/* length of the ansData buffer that follows*/
    tt32Bit     ancDataLen;
/* ancillary data, may have one or more cmsghdr (ttAncillaryData) in it 
 * the actuall length of this filed is determined by ancDataLen
 */
    tt8Bit      ancData[4];
} ttAncHdr;
typedef ttAncHdr TM_FAR * ttAncHdrPtr;

/*
 * Counting Semaphore List
 */
typedef struct tsCountSem
{
    ttGenericUnion            couCountSem;
    struct tsCountSem TM_FAR *couNext;
} ttCountSem;

typedef ttCountSem TM_FAR * ttCountSemPtr;

/*
 * Lock Entry
 */
typedef struct tsLockEntry
{
    ttCountSemPtr      locCountSemPtr;
    tt8Bit             locCount;
/* Flag to see if Counting Semaphore is Valid */
    tt8Bit             locCountSemValid;
    tt8Bit             locFiller[2];
} ttLockEntry;

typedef ttLockEntry TM_FAR * ttLockEntryPtr;

#ifdef TM_ERROR_CHECKING
/* Entry containing a lock entry, and a flag */
struct tsLockCheck
{
    ttLockEntry lckLockEntry;
    int         lckFlag;
    int         lckFiller;
};

typedef struct tsLockCheck TM_FAR * ttLockCheckPtr;

#endif /* TM_ERROR_CHECKING */


/*
 * Pend Entry
 */
typedef struct tsPendEntry
{
    ttCountSemPtr      pendSemaphorePtr;
    tt16Bit            pendStatus;
    tt8Bit             pendCount;
    tt8Bit             pendPostCount;
} ttPendEntry;
typedef ttPendEntry TM_FAR * ttPendEntryPtr;

/*
 * Event Entry
 */
typedef struct tsEventEntry
{
    ttUserGenericUnion eventUnion;
    tt16Bit            eventCount;
    tt8Bit             eventFiller[2];
} ttEventEntry;
typedef ttEventEntry TM_FAR * ttEventEntryPtr;

/*
 * Raw buffer types
 */
typedef struct tsRawMemory
{
    struct tsRawMemory TM_FAR * rawNextPtr;
}ttRawMemory;

typedef ttRawMemory     TM_FAR * ttRawMemoryPtr;
typedef ttRawMemoryPtr  TM_FAR * ttRawMemoryPtrPtr;


typedef tt8Bit                   ttRawBuffer;
typedef ttRawBuffer     TM_FAR * ttRawBufferPtr;

/*
 * Generic recycle entry
 */
typedef struct tsRecycleEntry
{
    struct tsRecycleEntry TM_FAR * rcylNextPtr;
} ttRecycleEntry;

typedef ttRecycleEntry  TM_FAR * ttRcylPtr;
typedef ttRcylPtr       TM_FAR * ttRcylPtrPtr;



/* Network header: IP Header */
typedef struct  tsIpHeader
{
    tt8Bit          iphVersLen;         /* IP header version and length */
    tt8Bit          iphTos;             /* Type of service */
    tt16Bit         iphTotalLength;     /* IP datagram length */
    tt16Bit         iphId;              /* IP header Identifaction field */
    tt16Bit         iphFlagsFragOff;    /* IP Fragmentation flags & offset */
    tt8Bit          iphTtl;             /* IP time to live */
    tt8Bit          iphUlp;             /* ULP (17 for UDP, 6 for TCP..) */
    tt16Bit         iphChecksum;        /* IP header checksum */
    tt4IpAddress    iphSrcAddr;         /* IP source address */
    tt4IpAddress    iphDestAddr;         /* Ip destination address */
} ttIpHeader;

typedef ttIpHeader TM_FAR * ttIpHeaderPtr;

/* END of IP network header */

#define pktRtePtr             pktSharedDataPtr->dataRtePtr
#define pktDestDevPtr         pktSharedDataPtr->dataDestDevPtr
#define pktTcpUnscaledWnd     pktSharedDataPtr->dataTcpUnscaledWnd
#define pktPppAccm            pktSharedDataPtr->dataPppAccm
/* packet source IP address */
#define pktSrcIpAddress       pktSharedDataPtr->dataSrcIpAddress
/* packet destination IP address */
#define pktDestIpAddress      pktSharedDataPtr->dataDestIpAddress
/* packet specific destination IP address */
#define pktSpecDestIpAddress  pktSharedDataPtr->dataSpecDestIpAddress
#define pktTcpUrgentOffset    pktSharedDataPtr->dataTcpUrgentOffset
#define pktEthernetAddress    pktSharedDataPtr->dataEthernetAddress
#define pkt6PhysAddr          pktSharedDataPtr->data6PhysAddr        
#define pkt6PhysAddrLen       pktSharedDataPtr->data6PhysAddrLength
#define pktTcpHdrLen          pktSharedDataPtr->dataTcpHdrLen
/* Not last fragment */
#define pktFragMore           pktSharedDataPtr->dataFragMore
/* TCP Time stamp echo reply */
#define pktTcpTsEcr           pktSharedDataPtr->dataTcpTsEcr
/* Begin byte offset of fragment in fragmented IP datagram */
#define pktFragBBOffset       pktSharedDataPtr->dataFragBBOffset
/* End byte offset of fragment in fragmented IP datagram */
#define pktFragEBOffset       pktSharedDataPtr->dataFragEBOffset

/*
 * structure allocated each time a Treck buffer is allocated. At least one
 * ttPacket points to the ttSharedData structure. This structure and data
 * associated with it can be shared among different ttPacket. See ttPacket
 * definition below.
 */
typedef struct  tsSharedData
{
/*
 * For send TCP recycle queue, or for UDP/ICMP send datagrams
 */
    struct tsSocketEntry TM_FAR *dataSocketEntryPtr;
/*
 * For User device driver free data routine. Pointer to device which
 * allocated the device user data.
 */
    struct tsDeviceEntry TM_FAR *dataDeviceEntryPtr;
/*
 * For User device driver free routine. Pointer to user device driver
 * data pointer.
 */
    ttCharPtr                    dataDeviceUserDataPtr;
/* Pointer to routing entry for outgoing packet */
    struct tsRteEntry    TM_FAR *dataRtePtr;

    union
    {
        ttEnetAddress dataUnEthernetAddress;

        tt16Bit       dataPadding;

#define dataEthernetAddress  dataEthPhysAddrUnion.dataUnEthernetAddress
#define data6PhysAddr        dataEthPhysAddrUnion.data6UnPhysAddr
        
    } dataEthPhysAddrUnion;

    
/*
 * Pointer to the beginnning of the allocated data buffer.  This pointer
 * is used by tfDeviceSend(), tfTcpGetTcpRecycledHeader()
 * to determine the beginning of the buffer.
 */
    tt8BitPtr                    dataBufFirstPtr;
/*
 * Pointer to the end of the data buffer, prior to the trailer.  This pointer
 * is used by tfSendAppend, tfTcpGetTcpHeader and tfSocketIncomingPacket
 * to determine the total size of the buffer.
 */
    tt8BitPtr                    dataBufLastPtr;

    tt16Bit                      dataPortNumber;
#define dataLinkLayerProtocol    dataPortNumber
    tt16Bit                      dataFlags;

    tt8Bit                       dataOwnerCount;
    tt8Bit                       dataTcpHdrLen;

/* Not last fragment */
#define dataFragMore             dataTcpHdrLen
    union
    {
/* 32-bit TCP unscaled window in host byte order (incoming packet) */
#define dataTcpUnscaledWnd       dataFirstUnion.dataUnTcpUnscaledWnd
        tt32Bit                      dataUnTcpUnscaledWnd;
/* PPP accm for outgoing packet */
#define dataPppAccm              dataFirstUnion.dataUnPppAccm
        tt32Bit                      dataUnPppAccm;
/*
 * packet destination IP address (copied from IP header) (incoming packet).
 * Not used with the TCP protocol
 */
#define dataDestIpAddress         dataFirstUnion.dataUnDestIpAddress
        tt4IpAddress                 dataUnDestIpAddress;
    } dataFirstUnion;
    union
    {
/* TCP Time stamp echo reply (incoming packet) */
#define dataTcpTsEcr             dataSecondUnion.dataUnTcpTsEcr
        tt32Bit                      dataUnTcpTsEcr;
/* To keep track of user application send data (for UDP, ICMP) */
#define dataUserDataLength dataSecondUnion.dataUnUserDataLength
        ttPktLen                     dataUnUserDataLength;
    } dataSecondUnion;
    union
    {
/*
 * packet source IP address (incoming packet). Stored after IP reassembly
 * if any.
 */
#define dataSrcIpAddress         dataThirdUnion.dataUnSrcIpAddress
        tt4IpAddress                 dataUnSrcIpAddress;
/*
 * Begin byte offset of fragment in fragmented IP datagram (incoming packet)
 * NOTE that it is OK to overlay with dataUnSrcIpAddress, since the
 * IP incoming code will store the source IP address after IP reassembly.
 */
#define dataFragBBOffset         dataThirdUnion.dataUnFragBBOffset
        ttPktLen                     dataUnFragBBOffset;

    } dataThirdUnion;
    union
    {
/*
 * packet specific destination IP address (incoming packet). Stored after
 * IP reassembly if any.
 * packet specific destination IP address for ARP resolution (outgoing
 * packet).
 * Packet source IP address as specified by the user in tfSendToFrom (outgoing
 * packet).
 */
#define dataSpecDestIpAddress    dataFourthUnion.dataUnSpecDestIpAddress
#define dataFromIpAddress        dataFourthUnion.dataUnSpecDestIpAddress
        ttIpAddress                  dataUnSpecDestIpAddress;
/*
 * End byte offset of fragment in fragmented IP datagram (incoming packet)
 * NOTE that it is OK to overlay with dataUnSpecDestIpAddress, since the
 * IP incoming code will store the specific destination IP address after
 * IP reassembly if any.
 */
#define dataFragEBOffset         dataFourthUnion.dataUnFragEBOffset
        ttPktLen                     dataUnFragEBOffset;
    } dataFourthUnion;
    union
    {
/*
 * Device corresponding to IP destination address in incoming packet
 * This is a computed value based on the IP destination address of
 * the packet in the IP incoming code. It may (or may not) match the
 * incoming device.
 */
#define dataDestDevPtr      dataFifthUnion.dataUnDestDevPtr
        struct tsDeviceEntry TM_FAR * dataUnDestDevPtr;
/*
 * The TCP header pointer overlays
 * this field for temporary storage while going through the state machine
 * while processing an incoming TCP packet.
 */
#define dataTcpHdrPtr       dataFifthUnion.dataUnTcpHdrPtr
        struct tsTcpHeader   TM_FAR * dataUnTcpHdrPtr;
/*
 * Pointer to user free function used with the User scattered send.
 */
#define dataUserFreeFuncPtr dataFifthUnion.dataUnUserFreeFuncPtr
        ttUserFreeFuncPtr             dataUnUserFreeFuncPtr;
    } dataFifthUnion;

#if defined (TM_DEV_SEND_OFFLOAD) && defined (TM_DEV_RECV_OFFLOAD)
#define dataDevSendOffload  dataSendRecvOffloadUnion.dataUnDevSendOffload
#define dataDevRecvOffload  dataSendRecvOffloadUnion.dataUnDevRecvOffload
    union
    {
        ttDevSendOffloadStruct      dataUnDevSendOffload;
        ttDevRecvOffloadStruct      dataUnDevRecvOffload;
    } dataSendRecvOffloadUnion;
#else /* ! (defined (TM_DEV_SEND_OFFLOAD && defined (TM_DEV_RECV_OFFLOAD))*/
#ifdef TM_DEV_SEND_OFFLOAD
    ttDevSendOffloadStruct          dataDevSendOffload;
#endif /* TM_DEV_SEND_OFFLOAD */
#ifdef TM_DEV_RECV_OFFLOAD
    ttDevRecvOffloadStruct          dataDevRecvOffload;
#endif /* TM_DEV_SEND_OFFLOAD */
#endif /* ! (defined (TM_DEV_SEND_OFFLOAD) && defined (TM_DEV_RECV_OFFLOAD))*/
} ttSharedData;

typedef ttSharedData TM_FAR * ttSharedDataPtr;


/* Treck buffer to convey information about a protocol packet.
 * Only one owner for a tsPacket. Always associated with a ttSharedData
 * structure. Passed as a parameter between different protocol layers. Used
 * to queue packets in different queues (such as send queues receive queues,
 * reassemble (TCP or IP) queues, arp resolution queues).
 */

typedef struct  tsPacket
{
    ttUserPacket                    pktUserStruct;
/* Next tsPacket for scattered data, recycle list  */
#define pktLinkNextPtr              pktUserStruct.pktuLinkNextPtr
/* Data in ttSharedData */
#define pktLinkDataPtr              pktUserStruct.pktuLinkDataPtr
/* Size of data in the corresponding ttSharedData structure */
#define pktLinkDataLength           pktUserStruct.pktuLinkDataLength
/* Total packet length (of chained scattered data)*/
#define pktChainDataLength          pktUserStruct.pktuChainDataLength
/*
 * Number of links linked to this one (not including this one). Used
 * for Scattered send, to limit the number of scattered buffers to
 * what the device driver can support.
 */
#define pktLinkExtraCount           pktUserStruct.pktuLinkExtraCount
#if defined(TM_DEV_SEND_OFFLOAD) || defined(TM_DEV_RECV_OFFLOAD)
#define pktDevOffloadPtr            pktUserStruct.pktuDevOffloadPtr
#endif /* TM_DEV_SEND_OFFLOAD || TM_DEV_RECV_OFFLOAD */
/* Point to corresponding sharable ttSharedData */
    ttSharedDataPtr                 pktSharedDataPtr;
/* Next packet (head of a new datagram in a queue) */
    struct tsPacket TM_FAR *        pktChainNextPtr;
#ifndef TM_INDRV_INLINE_SEND_RECV
/* For non in-line intramachine device driver recv queue */
    struct tsPacket TM_FAR *        pktRecvNextPtr;
#endif /* TM_INDRV_INLINE_SEND_RECV */
/* Device packet came in from or got out */
    struct tsDeviceEntry TM_FAR *   pktDeviceEntryPtr;


/* TCP Time of transmission (Vegas) (sent packet) */
    tt32Bit                         pktTcpXmitTime;
/* User flags: i.e. TM_PKTF_USER_OWNS (for user owned buffers) */
/*
 * User flags: i.e. TM_PKTF_USER_OWNS, TM_PKTF_USER_DEV_OWNS
 * (for user owned buffers)
 */
    tt16Bit                         pktUserFlags;
/* Packet flags one of TM_LL_UNICAST, TM_LL_BROADCAST, TM_LL_UNICAST
 * , plus one of TM_IP_UNICAST_FLAG, TM_IP_BROADCAST_FLAG
 * TM_IP_MULTICAST_FLAG, TM_IP_FORWARD_FLAG, TM_IP_MCAST_FORWARD_FLAG
 * TM_IP_LOCAL_FLAG
 * plus TM_OUTPUT_DEVICE_FLAG, TM_ETHERNET_ADDRESS_FLAG, TM_IP_RTE_FLAG
 */
    tt16Bit                         pktFlags;
/*
 * packet TCP flags for packets in the TCP send queue.
 */
    tt16Bit                         pktFlags2;
    tt16Bit                         pktMhomeIndex;
/* Number of times this packet has been decapsulated. Initially set to zero. */
    tt8Bit                          pktTunnelCount;
    tt8Bit                          pktIpHdrLen;

/*
 * Specifies the network layer type of this packet (IPv4, IPv6, ARP, etc).
 * This replaces the TM_NETWORK_LAYER flags in pktFlags
 */ 
    tt8Bit                          pktNetworkLayer;

    tt8Bit                          pktFiller[1];


} ttPacket;
typedef ttPacket TM_FAR *       ttPacketPtr;
typedef ttPacketPtr TM_FAR *    ttPacketPtrPtr;


typedef void (TM_CODE_FAR * ttTmCBFuncPtr)(
    ttVoidPtr       timerBlockPtr,
    ttGenericUnion  userParm1,
    ttGenericUnion  userParm2);

/*
 * Timer Entry Typedef
 */
typedef struct tsTimer
{
    struct tsTimer TM_FAR *tmrNextTimerPtr;
    ttTmCBFuncPtr          tmrCallBackFunctionPtr;
    ttGenericUnion         tmrUserParm1;
    ttGenericUnion         tmrUserParm2;
    tt32Bit                tmrStartTickCount;
    tt32Bit                tmrInitialTime;
    tt8Bit                 tmrFlags;
    tt8Bit                 tmrCallBackFlag;
    tt8Bit                 tmrRemove;
    tt8Bit                 tmrSuspend;
} ttTimer;

typedef ttTimer    TM_FAR * ttTimerPtr;
typedef ttTimerPtr TM_FAR * ttTimerPtrPtr;

/* Control block for managing timer queues */
typedef struct tsTimerCtrl
{
    ttTimerPtr timerActiveQueuePtr;
    ttTimerPtr timerActiveQTailPtr;
#ifdef TM_LOCK_NEEDED
/* Timer Queue Lock */
    ttLockEntry timerExecuteLockEntry;
    ttLockEntry timerActiveQueueLockEntry;
#endif /* TM_LOCK_NEEDED */
} ttTimerCtrl;

/*
 * NOTE:
 * The message data pointer points to the IP or ARP header
 * For Arp resolved packets:
 * 8 Bytes prior contains the following fields
 * EtherDestination and TYPE when calling lnkSendFuncPtr
 */
typedef int  (TM_CODE_FAR *ttLnkOpenFuncPtr)(void TM_FAR * deviceEntryPtr,
                                             tt16Bit       multiHomeIndex,
                                             int           protocolFamily);
typedef int  (TM_CODE_FAR *ttLnkCloseFuncPtr)(void TM_FAR *deviceEntryPtr);
typedef int  (TM_CODE_FAR *ttLnkSendFuncPtr)(ttPacketPtr packetPtr);
typedef int  (TM_CODE_FAR *ttLnkRecvFuncPtr)(void TM_FAR *deviceEntryPtr,
                                             void TM_FAR *bufferHandle);
typedef int  (TM_CODE_FAR *ttLnkIoctlFuncPtr)(int operation);
typedef int  (TM_CODE_FAR *ttLnkErrorFuncPtr)(int type);
typedef int  (TM_CODE_FAR *ttLnkResolveFuncPtr)(ttPacketPtr packetPtr);
typedef int  (TM_CODE_FAR *ttLnkSendRequestFuncPtr)(ttVoidPtr     deviceEntryPtr,
                                                    tt16Bit       mhomeIndex,
                                                    tt4IpAddress  ipAddress);
typedef int  (TM_CODE_FAR *ttLnkPostFuncPtr)(ttPacketPtr packetPtr);
typedef void (TM_CODE_FAR *ttLnkNotifyFuncPtr)(ttVoidPtr deviceEntryPtr,
                                               int       event);
typedef int  (TM_CODE_FAR *ttLnkMcastFuncPtr)(ttVoidPtr    deviceEntryPtr,
                                              tt4IpAddress ipAddress,
                                              int          command);
typedef int  (TM_CODE_FAR *ttLnkHdrCompressFuncPtr)(ttPacketPtr  packetPtr,
                                                    tt32BitPtr   pktTypePtr,
                                                    ttVoidPtrPtr optPtrPtr,
                                                    tt32Bit      opcode);

/*
 * Link Layer Entry Typedef
 */
typedef struct tsLinkLayerEntry
{
    ttLnkOpenFuncPtr                lnkOpenFuncPtr;
    ttLnkCloseFuncPtr               lnkCloseFuncPtr;
    ttLnkSendFuncPtr                lnkSendFuncPtr;
    ttLnkRecvFuncPtr                lnkRecvFuncPtr;
    ttLnkIoctlFuncPtr               lnkIoctlFuncPtr;
    ttLnkErrorFuncPtr               lnkErrorFuncPtr;
    ttLnkResolveFuncPtr             lnkResolveFuncPtr;


    ttLnkSendRequestFuncPtr         lnkSendRequestFuncPtr;
    ttLnkMcastFuncPtr               lnkMcastFuncPtr;
    ttLnkPostFuncPtr                lnkPostFuncPtr;
    ttLnkNotifyFuncPtr              lnkNotifyFuncPtr;
    ttLnkHdrCompressFuncPtr         lnkHdrCompressFuncPtr;
    struct tsLinkLayerEntry TM_FAR *lnkNextPtr;
    tt16Bit                         lnkLinkLayerProtocol;
    tt8Bit                          lnkPhysBroadAddr
                                        [tm_align(TM_MAX_PHYS_ADDR)];
    tt8Bit                          lnkNameArray
                                        [tm_align(TM_MAX_LINK_LAYER_NAME)];
    tt16Bit                         lnkMtu;  /* Default for the Link Layer */
    tt8Bit                          lnkHeaderLength;
    tt8Bit                          lnkPhysAddrLen;
    tt8Bit                          lnkFiller[2];
} ttLinkLayerEntry;

typedef ttLinkLayerEntry TM_FAR * ttLinkLayerEntryPtr;

/* Network header: Ethernet Header */
typedef struct tsEtherHeader
{
    ttEnetAddress ethDstAddr;
    ttEnetAddress ethSrcAddr;
    tt16Bit       ethType;
} ttEtherHeader;

typedef ttEtherHeader TM_FAR * ttEtherHeaderPtr;

/* Network header: Ethernet Header with VLAN tag fields */
typedef struct tsTaggedEtherHeader
{
    ttEnetAddress ethDstAddr;
    ttEnetAddress ethSrcAddr;
	tt16Bit       ethQTagLengthType;
    tt16Bit       ethQTagControlInfo;
    tt16Bit       ethType;
} ttTaggedEtherHeader;

typedef ttTaggedEtherHeader TM_FAR * ttTaggedEtherHeaderPtr;

/* END of Ethernet network headers */

/* Information structure for dialer, corresponding to one send-expect pair */
typedef struct tsDialerInfo
{

/* pointer to the next entry in the linked list of send-expect pairs */
    struct tsDialerInfo TM_FAR * dialiNextEntryPtr;

/* transmit timeout timer */
    ttTimerPtr                   dialiTimerPtr;

/* string to send to the device  */
    tt8BitPtr                    dialiSendStringPtr;

/* string to expect from the device, indicates sucess */
    tt8BitPtr                    dialiExpectStringPtr;

/* string recieved from device that would indicate error */
    tt8BitPtr                    dialiErrorStringPtr;

/* number of times to retry transmission of sendString */
    tt8Bit                       dialiNumRetries;

/* amount of time in seconds to wait before retransmitting */
    tt8Bit                       dialiTimeout;

    tt8Bit                       dialiFlags;

/* current number of characters matched in the expect string */
    tt8Bit                       dialiExpectMatch;

/* current number of characters match in the error string */
    tt8Bit                       dialiErrorMatch;

    tt8Bit                       dialiPadding[3];

} ttDialerInfo;

typedef ttDialerInfo TM_FAR * ttDialerInfoPtr;

#define TM_DIALER_INFO_NULL_PTR (ttDialerInfoPtr)0

/* Dialer entry structure */
typedef struct tsDialerEntry
{

/*
 * Pointer to old link layer recieve routine.  This is needed because the
 * dialer temporarily overwrites the link layer recieve function pointer so
 * all incoming data on this device is directed at the dialer, not the LL.
 */
    ttLnkRecvFuncPtr             dialLinkRecvFuncPtr;

/* Pointer to old link layer close routine */
    ttLnkCloseFuncPtr            dialLinkCloseFuncPtr;

/* Pointer to user's dialer notification function */
    ttUserLnkNotifyFuncPtr       dialNotifyFuncPtr;

/* Pointer to linked list of send-expect pairs */
    ttDialerInfoPtr              dialInfoPtr;

/* Pointer to dialer open routine */
    ttLnkOpenFuncPtr             dialOpenFuncPtr;

/* The protocol family (IPv6, IPv4) for the link layer (PPP) being opened */    
    int                          dialProtocolFamily;
    
/* Flag indicating success or failure of dialing attempt */
    tt8Bit                       dialSuccess;

/* Flag indicating if the dialer is in the process of closing */
    tt8Bit                       dialCloseFlag;

/* Padding to a multiple of 4-bytes */
    tt8Bit                       dialFiller[2];

} ttDialerEntry;

typedef ttDialerEntry TM_FAR * ttDialerEntryPtr;

#define TM_DIALER_ENTRY_NULL_PTR (ttDialerEntryPtr)0

/* ARP resolution */
typedef struct tsResEntry
{
/* Note: NextPtr need to be first for recycle list */
    struct tsResEntry    TM_FAR *resNextPtr;
    struct tsResEntry    TM_FAR *resPrevPtr;
    ttPacketPtr                  resPacketPtr;
    ttTimerPtr                   resTimerPtr;
    struct tsDeviceEntry TM_FAR *resDeviceEntryPtr;
    tt32Bit                      resRequestTries;
    tt16Bit                      resMhomeIndex;
    tt8Bit                       resValidEntry; /* not used by IPv6 */
    tt8Bit                       resFiller[1];
} ttResEntry;

typedef ttResEntry TM_FAR * ttResEntryPtr;

/* PROXY ARP entry */
typedef struct tsProxyEntry
{
/* Note: NextPtr need to be first for recycle list */
    struct tsProxyEntry    TM_FAR *proxNextPtr;
    struct tsProxyEntry    TM_FAR *proxPrevPtr;
/* Ip address of proxy entry */
    tt4IpAddress                   proxIpAddress;
} ttProxyEntry;

typedef ttProxyEntry TM_FAR * ttProxyEntryPtr;


/*
 * Boot entry type. To store information collected from the BOOPT/DHCP
 * server. Also to keep track of DHCP timers. This can grow as needed when
 * we add more features.
 */
typedef struct tsBtEntry
{
/* Back pointer to the device */
    struct tsDeviceEntry TM_FAR * btDevEntryPtr;
/* For BOOTP requests /DHCP discover/request retransmissions */
    ttTimerPtr                    btRetryTimerPtr;
/* For DHCP lease */
    ttTimerPtr                    btDhcpLeaseTimerPtr;

/* For DHCP manual renew */
    ttTimerPtr                    btDhcpRenewTimerPtr;

#ifdef TM_USE_DHCP_FQDN
    ttCharPtr                     btUserFqdnPtr;
#endif /* TM_USE_DHCP_FQDN */
    ttUserDhcpNotifyFuncPtr       btDhcpNotifyFuncPtr;
    ttUserBtEntry                 btUserEntry;
/* 2nd phase BOOT Server (TFTP server) */
#define btBootSIpAddress          btUserEntry.btuBootSIpAddress
/* Domain name server (for DNS) */
#define btDns1ServerIpAddress     btUserEntry.btuDns1ServerIpAddress
/* Second domain name server (for DNS) */
#define btDns2ServerIpAddress     btUserEntry.btuDns2ServerIpAddress
/* Our (leased) IP address */
#define btYiaddr                  btUserEntry.btuYiaddr
/* Our subnet mask */
#define btNetMask                 btUserEntry.btuNetMask
/* Default router */
#define btDefRouter               btUserEntry.btuDefRouter
/* Default router after lease renewal if different */
#define btLeaseDefRouter          btUserEntry.btuLeaseDefRouter
/* DHCP selected server IP address */
#define btDhcpServerId            btUserEntry.btuDhcpServerId
/* Time stamp of DHCP request */
#define btDhcpRequestTimeStamp    btUserEntry.btuDhcpRequestTimeStamp
/* DHCP Address lease time */
#define btDhcpLeaseTime           btUserEntry.btuDhcpLeaseTime
/* DHCP T1 time */
#define btDhcpT1                  btUserEntry.btuDhcpT1
/* DHCP T2 time */
#define btDhcpT2                  btUserEntry.btuDhcpT2
/* Domain name (64 bytes) (for DNS) */
#define btDomainName              btUserEntry.btuDomainName
/* TFTP server name (64 bytes) (for TFTP download) */
#define btBootSname               btUserEntry.btuBootSname
/* Boot file name (128 bytes) (for TFTP download) */
#define btBootFileName            btUserEntry.btuBootFileName
/* Boot file size (in 512-bytes blocks) (for TFTP download) */
#define btBootFileSize            btUserEntry.btuBootFileSize
/* NBNS name servers (array of IP addresses) */
#define btNetBiosNameServers      btUserEntry.btuNetBiosNameServers
#define btNetBiosNumNameServers   btUserEntry.btuNetBiosNumNameServers
/* seconds since we started sending a BOOTP request/DHCP discover*/
#define btSeconds                 btUserEntry.btuSeconds
#define btDhcpClientIdLength      btUserEntry.btuDhcpClientIdLength
#define btDhcpClientIdPtr         btUserEntry.btuDhcpClientIDPtr
#define btDhcpHostNameLength      btUserEntry.btuDhcpHostNameLength
#define btDhcpHostNamePtr         btUserEntry.btuDhcpHostNamePtr
/* Unique ID that indicates BT_CONF or BT_USER, and indicates index */
    tt32Bit                       btXid;
/* User passed parameters */
    ttUserIpAddress               btUserSetDhcpIpAddress;
    tt8Bit                        btUserSetDhcpClientIdPtr[
                                        ((TM_DHCP_CLIENTID_SIZE + 3) /4) * 4];
    tt8Bit                        btUserSetDhcpHostNamePtr[
                                        ((TM_DHCP_HOSTNAME_SIZE + 3) /4) * 4];
/*
 * index (in either multi-home array (btType == TM_BT_CONF), or
 * user table (btType == TM_BT_USER)
 */
    tt16Bit                       btIndex;
    tt8Bit                        btUserSetDhcpClientIdLength;
    tt8Bit                        btUserSetDhcpHostNameLength;
/* Indicate which parameters the user set */
    tt8Bit                        btUserSetFlags;
    tt8Bit                        btLeaseFlags;
/* DHCP state machine state */
    tt8Bit                        btDhcpState;
/* number of retries */
    tt8Bit                        btRetries;
/* multi-home entry (TM_BT_CONF) or User entry (TM_BT_USER) */
    tt8Bit                        btType;
/* Manual lease renewal retries */
    tt8Bit                        btDhcpRenewRetries;
#ifdef TM_USE_DHCP_FQDN
    tt8Bit                        btUserFqdnLen;
    tt8Bit                        btFqdnFlags;
#define btFqdnStatus              btUserEntry.btuFqdnStatus
#define btFqdnRCode1              btUserEntry.btuFqdnRCode1
#define btFqdnRCode2              btUserEntry.btuFqdnRCode2
#define btServerFqdn              btUserEntry.btuServerFqdn
#define btServerFqdnLen           btUserEntry.btuServerFqdnLen
#endif /* TM_USE_DHCP_FQDN */

} ttBtEntry;

typedef  ttBtEntry    TM_FAR   * ttBtEntryPtr;
typedef  ttBtEntryPtr TM_FAR   * ttBtEntryPtrPtr;


typedef int (TM_CODE_FAR * ttDevBootFuncPtr)( void TM_FAR * deviceEntryPtr,
                                              int           flag,
                                              tt16Bit       multiHomeIndex );

typedef void (TM_CODE_FAR * ttDevBootRecvFuncPtr)(ttPacketPtr  packetPtr,
                                                  ttBtEntryPtr bootEntryPtr);

#ifdef TM_USE_DHCP_FQDN
/* Hold the FQDN main information */
typedef struct tsFqdn
{	
    char            domainName[TM_FQDN_MAX_LEN + 1];
    tt8Bit          domainNameLen;
    tt8Bit          domainFlags;
} ttFqdn;	
#endif /* TM_USE_DHCP_FQDN */

typedef int (TM_CODE_FAR * ttDevRefillPoolFuncPtr)(
                                        ttUserInterface interfaceHandle );

typedef int (TM_CODE_FAR * ttDevEmptyXmitFuncPtr)(
                                        ttUserInterface interfaceHandle );

/*
 * One element in the ring of queued transmit buffers
 */
typedef struct tsRingEntry
{
#ifdef TM_USE_DRV_ONE_SCAT_SEND
    ttUserPacketPtr ringPacketUPtr;
#endif /* TM_USE_DRV_ONE_SCAT_SEND */
    ttCharPtr       ringDataPtr; /* Buffer pointer */
    tt16Bit         ringDataLength;  /* Buffer length */
/* Buffer flag (TM_USER_BUFFER_MORE or TM_USER_BUFFER_LAST) */
    tt16Bit         ringDataFlag;
} ttRingEntry;

typedef ttRingEntry TM_FAR * ttRingEntryPtr;

/*
 * Device driver transmit queue structure
 */
typedef struct tsDevXmitEntry
{
/* Number of slots in the device driver transmit ring of queued buffers */
    tt16Bit            xmitSlots;
/* Number of transmitted bytes in the current packet */
    tt16Bit            xmitBytes;
/* First element in device driver transmit queue */
    ttS16Bit           xmitReadIndex;
/* End (one past last) element in device driver transmit queue */
    ttS16Bit           xmitWriteIndex;
/* TM_RING_EMPTY, TM_RING_FULL, TM_RING_OTHER */
    tt8Bit             xmitFlag;

/* Transmission error in current packet */
    tt8Bit             xmitErrorFlag;
    tt8Bit             xmitFiller[2];
/* Device driver transmit ring of queued buffers */
    ttRingEntry        xmitRing[1]; /* variable length */
} ttDevXmitEntry;

/* device driver transmit queue pointer */
typedef ttDevXmitEntry  TM_FAR * ttDevXmitEntryPtr;


/*
 * TCP sequence numbering (used in ttNatTrigger and ttTcpHeader)
 */

typedef tt32Bit  ttSqn; /* 32-bit sequence number */


/*
 * NAT
 *
 */
typedef struct tsNatTrigger    /* interceptor of inbound or outbound packets */
{
    struct tsNatTrigger TM_FAR  *ntrNextPtr;    /* device's list of triggers */
    struct tsNatTrigger TM_FAR  *ntrSpawnPtr;   /* type SESSION -> type NAPT */
    tt4IpAddress                 ntrIpPrivate;
    tt4IpAddress                 ntrIpPublic;
    tt4IpAddress                 ntrIpRemote;
    tt32Bit                      ntrTtl;             /* TM_NTRTTL_... seconds*/
    ttSqn                        ntrOutSeqDeltaOld;  /* host order           */
    ttSqn                        ntrOutSeqThreshold; /* Private, host order  */
    ttSqn                        ntrOutSeqDeltaNew;  /* host order           */
    ttIpPort                     ntrPortPrivate;     /* aka ntrPortPublicNext*/
    ttIpPort                     ntrPortPublic;      /* aka ntrPortPublicMin */
    ttIpPort                     ntrPortRemote;      /* aka ntrPortPublicMax */
    tt16Bit                      ntrIpIdPrivate;     /* IP ID for fragments  */
    tt16Bit                      ntrIpIdPublic;      /* IP ID for fragments  */
    tt8Bit                       ntrType;            /* TM_NTRTYPE_...       */
    tt8Bit                       ntrFlag;            /* TM_NTRFLAG_...       */
} ttNatTrigger;
typedef ttNatTrigger TM_FAR *ttNatTriggerPtr;

/* Double-duty members of ttNatTrigger:         */
/*                                              */
/*      when ntrType is    when ntrType is      */
/*      NTRTYPE_NAPT       NTRTYPE_...SESSION   */
/*      (host byte order)  (network byte order) */
/*      -----------------  -----------------    */
#define ntrPortPublicNext  ntrPortPrivate
#define ntrPortPublicMin   ntrPortPublic
#define ntrPortPublicMax   ntrPortRemote

typedef int ( TM_CODE_FAR * ttNatFuncPtr)( ttPacketPtrPtr packetPtrPtr,
                                           int            flag );


/*
 * Crypto library, used by IPsec or SSL
 */


#if (defined(TM_SNMP_MIB) || defined(TM_PPP_LQM))
/* IPv6 SNMP MIB, implemented per interface when TM_SNMP_MIB is #define'd. */
typedef struct tsIfStatMib
{
    tt64Bit ipIfStatsHCInReceives;
    tt64Bit ipIfStatsHCInOctets;
    tt32Bit ipIfStatsInHdrErrors;
    tt32Bit ipIfStatsInNoRoutes;
    tt32Bit ipIfStatsInAddrErrors;
    tt32Bit ipIfStatsInUnknownProtos;
    tt32Bit ipIfStatsInTruncatedPkts;
    tt32Bit ipIfStatsInForwDatagrams;
    tt32Bit ipIfStatsReasmReqds;
    tt32Bit ipIfStatsReasmOKs;
    tt32Bit ipIfStatsReasmFails;
    tt32Bit ipIfStatsInDiscards;
    tt32Bit ipIfStatsInDelivers;
    tt32Bit ipIfStatsOutRequests;
    tt32Bit ipIfStatsOutNoRoutes;
    tt32Bit ipIfStatsOutForwDatagrams;
    tt32Bit ipIfStatsOutDiscards;
    tt32Bit ipIfStatsOutFragReqds;
    tt32Bit ipIfStatsOutFragOKs;
    tt32Bit ipIfStatsOutFragFails;
    tt32Bit ipIfStatsOutFragCreates;
    tt64Bit ipIfStatsHCOutTransmits;
    tt64Bit ipIfStatsHCOutOctets;
    tt64Bit ipIfStatsHCInMcastPkts;
    tt64Bit ipIfStatsHCInMcastOctets;
    tt64Bit ipIfStatsHCOutMcastPkts;
    tt64Bit ipIfStatsHCOutMcastOctets;
    tt64Bit ipIfStatsHCInBcastPkts;
    tt64Bit ipIfStatsHCOutBcastPkts;
#define ipIfStatsInReceives      tm_get_64bit_low(ipIfStatsHCInReceives)
#define ipIfStatsInOctets        tm_get_64bit_low(ipIfStatsHCInOctets)
#define ipIfStatsOutTransmits    tm_get_64bit_low(ipIfStatsHCOutTransmits)
#define ipIfStatsOutOctets       tm_get_64bit_low(ipIfStatsHCOutOctets)
#define ipIfStatsInMcastPkts     tm_get_64bit_low(ipIfStatsHCInMcastPkts)
#define ipIfStatsInMcastOctets   tm_get_64bit_low(ipIfStatsHCInMcastOctets)
#define ipIfStatsOutMcastPkts    tm_get_64bit_low(ipIfStatsHCOutMcastPkts)
#define ipIfStatsOutMcastOctets  tm_get_64bit_low(ipIfStatsHCOutMcastOctets)
#define ipIfStatsInBcastPkts     tm_get_64bit_low(ipIfStatsHCInBcastPkts)
#define ipIfStatsOutBcastPkts    tm_get_64bit_low(ipIfStatsHCOutBcastPkts)
} ttIfStatMib;
#endif /* TM_SNMP_MIB or TM_PPP_LQM */



#if (defined(TM_OPTIMIZE_MANY_MHOMES) && !defined(TM_SINGLE_INTERFACE_HOME))
/* used to sort IP addresses to improve performance of IP address lookup in the
   receive-path when many IP aliases are configured on a single interface.
   The SNMP Agent can use this to speed up non-exact match processing
   in tfIpAddrTableEntryGet. */

typedef struct tsIpAddrEntry
{
    tt16Bit devIndex;
    tt16Bit mHomeIndex;
} ttIpAddrEntry;
typedef ttIpAddrEntry TM_FAR * ttIpAddrEntryPtr;

typedef struct tsSortedIpAddrCache
{
#ifdef TM_LOCK_NEEDED
    ttLockEntry         sipcLockEntry;  /* lock for this structure */
#endif /* TM_LOCK_NEEDED */
/* IP address cache */
    ttIpAddrEntryPtr    sipcIpAddrArray;
/* Count of IP address entries in cache */
    tt32Bit             sipcIpAddrArrayCount;
/* Pointer to entry last read in the cache */
    ttIpAddrEntryPtr    sipcLastReadPtr;
/* Current Serial number to keep track of cache updates */
    tt32Bit             sipcSerialNum;
/* Copy of Serial number at last cache reading */
    tt32Bit             sipcSerialNumAtLastRead;
} ttSortedIpAddrCache;

typedef ttSortedIpAddrCache TM_FAR * ttSortedIpAddrCachePtr;
#endif /* TM_OPTIMIZE_MANY_MHOMES and not TM_SINGLE_INTERFACE_HOME */

/*
 * Device Entry Typedef
 */
typedef struct tsDeviceEntry
{
    struct tsDeviceEntry TM_FAR *devNextDeviceEntry;
/* Device Function Entries */
    ttDevOpenCloseFuncPtr   devOpenFuncPtr;
    ttDevOpenCloseFuncPtr   devCloseFuncPtr;
    ttDevSendFuncPtr        devSendFuncPtr;
    ttDevSendFuncPtr        devSavedSendFuncPtr;
#ifdef TM_USE_DRV_ONE_SCAT_SEND
    ttDevOneScatSendFuncPtr devOneScatSendFuncPtr;
    ttDevOneScatSendFuncPtr devSavedOneScatSendFuncPtr;
#endif /* TM_USE_DRV_ONE_SCAT_SEND */
#ifdef TM_USE_DRV_SCAT_RECV
    ttDevScatRecvFuncPtr    devScatRecvFuncPtr;
#endif /* TM_USE_DRV_SCAT_RECV */
    ttDevRecvFuncPtr        devRecvFuncPtr;
    ttDevFreeRecvFuncPtr    devFreeRecvFuncPtr;
    ttDevIoctlFuncPtr       devIoctlFuncPtr;
    ttDevEmptyXmitFuncPtr   devEmptyXmitFuncPtr;
    ttDevRefillPoolFuncPtr  devRefillPoolFuncPtr;
    ttDevGetPhysAddrFuncPtr devGetPhysAddrFuncPtr;

/* BOOTP or DHCP configure entry point */
    ttDevBootFuncPtr        devBootFuncPtr;
/* BOOTP or DHCP recv call back function */
    ttDevBootRecvFuncPtr    devBootRecvFuncPtr;
/* User notify function for BOOTP/DHCP configuration completion */
    ttDevNotifyFuncPtr      devNotifyFuncPtr;

/* Head of recv queue (Intra machine, or loop back only) */
    ttPacketPtr             devReceiveQueuePtr;
/* Pointer to last packet in recv queue (Intra machine, or loop back only) */
    ttPacketPtr             devRcvLastQueuePtr;
/* Head of send queue (for packets waiting to be sent) */
    ttPacketPtr             devSendQueuePtr;
/* Last packet in the send queue */
    ttPacketPtr             devSendQueueLastPtr;
/* Current packet being sent in the device driver */
    ttPacketPtr             devSendQueueCurPtr;
/*
 * Pointer to next packet in the send queue, that the transmit task need
 * to send. Used only if transmit task option is set.
 */
    ttPacketPtr             devXmitTaskPacketPtr;
/*
 * Pointer to device driver transmit queue (for buffers waiting for room)
 * Non null only if user set the corresponding device option.
 */
    ttDevXmitEntryPtr       devXmitEntryPtr;
/* Current pointer to incoming data in interface loop back driver */
    tt8BitPtr               devReceiveDataPtr;
/* Pointer to device receive pool, if user created one */
    ttVoidPtr               devPoolHeadPtr;
/* ARP resolution */
    ttResEntryPtr           devResEntryPtr;

/*
 * For BOOTP, DHCP. First slot is reserved to configure the device in
 * slot 0.
 */
#define devBtEntryPtr       devBtConfEntryTablePtr[0]
/* Automatic DHCP/BOOTP configuration entries */
    ttBtEntryPtrPtr         devBtConfEntryTablePtr;
/*
 * Pointer to a table containing pointers for user Bootp/DHCP requests.
 * To be used by the user (either to obtain IP addresses for its own
 * use or for non automatic configuration of the device.)
 */
    ttBtEntryPtrPtr         devBtUserEntryTablePtr;
#ifdef TM_IGMP
/* Pointer to list of IP multicast addresses joined on this interface */
    ttVoidPtr               devIgmpPtr;
/* Timer activated when an IGMP V1 query is received */
    ttTimerPtr              devIgmpV1TmrPtr;
#endif /* TM_IGMP */

#if (defined(TM_IGMP) || defined(TM_USE_IPV6))
/* Pointer to array of owner counts for Ethernet mcast entries */
    tt16BitPtr              devLnkMcastOwnerCountPtr;
#endif /* TM_IGMP || TM_USE_IPV6 */
    
    ttLinkLayerEntryPtr     devLinkLayerProtocolPtr;


/* NAT fields, used by trnat.c */
    ttNatFuncPtr            devNatFuncPtr;
    ttNatTriggerPtr         devNatTriggerHead;
    ttTimerPtr              devNatTimer;


/*
 * Only used in a virtual tunnel device.  Pointer to the original device
 * entry.  Currently used for source address selection.
 */ 
    struct tsDeviceEntry TM_FAR * devTunnelEntryPtDevPtr;
    
#define devStateVectorPtr   devLinkUnion.devuStateVectorPtr
#define devLnkMcastPtr      devLinkUnion.devuLnkMcastPtr
    union
    {
/*
 * State vector Used by PPP or SLIP
 */
        ttVoidPtr           devuStateVectorPtr;
/* Pointer to list of ethernet multicast addresses */
        ttVoidPtr           devuLnkMcastPtr;
    } devLinkUnion;
/*
 * Pointer to dialer structure:  if this pointer is non null, the
 * dialer is enabled.
 */
    ttDialerEntryPtr        devDialerEntryPtr;
    ttVoidPtr               devDriverPtr;
    tt32Bit                 devBytesNotified;
    tt32Bit                 devPacketsNotified;
#if (defined(TM_SNMP_MIB) || defined(TM_PPP_LQM))
    ttIfStatMib             dev4Ipv4Mib;
#endif /* TM_SNMP_MIB */

#ifdef TM_SNMP_MIB
    tt32Bit                 devLastChange;
#define devInOctets         tm_get_64bit_low(dev4Ipv4Mib.ipIfStatsHCInOctets)
#define devInUnknownProtos  dev4Ipv4Mib.ipIfStatsInUnknownProtos
#define devOutDiscards      dev4Ipv4Mib.ipIfStatsOutDiscards
    tt32Bit                 devOutErrors;
    tt32Bit                 devOutQLen;
#define devOutOctets        dev4Ipv4Mib.ipIfStatsOutOctets
#endif /* TM_SNMP_MIB */

#if (defined(TM_SNMP_MIB) || defined(TM_PPP_LQM))
    tt32Bit                 devInUcastPkts;
#define devInMulticastPkts  tm_get_64bit_low(dev4Ipv4Mib.ipIfStatsHCOutMcastPkts)
#define devInBroadcastPkts  tm_get_64bit_low(dev4Ipv4Mib.ipIfStatsHCInBcastPkts)
#define devInDiscards       dev4Ipv4Mib.ipIfStatsInDiscards
    tt32Bit                 devInErrors;
    tt32Bit                 devOutUcastPkts;
#define devOutMulticastPkts dev4Ipv4Mib.ipIfStatsOutMcastPkts
#define devOutBroadcastPkts dev4Ipv4Mib.ipIfStatsOutBcastPkts
#endif /* (TM_SNMP_MIB or TM_PPP_LQM) */

#ifdef TM_PPP_LQM
    tt32Bit         devLqmOutLqrs;      /* RFC-1989: OutLQRs counter */
    tt32Bit         devLqmInLqrs;       /* RFC-1989: InLQRs counter */
    tt32Bit         devLqmInGoodOctets; /* RFC-1989: InGoodOctets counter */
    tt32Bit         devLqmOutOctets;    /* RFC-1989: ifOutOctets counter */
#endif /* TM_PPP_LQM */


/* IP address/netMask (multihoming) */
#ifdef TM_SINGLE_INTERFACE_HOME
    tt4IpAddress            devIpAddr;
    tt4IpAddress            devNetMask;
    tt4IpAddress            devDestIpAddr; /* for SLIP/PPP */

#ifdef TM_USE_VCHAN
/* Virtual channel */
    tt32Bit                 devVChan;
#endif /* TM_USE_VCHAN */

#define devDBroadIpAddr     devDestIpAddr   /* for LAN, directed broadcast */
#define devPt2PtPeerIpAddr  devDestIpAddr   /* for PPP or SLIP */
#else /* ! TM_SINGLE_INTERFACE_HOME */
    tt4IpAddress            devIpAddrArray[TM_MAX_IPS_PER_IF];
    tt4IpAddress            devNetMaskArray[TM_MAX_IPS_PER_IF];
    tt8Bit                  dev4PrefixLenArray[tm_align(TM_MAX_IPS_PER_IF)];
/* Flag per IP address. Ip address configured or not. */
    tt8Bit                  devIpConfFlagArray[tm_align(TM_MAX_IPS_PER_IF)];
/* for SLIP/PPP, other side's IP address in slot 0. See below. IPv4 uses
   this to precalculate/cache the directed broadcast address associated with
   each unicast IP address configured on the interface. */
    tt4IpAddress            devDestIpAddrArray[TM_MAX_IPS_PER_IF];
#if (defined(TM_OPTIMIZE_MANY_MHOMES) && !defined(TM_SINGLE_INTERFACE_HOME))
    ttSortedIpAddrCache     devSortedDBroadIpAddrCache;
#endif /* TM_OPTIMIZE_MANY_MHOMES and not TM_SINGLE_INTERFACE_HOME */
/* For LAN, directed broadcast for the interface */
#define devDBroadIpAddrArray devDestIpAddrArray
/* for PPP */
#define devPt2PtPeerIpAddr   devDestIpAddrArray[0]

#ifdef TM_USE_VCHAN
/* Per mhome virtual channel IDs for IPv4 */
    tt32Bit                 devVChanArray[TM_MAX_IPS_PER_IF];
#endif /* TM_USE_VCHAN */
#endif /* ! TM_SINGLE_INTERFACE_HOME */

    ttEventEntry            devRecvEvent;
    ttEventEntry            devSentEvent;
    ttEventEntry            devXmitEvent;

#ifdef TM_LOCK_NEEDED
    ttLockEntry             devLockEntry;
    ttLockEntry             devDriverLockEntry;

/* NAT fields, used by trnat.c */
    ttLockEntry             devNatLockEntry;
#endif /* TM_LOCK_NEEDED */

    int                     devScatterExtraCount;
    int                     devErrorCode;
/* Size of devBtUserEntryTablePtr */
    int                     devBtUserMaxEntries;
/*
 * devIndex: one unique value per added interface (tfAddInterface).
 * devIndex used to identify the interface (for SNMP). The SNMP ifIndex
 * will be computed as (devIndex * TM_MAX_IPS_PER_IF) + mHomeIndex
 */
    int                     devIndex;
/* number of postponed tfSendCompleteInterface() */
    int                     devSendCompltCount;
#ifdef TM_USE_DRV_ONE_SCAT_SEND
/* number of postponed tfSendCompletePacketInterface() */
    int                          devSendCompltPacketCount;
#endif /* TM_USE_DRV_ONE_SCAT_SEND */

    tt8Bit                  devPhysAddrArr[tm_align(TM_MAX_PHYS_ADDR)];
    tt8Bit                  devNameArray[tm_align(TM_MAX_DEVICE_NAME)];

/*
 * Device open, Multicast enabled, IP Forwarding enabled, IP Directed
 * broadcast forwarding enabled, IP Mcast forwarding enabled, write
 * scattered enabled. BOOTP, DHCP configurations.
 */
    tt16Bit                 devFlag;
    tt16Bit                 devMtu; /*Setable by the User */

/* NAT fields, used by trnat.c */
    tt16Bit                 devNatOpt;
    tt16Bit                 devNatNumTriggers;
/* Fields for TCP segmentation/checksum offload */
#if defined(TM_DEV_SEND_OFFLOAD) || defined(TM_DEV_RECV_OFFLOAD)
    tt16Bit                 devOffloadFlags;
    tt16Bit                 devOffloadPadding;
#endif /* TM_DEV_SEND_OFFLOAD || TM_DEV_RECV_OFFLOAD */
#ifdef TM_DEV_SEND_OFFLOAD
    tt32Bit                 devOffloadMaxSize;
#endif /* TM_DEV_SEND_OFFLOAD */
    tt16Bit                 devTxAlign;

/*
 * Settable by the user with tfInterfaceSetOptions. Copy incoming packet
 * from the network whose sizes are below this configurable threshold
 */
    tt16Bit                 devRecvCopyThreshHold;
/* 
 * Receive data length for loopback interfaces (trloop.c, trindrv.c) 
 * when not using one single call driver send, and not using driver
 * scattered recv.
 */
#define devReceiveDataLength          devFirstUnion.devUnReceiveDataLength
/* 
 * Settable by the user with tfInterfaceSetOptions, when using device
 * driver scattered recv. 
 * Partially copy incoming packet up to this value, if that much data
 * is not contiguous at the head of the packet.
 * Default value is TM_DEV_DEF_RECV_CONT_HDR_LENGTH
 */
#define devRecvContiguousHdrLength  \
                             devFirstUnion.devUnRecvContHdrLength
    union 
    {
        tt16Bit                      devUnReceiveDataLength;
        tt16Bit                      devUnRecvContHdrLength;
    } devFirstUnion;
#if (defined(TM_IGMP) || defined(TM_USE_IPV6))
/*
 * Number of Ethernet multicast entries used for specific multicast IP
 * addresses
 */
    tt16Bit                 devLnkMcastEntries;
/* Number of multicast entries that are currently allocated */
    tt16Bit                 devLnkMcastAllocEntries;
#endif /* TM_IGMP || TM_USE_IPV6 */

/*
 * Multihome index of the tunnel entry point. Only used in a virtual tunnel
 * device.
 */ 
    tt16Bit                 devTunnelEntryPtMhome;
    tt16Bit                 devFlag2; 
/*
 * added devLinkLayerProtocol, so that we do not have to derefence link
 * layer entry to get this info.
 */
    tt16Bit                 devLinkLayerProtocol;
    
#ifdef TM_SINGLE_INTERFACE_HOME
    tt8Bit                  devIpConfFlag;
    tt8Bit                  dev4PrefixLen;
#else /* ! TM_SINGLE_INTERFACE_HOME */

/*
 * Biggest index + 1 of configured multihome entries:  between 0 and
 * TM_MAX_IPS_PER_IF
 */
    tt16Bit                 devMhomeUsedEntries;
#endif /* ! TM_SINGLE_INTERFACE_HOME */

    tt8Bit                  devPhysAddrLength;
/*
 * added devLinkHeaderLength, so that we do not have to derefence link
 * layer entry to get this info.
 */
    tt8Bit                  devLinkHeaderLength;

/* Initial BOOTP/DHCP request timeout value. */
    tt8Bit                  devBtInitTimeoutIntv;

/* Maximum number of BOOTP/DHCP retries */
    tt8Bit                  devBtMaxRetries;

#if (defined(TM_IGMP) || defined(TM_USE_IPV6))
/* Number of increasing allocations */
    tt8Bit                  devLnkMcastAllocations;
/* Number of requests to add all multicast addresses */
    tt8Bit                  devLnkMcastAllAddresses;
/*
 * IGMP through that interface should use IGMP v1
 * (Not used for IPv6, but needed for padding)
 */
    tt8Bit                  devIgmpV1Flag;
    tt8Bit                  devIgmpFiller[1];
#endif /* TM_IGMP || TM_USE_IPV6 */

#ifdef TM_MULTIPLE_CONTEXT
    tt32Bit                 devNetworkId;
#endif /* TM_MULTIPLE_CONTEXT */

} ttDeviceEntry;

typedef  ttDeviceEntry TM_FAR   * ttDeviceEntryPtr;

/* Network header: ARP Header */
typedef struct tsArpHeader
{
    tt16Bit     arpHardware;
    tt16Bit     arpProtocol;
    tt8Bit      arpHardwareLength;
    tt8Bit      arpProtocolLength;
    tt16Bit     arpOperation;
    tt8Bit      arpSendPhyAddress[TM_ETHER_ADDRESS_LENGTH];
    tt8Bit      arpSendNetAddress[TM_IP_ADDRESS_LENGTH];
    tt8Bit      arpTargetPhyAddress[TM_ETHER_ADDRESS_LENGTH];
    tt8Bit      arpTargetNetAddress[TM_IP_ADDRESS_LENGTH];
} ttArpHeader;

typedef ttArpHeader TM_FAR *ttArpHeaderPtr;

/* END of ARP network header */


/* Network header: UDP Header */
typedef struct  tsUdpHeader
{
    ttIpPort udpSrcPort; /* Source port of the UDP datagram */
    ttIpPort udpDstPort; /* Destination port of the UDP datagram */
    tt16Bit  udpLength; /* length in bytes of the UDP datagram */
/* One's complement checksum of UDP datagram+ pseudo_header */
    tt16Bit  udpChecksum;
} ttUdpHeader;

typedef ttUdpHeader TM_FAR * ttUdpHeaderPtr;

/* END of UDP network header */

/* Network header: ICMP Header */
typedef struct  tsIcmpHeader
{
    tt8Bit  icmType; /* ICMP type field */
    tt8Bit  icmCode; /* ICMP code field */
    tt16Bit icmChecksum; /* ICMP checksum */
    union
    {
        tt32Bit icmUnused32; /* ICMP Header unused second 32-bit word */
        tt32Bit icmRedirectRouterAddress; /* REDIRECT router address */
        tt8Bit  icmParamProblemOffset; /* ICMP parameter problem offset */
        struct
        {
            tt16Bit icmIdent; /* ICMP header request/reply identifier */
            tt16Bit icmSeqno; /* ICMP header request/reply sequence number */
        } icmRequestReplyStruct; /* for Request/Reply ICMP packets */
        struct
        {
            tt16Bit icmUnused16;
            tt16Bit icmNxtHopMtu; /* Next Hop MTU for path MTU discovery */
        } icmPathMtuStruct; /* Path MTU discovery structure */
        struct
        {
/* Number of router entries (in data portion) */
            tt8Bit  icmNumAddrs;
            tt8Bit  icmAddrEntrySize; /* Address entry size in 32-bit words */
            tt16Bit icmLifetime; /* TTL of the route, 1800 in seconds (30mn)*/
        } icmRtrDiscStruct1; /* Router discovery structure */
    } icmSecondLongWordUnion; /* ICMP header second 32-bit word */
    union
    {
        tt4IpAddress    icmAddressMask; /* ICMP address mask reply */
        struct
        {
            tt32Bit icmOriginate; /* ICMP originate timestamp */
            tt32Bit icmReceive; /* ICMP receive timestamp */
            tt32Bit icmTransmit; /* ICMP transmit timestamp */
        } icmTimeStampStruct; /* ICMP time stamp structure */
        struct
        {
/* router address. Overlays icmAddressMask */
            tt4IpAddress    icmRouterAddress;
            tt32Bit         icmRouterPrefLevel; /* rtr preference level (0) */
        } icmRtrDiscStruct2; /* ICMP router discovery structure (data) */
        ttIpHeader  icmIpHeader; /* ICMP error packet data: IP header */
        tt8Bit      icmpOptionalData[8]; /* Echo request/reply data */
    } icmDataUnion; /* ICMP Packet data (after first two 32-bit words) */
} ttIcmpHeader;

typedef ttIcmpHeader TM_FAR * ttIcmpHeaderPtr;

/* Second word union defines */
/* Unused second word */
#define icmSUUnused     icmSecondLongWordUnion.icmUnused32
/* Identification field */
#define icmSUIdent      icmSecondLongWordUnion.icmRequestReplyStruct.icmIdent
/* Sequence number field */
#define icmSUSeqno      icmSecondLongWordUnion.icmRequestReplyStruct.icmSeqno
/* REDIRECT router address */
#define icmSURedGwy     icmSecondLongWordUnion.icmRedirectRouterAddress
/* ICMP Parameter problem offset */
#define icmSUParmProbl  icmSecondLongWordUnion.icmParamProblemOffset
/* Router discovery number of addresses*/
#define icmSURdiscNumAddrs \
                    icmSecondLongWordUnion.icmRtrDiscStruct1.icmNumAddrs
/* Router discovery address size (in short words) */
#define icmSURdiscAddrSiz \
                    icmSecondLongWordUnion.icmRtrDiscStruct1.icmAddrEntrySize
/* Router discovery Time to live */
#define icmSURdiscLife   icmSecondLongWordUnion.icmRtrDiscStruct1.icmLifetime
/* Next hop MTU for path MTU discovery */
#define icmSUNxtHopMtu  icmSecondLongWordUnion.icmPathMtuStruct.icmNxtHopMtu
/* Data union defines */
/* Address mask */
#define icmDUAddrMask   icmDataUnion.icmAddressMask
/* Ip header in ICMP error packets */
#define icmDUIpHeader   icmDataUnion.icmIpHeader
#define icmDURdiscGwy   icmDataUnion.icmRtrDiscStruct2.icmRouterAddress
#define icmDURdiscPref  icmDataUnion.icmRtrDiscStruct2.icmRouterPrefLevel
/* Optional data for echo replies */
#define icmDUData       icmDataUnion.icmpOptionalData

/* END of ICMP network header */

/* tlHeader overlays transport layer header. Icmp header */

typedef struct tsTlHeader
{
    union
    {
        struct
        {
#define tlSport                 tlHeaderUnion.tlUdpTcpPortStruct.tluSport
            ttIpPort tluSport;    /* UDP/TCP source port */
#define tlDport                 tlHeaderUnion.tlUdpTcpPortStruct.tluDport
            ttIpPort tluDport;    /* UDP/TCP destination port */
        } tlUdpTcpPortStruct;
        struct
        {
#define tlIcmType               tlHeaderUnion.tlIcmpStruct.tluIcmType
            tt8Bit  tluIcmType; /* ICMP type field */
#define tlIcmCode               tlHeaderUnion.tlIcmpStruct.tluIcmCode
            tt8Bit  tluIcmCode; /* ICMP code field */
            tt16Bit tluIcmChecksum; /* ICMP checksum */
#define tlIcmIdent              tlHeaderUnion.tlIcmpStruct.tluIcmIdent
            tt16Bit tluIcmIdent; /* ICMP header request/reply identifier */
#define tlIcmSeqno              tlHeaderUnion.tlIcmpStruct.tluIcmSeqno
            tt16Bit tluIcmSeqno; /* ICMP header request/reply sequence number */
        } tlIcmpStruct;
    } tlHeaderUnion;
} ttTlHeader;

typedef ttTlHeader TM_FAR * ttTlHeaderPtr;


/*
 * ttIpFHEntry is a fragment head. IP fragment packets belonging to the
 * same IP datagram are queued to this list via the ipfPacketNextPtr
 * A fragment head is linked to the list of all fragments heads via the
 * ipfHeadNextPtr.
 */
typedef struct  tsIpFHEntry
{
    struct tsIpFHEntry              TM_FAR * ipfHeadNextPtr;
    ttPacketPtr                              ipfPacketNextPtr;
    tt4IpAddress                             ipfSrcAddr;
    tt4IpAddress                             ipfDestAddr;
/* Size withoug IP headers of queued IP datagram */
    tt32Bit                                  ipfSize;
    tt16Bit                                  ipfId;
    tt8Bit                                   ipfTtl;
    tt8Bit                                   ipfUlp;
} ttIpFHEntry;

typedef ttIpFHEntry TM_FAR * ttIpFHEntryPtr;

/*
 * IGMP entry for a multicast host group that the user has joined on an
 * interface.
 */
typedef struct tsIgmpEntry
{
    struct tsIgmpEntry TM_FAR * igmpNextPtr;
    struct tsIgmpEntry TM_FAR * igmpPrevPtr;
    ttDeviceEntryPtr            igmpDevPtr;
/* Multicast host group joined */
    tt4IpAddress                igmpHostGroup;
/* Timer (active in delaying member state) */
    ttTimerPtr                  igmpTimerPtr;
/* Mhome index corresponding to interface IP address configured by the user */
    tt16Bit                     igmpMhomeIndex;
    tt16Bit                     igmpOwnerCount;
/* idle member, or delaying member */
    tt8Bit                      igmpState;
/* Flag to indicate that we were the last host to send a report */
    tt8Bit                      igmpFlag;
    tt8Bit                      igmpPad[2];
} ttIgmpEntry;

typedef ttIgmpEntry TM_FAR * ttIgmpEntryPtr;


/* Multi home function pointer. Used in all interface matching functions */
typedef  int  (*ttMhomeFunctPtr)(ttDeviceEntryPtr devPtr,
                                 tt4IpAddress     ipAddr,
                                 tt16BitPtr       mhomeIndexPtr);

/*
 * This is the UDP/TCP pseudo-header structure used to compute the UDP/TCP
 * checksum. We have changed order of entries so as to overlay the IP
 * header exactly, since changing the order of 16-bit quantities in a
 * header will not affect checksum computation. This way we avoid
 * copying the source and destination addresses, and protocol field.
 * (We can use them right from the IP header.)
 */
/* Network header: TCP/UDP Pseudo-header */
typedef struct  tsPseudoHeader
{
    tt8Bit       pshZero; /* Must be zero for checksum computation */
    tt8Bit       pshProtocol; /* ULP protocol (17 for UDP, 6 for TCP) */
    tt16Bit      pshLayer4Len;/* UDP datagram or TCP segment length in bytes */
    tt4IpAddress pshSrcIpAddr; /* IP source address */
    tt4IpAddress pshDstIpAddr; /* IP destination address */
} ttPseudoHeader;
/* END of TCP/UDP network header */

typedef ttPseudoHeader TM_FAR * ttPseudoHeaderPtr;

/* Radix node structure
 * rtnNextPtr   Pointer to next leaf in the list of non host leaves (for
 *              network match searches)
 * rtnParentPtr Pointer to parent node.
 * rtnFlags     Flag indicating whether this is a leaf or a node, whether
 *              this is the root node, whether this node is in the tree,
 *              and whether a leaf node is a host leaf. TM_RTN_NODE,
 *              TM_RTN_ROOT, TM_RTN_IN, TM_RTN_HOST.
 * rtnFiller    padding for alignment
 * rtnBitOffset Bit offset of the bit to be tested for an internal Node.
 *              For a normal leaf it is the length of the network mask
 *              (number of non zero bits)
 * rtnLeafNodeUnion    Union of a leaf and internal node structures
 *   rtnLeafStruct   Leaf specific fields
 *     rtnKey             key (destination IP address) of a leaf
 *     rtnMask            network mask of a leaf
 *     rtnDuplicateKeyPtr Pointer to a leaf that has a duplicate key
 *     rtnPrevPtr         Pointer to a previous leaf in the list of non host
 *                        leaves (for network match searches)
 *   rtnNodeStruct   Internal Node specific fields
 *     rtnBitMask  IP address mask of the bit to be tested (to go left or
 *                 right).  For example to test the 0 bit offset of the IP
 *                 address, the mask would be 0x80000000, to test the 31 bit
 *                 offset of the IP address, the mask would be 0x1.
 *     rtnLeftPtr  Left child for an internal node
 *     rtnRightPtr Right child for an internal node
 */
typedef struct  tsRtRadixNode
{
/* Note: NextPtr need to be first for recycle list */
    struct tsRtRadixNode         TM_FAR *rtnNextPtr; /* need to be first */
    struct tsRtRadixNode         TM_FAR *rtnParentPtr;
/* Bit offset for a node structure (0-127 for IPV6, 0-31 for IPV4 */
    tt16Bit                              rtnBitOffset;
/* Mask prefix length for a leaf structure (0-128 for IPV6, 0-32 for IPV4) */
#define rtnPrefixLength rtnBitOffset
    tt8Bit                               rtnFlags;
    tt8Bit                               rtnFiller1;
    union
    {
        struct
        {
            ttIpAddress                  rtnKey;
/* 
 * For IPV4 mode only, just the 32-bit mask of the IP address.
 */
            tt4IpAddress                 rtnMask;
            struct tsRtRadixNode TM_FAR *rtnDuplicateKeyPtr;
            struct tsRtRadixNode TM_FAR *rtnPrevPtr;
        } rtnLeafStruct;
        struct
        {
            tt4IpAddress                 rtnBitMask;
            struct tsRtRadixNode TM_FAR *rtnLeftPtr;
            struct tsRtRadixNode TM_FAR *rtnRightPtr;
        } rtnNodeStruct;
    } rtnLeafNodeUnion;
} ttRtRadixNode;

typedef ttRtRadixNode TM_FAR * ttRtRadixNodePtr;

/* Shorthands into tsRtRadixNode sub-unions/sub-structures */
#define rtnLSKey            rtnLeafNodeUnion.rtnLeafStruct.rtnKey
#define rtnLSMask           rtnLeafNodeUnion.rtnLeafStruct.rtnMask
#define rtnLS6MaskIndex     rtnLeafNodeUnion.rtnLeafStruct.rtn6MaskIndex
#define rtnLS6MaskPrefix    rtnLeafNodeUnion.rtnLeafStruct.rtn6MaskPrefix
#define rtnLSDupKeyPtr      rtnLeafNodeUnion.rtnLeafStruct.rtnDuplicateKeyPtr
#define rtnLSPrevPtr        rtnLeafNodeUnion.rtnLeafStruct.rtnPrevPtr
#define rtnNSBitMask        rtnLeafNodeUnion.rtnNodeStruct.rtnBitMask
#define rtnNS6LIndex        rtnLeafNodeUnion.rtnNodeStruct.rtn6LongIndex
#define rtnNS6LBitOffset    rtnLeafNodeUnion.rtnNodeStruct.rtn6LongBitOffset
#define rtnNSLeftPtr        rtnLeafNodeUnion.rtnNodeStruct.rtnLeftPtr
#define rtnNSRightPtr       rtnLeafNodeUnion.rtnNodeStruct.rtnRightPtr

/*
 * Routing entry host gateway union. Used to store either:
 * . the device IP address for a local route,
 * . the Ethernet address for an ARP entry, or multicast route, or broadcast
 *   route
 * . the gateway (next hop) IP address for an indirect route.
 */
typedef union tuRteHostGway
{
    struct
    {
/* device IP address for Local routing entry */
        ttIpAddress         rteDevIpAddress;
    } rteLocalNetStruct;
    struct
    {
/* the physical address MUST be the first field */
        union { 
/* device Ethernet address for ARP entry, multicast route */
            ttEnetAddress   rteEnetAddress; /* Ethernet host route */
            tt8Bit          rtePhysAddr[
                (((TM_MAX_PHYS_ADDR + 5) / 4) * 4) - 2];
            ttIpAddress     rtePhysIpAddress; /* this is trash-proof */
        }   rtePhysAddrUnion;
        tt8Bit              rteFiller1;
        tt8Bit              rtePhysAddrLen;
    } rteLanArpStruct;
    struct
    {
        ttIpAddress         rteGatewayAddress; /* indirect route */
        tt16Bit             rteTag;            /* Route tag */
        tt16Bit             rteFiller2;        /* Multiple of 4 bytes */
    } rteGatewayStruct;

    struct 
    {    
        ttIpAddress         rteTunnelExitAddr;
    } rteTunnelStruct;


} ttRteHostGwayUnion;

/* Shorthands into host gateway union */
#define GSGateway              rteGatewayStruct.rteGatewayAddress
#define GSTag                  rteGatewayStruct.rteTag
#define HSDevIpAdd             rteLocalNetStruct.rteDevIpAddress
#define HSEnetAdd              rteLanArpStruct.rtePhysAddrUnion.rteEnetAddress
#define HSPhysAddr             rteLanArpStruct.rtePhysAddrUnion.rtePhysAddr
#define HSPhysAddrLen          rteLanArpStruct.rtePhysAddrLen
#define HSTunnelExitAddr       rteTunnelStruct.rteTunnelExitAddr
#define HSLastReachedTickCount rteLanArpStruct.rteLastReachedTickCount


typedef ttRteHostGwayUnion TM_FAR * ttRteHostGwayUnionPtr;

/* Router entry structure:
 * rteLeafNode      Leaf node for a routing entry
 * rteInternalNode  Internal node corresponding to the added leaf node (it
 *                  is the routing entry leaf node's parent)
 * rteFlags         16-bit flag for routing entry.
 * rteFlags2        extra 16-bit flag for routing entry.
 * rteOwnerCount    Number of owners for this routing entry
 * rteMhomeIndex    Multihome index in device entry
 * rteDevPtr        Pointer to device entry corresponding to the route
 * rteHostGwayUnion Host gateway union. (See above.)
 * rteClonedPtr     Pointer to ARP entry for route or pointer to loop back
 *                  entry for local route
 * rteCloneMask     Ip network address mask for cloned routes (ARP) (IPV4)
 * rteMtu           IP Mtu for the route
 * rteHops          Number of hops for the route
 * rteTtl           Time to live in seconds for this entry (for redirect, RIP)
 * rteDelay;        Delay for the route
 * rteRtt           Round trip time for the route
 * rteClonePrefixLength (IPV6)
 *                  Ip network address prefix length for cloned routes (ARP)
 * rteRefresh       ARP cache need to be refreshed soon. Makes the IP
 *                  layer send an ARP request in the send path.
 */

typedef struct  tsRteEntry
{
    struct tsRtRadixNode    rteLeafNode;
    struct tsRtRadixNode    rteInternalNode;
    ttDeviceEntryPtr        rteDevPtr;
    ttRteHostGwayUnion      rteHostGwayUnion;
/* Pointer to ARP entry for this route or to Loop back entry for local route */
    struct tsRteEntry TM_FAR *rteClonedPtr;
#ifdef TM_SNMP_CACHE
    struct tsRteEntry TM_FAR *rteNextCachePtr;
    struct tsRteEntry TM_FAR *rtePrevCachePtr;
#endif /* TM_SNMP_CACHE */
    tt4IpAddress             rteCloneMask; /* IPV4 */
    tt32Bit                  rteHops;
    tt32Bit                  rteTtl;
    tt16Bit                  rteMtu;
    tt16Bit                  rteFlags;
    tt16Bit                  rteFlags2; /* Because rteFlags is full */
    tt16Bit                  rteOwnerCount;
    tt16Bit                  rteMhomeIndex;
/* SNMPD/NSTAT cache index */
    tt8Bit                   rteSnmpCacheIndex;
    tt8Bit                   rteRefresh;
} ttRteEntry;


typedef ttRteEntry    TM_FAR * ttRteEntryPtr;
typedef ttRteEntryPtr TM_FAR * ttRteEntryPtrPtr;

typedef int (*ttIntRtFuncPtr)(ttRteEntryPtr rtePtr, ttVoidPtr arg2Ptr);

#define rteGSGateway                rteHostGwayUnion.GSGateway
#define rteGSTag                    rteHostGwayUnion.GSTag
#define rteHSDevIpAdd               rteHostGwayUnion.HSDevIpAdd
#define rteHSEnetAdd                rteHostGwayUnion.HSEnetAdd
#define rteHSPhysAddr               rteHostGwayUnion.HSPhysAddr
#define rteHSPhysAddrLen            rteHostGwayUnion.HSPhysAddrLen
#define rteHSTunnelExitAddr         rteHostGwayUnion.HSTunnelExitAddr
#define rteHSLastReachedTickCount   rteHostGwayUnion.HSLastReachedTickCount



/*
 * This structure is used in the tfIpSendPacket(), tfIpForwardPacket(),
 * tf6IpSendPacket(), tf6IpForwardPacket() routines to cache the last accessed 
 * routing entry in the routing table.
 * We have one such cache per socket to be used by the ICMP/UDP/TCP socket
 * send code. 
 * For all other protocols, we have two context global variables in the IP
 * code of type ttRtCacheEntry: tvRtSendCache and tvRtForwardCache. 
 * Similarly, in the IPv6 code we have two context global variables of type
 * ttRtCacheEntry: tv6RtSendCache, and tv6RtForwardCache.
 */
typedef struct  tsRtCacheEntry
{
    ttIpAddress       rtcDestIpAddr; /* Destination IP Address */
    ttRteEntryPtr     rtcRtePtr;     /* Pointer to a routing entry */


  
} ttRtCacheEntry;

typedef ttRtCacheEntry TM_FAR * ttRtCacheEntryPtr;


/*
 * rthLeftEndNode  Left end root leaf of the tree (rtnKey is all 0) (invalid
 *  route entry), but default gateway stored as duplicate of this leaf.
 * rthHeadNode Head node of the tree.
 * rthRightEndNode Right end leaf of the tree (rtnKey is all 1)
 *  (invalid route entry)
 */
typedef struct  tsRtRadixHeadEntry
{
    ttRtRadixNode           rthLeftEndNode;
    ttRtRadixNode           rthHeadNode;
    ttRtRadixNode           rthRightEndNode;

/* Parameters to delayed (until tree is unlocked) tfRtResetCaches() function */
    tt16Bit                 rthResetCachesFlags;
    tt16Bit                 rthResetCachesAf;
} ttRtRadixHeadEntry;

/*
 * Short cuts used by SNMP.
 */
/* Route Destination IP address */
#define rteDest         rteLeafNode.rtnLSKey
/* Route Destination Network Mask */
#define rteMask         rteLeafNode.rtnLSMask
/* Route Next Hop */
#define rteNextHop      rteGSGateway
/* Device Index of outgoing interface */
#define rteDevIndex     rteDevPtr->devIndex


typedef struct tsArpMapping
{
    union
    {
        ttEnetAddress arpmUnEnetAddress;
#define arpmEnetAddress arpmEthPhysAddrUnion.arpmUnEnetAddress
        tt8Bit        arpmUnPhysAddr[(((TM_MAX_PHYS_ADDR + 5) / 4) * 4) - 2];
#define arpmPhysAddr  arpmEthPhysAddrUnion.arpmUnPhysAddr
    } arpmEthPhysAddrUnion;
    tt8Bit         arpmPhysAddrLen;
/* the arpmCommand field must come before arpmIpAddress */
    tt8Bit         arpmCommand;

    union
    {
        tt4IpAddress arpm4UnIpAddress;
#define arpm4IpAddress arpmIpAddressUnion.arpm4UnIpAddress
/* arpmIpAddress is for backwards compatibility with SNMP Agent code */
#define arpmIpAddress arpm4IpAddress
    } arpmIpAddressUnion;
    tt32Bit        arpmTtl;
    int            arpmDevIndex;
} ttArpMapping;

typedef ttArpMapping TM_FAR * ttArpMappingPtr;

/* ICMP info from the last ICMP error received for this socket */
typedef struct tsIcmpInfo
{
/* Source IP address of the ICMP error message in network byte order */
    ttIpAddress                 icmSrcIPAddr;
/*
 * Set to 1 by ICMP, when ICMP stores a new ICMP error message info,
 * to indicate to the user that new info has been stored.
 * Set to 0 by application, when application has read the ICMP info.
 */
    tt16Bit                     icmCtrlW;
/* ICMP type of ICMP error message */
    tt8Bit                      icmTypeB;
/* ICMP code of ICMP error message */
    tt8Bit                      icmCodeB;
} ttIcmpInfo;

#ifdef TM_IGMP
/* keep track of socket IGMP group joins so that we can leave these groups
   when the socket is closed */
typedef struct tsSockIgmpInfo {
    ttDeviceEntryPtr    siiDevicePtr;
    tt4IpAddress        siiMcastAddr;
} ttSockIgmpInfo;
typedef ttSockIgmpInfo TM_FAR *ttSockIgmpInfoPtr;
#endif /* TM_IGMP */


/*
 * Socket Entry Typedef
 * Defined as a red black binary tree
 * Search key is (protocolNumber<<16)|ourLayer4Port
 * Size of Each Entry is 144Bytes
 */
typedef struct tsSocketEntry
{
/* NOTE: socRbRightSocketPtr has to be first for recycling */
    struct tsSocketEntry TM_FAR *socRbRightSocketPtr;
    ttDeviceEntryPtr            socMcastDevPtr;
    tt16Bit                     socFlags;
    tt16Bit                     socOptions;
    ttIpAddress                 socPeerIpAddress;
    ttIpAddress                 socOurIfIpAddress;
    ttIpPort                    socPeerLayer4Port;
    ttIpPort                    socOurLayer4Port;
    tt32Bit                     socRedBlackKey;
    tt32Bit                     socSendQueueBytes;
    tt32Bit                     socMaxSendQueueBytes;
    tt32Bit                     socLowSendQueueBytes;
    tt16Bit                     socMaxSendQueueDgrams;  /* non-TCP */
    tt16Bit                     socSendQueueDgrams;     /* non-TCP */
    tt32Bit                     socRecvQueueBytes;
    tt32Bit                     socMaxRecvQueueBytes;
    tt32Bit                     socLowRecvQueueBytes;
    tt16Bit                     socMaxRecvQueueDgrams;  /* non-TCP */
    tt16Bit                     socRecvQueueDgrams;     /* non-TCP */
    tt32Bit                     socOobMark;
    ttPacketPtr                 socReceiveQueueNextPtr;
    ttPacketPtr                 socReceiveQueueLastPtr;
    ttPacketPtr                 socSendQueueNextPtr;
    ttPacketPtr                 socSendQueueLastPtr;
/*
 * Queue of sockets using the same local port number.
 */
    struct tsSocketEntry TM_FAR *socReuseAddrNextPtr;
    struct tsSocketEntry TM_FAR *socReuseAddrPrevPtr;
/*
 * For a connection request/accepted vector/socket: pointer to the
 * listening socket.
 */
    struct tsSocketEntry TM_FAR *socListenSocketPtr;
    struct tsSocketEntry TM_FAR *socRbParentSocketPtr;
    struct tsSocketEntry TM_FAR *socRbLeftSocketPtr;
#ifdef TM_SNMP_CACHE
    struct tsSocketEntry TM_FAR *socNextCachePtr;
    struct tsSocketEntry TM_FAR *socPrevCachePtr;
#endif /* TM_SNMP_CACHE */
/* Socket call back function without a user parameter */
    ttUserSocketCBFuncPtr       socUserCBFuncPtr;
/* Socket call back function with a user parameter */
    ttUserSocketCBParamFuncPtr  socUserCBParamFuncPtr;
/* User API parameter to socUserCBParamFuncPtr, or PING API entry pointer */
    ttVoidPtr                   socApiEntryPtr;
#ifdef TM_PEND_POST_NEEDED
    ttPendEntryPtr              socSelectPendEntryPtr; /* for select */
    ttPendEntry                 socSendPendEntry; /* Send Q or connect */
    ttPendEntry                 socRecvPendEntry; /* Recv Q or accept */
    ttPendEntry                 socClosePendEntry; /* linger on close */
#endif /* TM_PEND_POST_NEEDED */
    ttLockEntry                 socLockEntry;
    ttRtCacheEntry              socRteCacheStruct;
/* ICMP info from the last ICMP error message received for this socket */
    ttIcmpInfo                  socIcmpInfo;

#ifdef TM_IGMP
/* keep track of socket IGMP group joins so that we can leave these groups
   when the socket is closed */
    ttSockIgmpInfoPtr           socIgmpInfoArray;
/* number of in-use entries in socIgmpInfoArray */
    tt16Bit                     socIgmpInfoInUseCount;
/* number of allocated entries in socIgmpInfoArray */
    tt16Bit                     socIgmpInfoAllocCount;
#endif /* TM_IGMP */


    
    tt16Bit                     socIndex;
    tt16Bit                     socMaxBackLog;
    tt16Bit                     socBackLog;
    tt16Bit                     socLingerTime; /* in seconds */
    tt16Bit                     socSelectFlags;
    tt16Bit                     socRecvError;
    tt16Bit                     socSendError;
    tt16Bit                     socCBFlags; /* socket call back flags */
    tt16Bit                     socSendAppendThreshold;
    tt16Bit                     socRecvCopyFraction;
    tt16Bit                     socOwnerCount;
/*
 * Tree type flag for TCP sockets:
 * (TM_SOC_TREE_CL_FLAG, or TM_SOC_TREE_CO_FLAG)
 */
    tt16Bit                     socCoFlags;
    tt16Bit                     socMcastMhomeIndex;
    tt8Bit                      socProtocolNumber;
    tt8Bit                      socRbTreeColor;
    tt8Bit                      socProtocolFamily; /* PF_INET or PF_INET6 */
    tt8Bit                      socFlags2;
/* SNMPD/NSTAT cache index */
    tt8Bit                      socSnmpCacheIndex;
    tt8Bit                      socIpTtl;
    tt8Bit                      socIpMcastTtl;
    tt8Bit                      socIpTos;
    tt8Bit                      socFiller[2];

#ifdef TM_SNMP_MIB
    tt64Bit                     socHCInDatagrams;
    tt64Bit                     socHCOutDatagrams;
    tt64Bit                     socHCInOctets;
    tt64Bit                     socHCOutOctets;
    tt32Bit                     socStartTime;
#endif /* TM_SNMP_MIB */
} ttSocketEntry;

typedef ttSocketEntry TM_FAR * ttSocketEntryPtr;
typedef ttSocketEntryPtr TM_FAR * ttSocketEntryPtrPtr;


typedef int (*ttIntSocFuncPtr)(ttSocketEntryPtr socketPtr, ttVoidPtr arg2Ptr);

/*
 * TCP
 */

typedef tt32Bit  ttTs;  /* 32-bit time stamp */

/* Network header: TCP Header */
typedef struct tsTcpHeader
{
    ttIpPort tcpSrcPort;    /* Source port of the TCP segment */
    ttIpPort tcpDstPort;    /* Destination port of the TCP segment */
    ttSqn    tcpSeqNo;      /* 32-bit TCP sequence number */
    ttSqn    tcpAckNo;      /* 32-bit TCP acknowledgment number */
    tt8Bit   tcpDataOffset; /* 4-bit Data Offset/4-bit reserved bits */
    tt8Bit   tcpControlBits;/* 2-bit reserved bits/6-bit Control bits */
    tt16Bit  tcpWindowSize; /* 16-bit TCP window size */
/* One's complement checksum of TCP segment + pseudo_header */
    tt16Bit  tcpChecksum;
    tt16Bit  tcpUrgentPtr;  /* 16-bit Urgent pointer */
} ttTcpHeader;

typedef ttTcpHeader TM_FAR * ttTcpHeaderPtr;

/* END of TCP network header */

/*
 * TCP SEL ACK Block queue entry structure with Left Edge and Right Edge
 * sequence numbers
 */
typedef struct tsSackEntry
{
    struct tsSackEntry TM_FAR * blocQNextPtr;
    struct tsSackEntry TM_FAR * blocQPrevPtr;
           ttSqn                blocQLeftEdge;
           ttSqn                blocQRightEdge;
} ttSackEntry;

typedef ttSackEntry TM_FAR * ttSackEntryPtr;

typedef union tuTcpConnHeaderPtr
{
    ttSockAddrPtrUnionPtr       tcpuSockAddrUPtr;
    ttPacketPtr                 tcpuPacketPtr;
} ttTcpConnHeaderPtrUnion;

/*
 * Tcp State vector.
 */

typedef struct tsTcpVect
{
    ttSocketEntry               tcpsSocketEntry;
/* for queuing out of order packets */
    ttPacketPtr                 tcpsReassemblePtr;
/* for queuing recycled headers */
    ttPacketPtr                 tcpsRecycleHdrPtr;
/*
 * Outgoing TCP header (SYN, RST, or Keep Alive), or sockaddr pointer
 * for connect call.
 */
    ttTcpConnHeaderPtrUnion     tcpsConnHeaderPtrU;
#define tcpsSockAddrUPtr     tcpsConnHeaderPtrU.tcpuSockAddrUPtr
#define tcpsHeaderPacketPtr  tcpsConnHeaderPtrU.tcpuPacketPtr
#define tcpsSockAddrInPtr    tcpsSockAddrUPtr->sockInPtr
#define tcpsSockAddrIn6Ptr   tcpsSockAddrUPtr->sockIn6Ptr
#define tcpsSockAddrPeer6Ptr tcpsSockAddrUPtr->sockPeerAddr6Ptr
/* Tcp Timers + socket linger timer */
    ttTimerPtr                  tcpsTmPtr[TM_TCPTM_LINGER+1];
/*
 * For an listening/connection request vector/socket: pointer to next
 * and previous connection request vector/socket.
 */
    struct tsTcpVect TM_FAR   * tcpsConReqNextPtr;
    struct tsTcpVect TM_FAR   * tcpsConReqPrevPtr;
#ifdef TM_TCP_SACK
/* For queueing SEL ACK blocks */
    ttSackEntryPtr              tcpsSackBlockPtr[2];
#endif /* TM_TCP_SACK */
#ifdef TM_OPTIMIZE_SPEED
/* Cache the send queue packet that we are currently sending */
    ttPacketPtr                 tcpsSendQPacketPtr;
/* Cache the sequence number of that packet */
    ttSqn                       tcpsSendQSqn;
#endif /* TM_OPTIMIZE_SPEED */
/* Keep track of send complete bytes for socket CB function */
    tt32Bit                     tcpsSendCompltBytes;
/* Delay ACK time for connection */
    tt32Bit                     tcpsDelayAckTime;
    ttS32Bit                    tcpsScSmRtt;  /* Scaled Smooth RTT */
    ttS32Bit                    tcpsScRttVar; /* Scaled Rtt variance */
    tt32Bit                     tcpsRtoDef;  /* default RTO */
/* Minimum Retransmission timeout*/
    tt32Bit                     tcpsRtoMin;
/* Maximum Retransmission timeout*/
    tt32Bit                     tcpsRtoMax;
    tt32Bit                     tcpsRto;    /* Retransmission timeout*/
/* Tcp maximum retransmition time, before connection is broken */
    tt32Bit                     tcpsMaxRt;
/* Minimum Zero window probe time */
    tt32Bit                     tcpsProbeMin;
/* Maximum Zero window probe time */
    tt32Bit                     tcpsProbeMax;
/*
 * Send sequence variables
 */
/* Initial send sequence number (host byte order) */
    ttSqn                       tcpsIss;
/* send unacknowledge (host byte order) */
    ttSqn                       tcpsSndUna;
/* send next(host byte order)  */
    ttSqn                       tcpsSndNxt;
/* Highest sequence number sent (used because of retransmit) */
    ttSqn                       tcpsMaxSndNxt;
/* send urgent pointer (host byte order) */
    ttSqn                       tcpsSndUp;
#ifdef TM_TCP_SACK
/* Forward-most sequence number + 1 held by the receiver */
    ttSqn                       tcpsSndFack;
/* Highest send next when starting recovery */
    ttSqn                       tcpsCongSndNxt;
/* Retransmitted data while in recovery mode */
    tt32Bit                     tcpsRetranData;
/* Data smoothing */
    ttS32Bit                    tcpsWinTrim;
#endif /* TM_TCP_SACK */
/* Initial Send Congestion Window */
    tt32Bit                     tcpsInitialCwnd;
/* Send congestion window */
    tt32Bit                     tcpsCwnd;
/* Send slow start threshold size */
    tt32Bit                     tcpsSsthresh;
/* send window (host byte order) */
    tt32Bit                     tcpsSndWnd;
/* Maximum send window seen so far (host byte order) */
    tt32Bit                     tcpsMaxSndWnd;
/* sequence number used for last window update */
    ttSqn                       tcpsSndWL1;
/* acknowledgment number used for last window update */
    ttSqn                       tcpsSndWL2;
/*
 * Receive sequence variables
 */
/* receive next (host byte order) */
    ttSqn                       tcpsRcvNxt;
/* receive window (host byte order) */
    tt32Bit                     tcpsRcvWnd;
/* 
 * Pre-accept maximum recv window size (before receving the fist SYN),
 * and saved real maximum recv window size after that.
 */
    tt32Bit                     tcpsMaxRecvQueueBytes;
/* receive urgent pointer (host byte order) */
    ttSqn                       tcpsRcvUp;
/*
 * Sequence number of right edge of advertized window. (Could be smaller
 * than Rcv.Nxt+Rcv.Wnd to avoid silly window syndrome. Also used to
 * prevent shrinkage of our receive window.)
 */
    ttSqn                       tcpsRcvAdv;
/* initial receive sequence number (host byte order) */
    ttSqn                       tcpsIrs;
/* Congestion window decrease time stamp */
    ttTs                        tcpsCwndAge;
/*
 * Time stamp option support fields: TS.Recent and Last.ACK.sent
 * TS.Recent (tsVal's peer): holds a timestamp to be echoed in TSecr
 * whenever a segment is sent by us.
 */
    ttTs                        tcpsTsRecent;
/* host time of last TS.Recent*/
    ttTs                        tcpsTsRecentAge;
/* Last.ACK.sent: ACK field from the last segment sent by us.*/
    ttSqn                       tcpsLastAckSent;
    int                         tcpsSoftError; /* ICMP soft error */
    int                         tcpsAbortError; /* abort connection error */
/* Idle time (in seconds) of connection */
    int                         tcpsIdleTime;
/* number of retransmitted seg*/
    int                         tcpsReXmitCnt;
/* Maximum number of retransmitted seg*/
    int                         tcpsMaxReXmitCnt;
/* Keep alive timer (2 hours) in seconds */
    int                         tcpsKeepAliveTime;
/* Keep alive timer interval in seconds / Idle timer */
    int                         tcpsIdleIntvTime;
    int                         tcpsKeepAliveProbeCnt;
/*
 * Amount of time to stay in FIN WAIT 2 when socket has been closed
 * (10 minutes) in seconds.
 */
    int                         tcpsFinWt2Time;
/*
 * 2*Maximum segment life time (2*30 seconds) (TIME WAIT time) in
 * seconds
 */
    int                         tcps2MSLTime;
    tt16Bit                     tcpsSelectFlags; /* events to post on */
    tt16Bit                     tcpsSocketCBFlags; /* CB functions */
    tt16Bit                     tcpsMSS;        /* TCP MSS (RFC793)*/
    tt16Bit                     tcpsPeerMss;    /* TCP Peer MSS */
    tt16Bit                     tcpsEffSndMss;  /* TCP MSS without options */
/* TCP header size on recycle list*/
    tt16Bit                     tcpsHeaderSize;
    tt16Bit                     tcpsFlags;      /* TCP flags */
    tt16Bit                     tcpsFlags2;     /* additional flag bits */
    tt16Bit                     tcpsFlags3;     /* additional flag bits */
    tt16Bit                     tcpsHdrLeftOverBytes;
/* Cached partial checksum */
    tt16Bit                     tcpsPseudoXsum;
/* Offload flags */
    tt16Bit                     tcpsOffloadFlags;
#ifdef TM_DEV_SEND_OFFLOAD
/* Offload data size (without TCP/IP header size) */
    tt32Bit                     tcpsOffloadDataSize;
#else /* !TM_DEV_SEND_OFFLOAD */
#define tcpsOffloadDataSize tcpsEffSndMss
#endif /* !TM_DEV_SEND_OFFLOAD */
    tt8Bit                      tcpsSndWndScale;/* window scale option */
    tt8Bit                      tcpsRcvWndScale;/* window scale option */
    tt8Bit                      tcpsState;      /* TCP vector state */
    tt8Bit                      tcpsDupAcks;    /* Duplicate ACKs */
/* number of non duplicate acks after duplicate ACK(s) */
    tt8Bit                      tcpsAcksAfterRexmit;
/* Out of band byte */
    tt8Bit                      tcpsOobByte;
/* Used to avoid race conditions on listening socket back pointer */
    tt8Bit                      tcpsThreadCount;
/*
 * Maximum number of SQN in SACK option that will fit in the TCP header
 * (6, or 8)
 */
    tt8Bit                      tcpsMaxSackSqns;
} ttTcpVect;

typedef ttTcpVect TM_FAR * ttTcpVectPtr;

/*
 * SLIP State vector.
 */
typedef struct tsSlipVect
{
/* The packet we are working on now for receive */
    ttPacketPtr                slipsPacketPtr;
/* Where are we in the packet data we are creating with receive */
    tt8BitPtr                  slipsWorkingPtr;
    tt8BitPtr                  slipsSendDataPtr;
    ttS16Bit                   slipsSendBufSize;
/* For input Processing (where are we in unstuffing) */
    tt8Bit                     slipsLastByteStuffState;
    tt8Bit                     slipsFiller[1];
    tt8Bit                     slipsSendBuf[TM_SLIP_DEF_BUF_SIZE];
} ttSlipVect;

typedef ttSlipVect TM_FAR * ttSlipVectPtr;


/*
 * MD4
 * See Copyright Information from RSA in trmd4.c
 */
typedef struct {
    tt32Bit md4State[4]; 
    tt32Bit md4Count[2]; 
    tt8Bit  md4Buffer[64]; 
} ttMd4Ctx;
typedef ttMd4Ctx TM_FAR * ttMd4CtxPtr;

/*
 * MD5
 * See Copyright Information from RSA in trmd5.c
 */

typedef struct tsMd5Ctx
{
/* number of _bits_ handled mod 2^64 */
  tt32Bit i[2];
/* scratch buffer */
  tt32Bit buf[4];
/* input buffer */
  tt8Bit  in[64];
} ttMd5Ctx;
typedef ttMd5Ctx TM_FAR * ttMd5CtxPtr;


typedef struct 
{
    tt32Bit digest[ 5 ];            /* Message digest */
    tt32Bit countLo;
    tt32Bit countHi;                /* 64-bit bit count */
    tt32Bit data[ 16 ];             /* SHS data buffer */
    tt32Bit isBigEndian;
} ttSha1Ctx;
typedef ttSha1Ctx TM_FAR * ttSha1CtxPtr;

#ifdef TM_USE_RIPEMD
typedef struct 
{
    tt32Bit digest[5];
    tt32Bit countLo;
    tt32Bit countHi;
    tt32Bit data[16];
} ttRmd160Ctx;
typedef ttRmd160Ctx TM_FAR * ttRmd160CtxPtr;
#endif /* TM_USE_RIPEMD*/

/*
 * RFC-2507 IP header compression.
 */ 

/*
 * Parameters for IP header compression operation.
 */
typedef struct tsIphcOptions
{

/*
 * Maximum amount of time between sending full non-TCP headers.
 *      Default: TM_IPHC_MAX_TIME_DEF
 *      Limits: 1 - 255
 */
    tt16Bit iphcoMaxTime;

/*
 * Largest number of compressed non-TCP headers that may be sent without
 * sending a full header.
 *      Default: TM_IPHC_MAX_PERIOD_DEF
 *      Limits: 1 - 65535
 */
    tt16Bit iphcoMaxPeriod;

/*
 * Maximum number of outgoing non-TCP contexts.
 *      Default: TM_IPHC_CONTEXT_SPACE_DEF
 *      Limits: 3 - 65535
 */
    tt16Bit iphcoOutUdpSpace;

/*
 * Maximum number of incoming non-TCP contexts.
 *      Default: TM_IPHC_CONTEXT_SPACE_DEF
 *      Limits: 3 - 65535
 */
    tt16Bit iphcoInUdpSpace;

    tt16Bit iphcoMaxHeader;
    
/*
 * Maximum number of outgoing TCP contexts.
 *      Default: TM_IPHC_CONTEXT_SPACE_DEF
 *      Limits: 3 - 255
 */
    tt8Bit  iphcoOutTcpSpace;

/*
 * Maximum number of incoming TCP contexts.
 *      Default: TM_IPHC_CONTEXT_SPACE_DEF
 *      Limits: 3 - 255
 */
    tt8Bit  iphcoInTcpSpace;

/*
 * Flag indicating whether the IPHC decompressor should attempt to recover
 * from TCP errors.  This involves computing the TCP checksum (at least
 * one additional time) upon decompression of the packet to verify that
 * decompression was successful.  This is required to use the 'twice'
 * algorithm or header request error recovery.  If this flag is not set, the
 * checksum is not verified and will be validated only at the TCP layer.
 * The error will be corrected when the TCP peer retransmits this frame.
 *      Default: TM_IPHC_TCP_ERR_REC_DEF
 *      Limits: 1 or 0
 */
    tt8Bit  iphcoTcpErrRec;

/* Pad structure to 32-bit alignment */
    tt8Bit  iphcoPadding;

} ttIphcOptions;

typedef ttIphcOptions TM_FAR * ttIphcOptionsPtr;

/*
 * A UDP compression/decompression context.  Allocated for each UDP packet 
 * stream.
 */
#ifdef TM_LINT
LINT_UNREF_MEMBER_BEGIN
#endif /* TM_LINT */

typedef struct tsIphcCtxInfo    
{
/* Source address of the outer IP header. */
    ttIpAddress     ctxOuterSrcAddr;
/* Destination address of the outer IP header. */
    ttIpAddress     ctxOuterDestAddr;
/* Source address of the inner (tunneled) IP header. */
    ttIpAddress     ctxInnerSrcAddr;
/* Destination address of the inner (tunneled) IP header. */
    ttIpAddress     ctxInnerDestAddr;
/* Upper layer source port. */
    tt16Bit         ctxSourcePort;
/* Upper layer destination port. */
    tt16Bit         ctxDestPort;
/* Upper layer protocol type. */
    tt8Bit          ctxUpperLayerProto;
/* Protocol family (IPv4, IPv6) of outer IP header. */
    tt8Bit          ctxOuterProtoFamily;
/* Protocol family (IPv4, IPv6) of inner (tunneled) IP header.  */
    tt8Bit          ctxInnerProtoFamily;
    tt8Bit          ctxPaddingp[1];
} ttIphcCtxInfo;        

typedef ttIphcCtxInfo TM_FAR * ttIphcCtxInfoPtr;

typedef struct tsIphcUdpContext 
{
/* Information used to select this context for compression. */
    ttIphcCtxInfo                    uctxContextInfo;
    
/* Pointer to next context in list. */
    struct tsIphcUdpContext TM_FAR * uctxNextPtr;

/* Current full IP Header. */
    tt8BitPtr                        uctxIpHeaderPtr;

/* Current full UDP Header. */
    ttUdpHeader                      uctxUdpHeader;

/* 
 * Timestamp indicating the last time a full header was sent from this 
 * context.
 */
    tt32Bit                          uctxLastFullHdrTime;

/* Total length of the current IP header saved in uctxIpHeaderPtr. */
    ttPktLen                         uctxIpHeaderLength;
    
/*
 * The number of compressed headers that have been sent since the last full 
 * header.
 */
    tt16Bit                          uctxCompHdrCount;

/* Current full header time period. */
    tt16Bit                          uctxPeriod;

/* Context ID. */
    tt16Bit                          uctxCid;
    
    
/* Context Generation */
    tt8Bit                           uctxGeneration;

/* Pad structure to 32-bit alignment */
    tt8Bit                           uctxPadding[1];
} ttIphcUdpContext; 

typedef ttIphcUdpContext TM_FAR * ttIphcUdpContextPtr;
typedef ttIphcUdpContextPtr TM_FAR * ttIphcUdpContextPtrPtr;

/*
 * A TCP compression/decompression context.  Allocated for each TCP packet 
 * stream.
 */
typedef struct tsIphcTcpContext 
{
/* Information used to select this context for compression. */
    ttIphcCtxInfo                    tctxContextInfo;
    
/* Pointer to the next context in the list */
    struct tsIphcTcpContext TM_FAR * tctxNextPtr;

/* Current full IP header */
    tt8BitPtr                        tctxIpHeaderPtr;

/* Current full TCP header */
    ttTcpHeader                      tctxTcpHeader;

/* Length of payload in last compressed TCP packet. */
    ttPktLen                         tctxLastPayloadLen;

/* Total length of the current IP header saved in tctxIpHeaderPtr. */
    ttPktLen                         tctxIpHeaderLength;

/* Socket descriptor defining the packet stream for this context. */
    tt16Bit                          tctxSocketDesc;

/* Context ID */
    tt16Bit                          tctxCid;
    
/* Boolean flag indicating if this context is valid. */
    tt8Bit                           tctxValid;

/* Pad structure to 32-bit alignment */
    tt8Bit                           tctxPadding[3];
} ttIphcTcpContext; 
#ifdef TM_LINT
LINT_UNREF_MEMBER_END
#endif /* TM_LINT */

typedef ttIphcTcpContext TM_FAR * ttIphcTcpContextPtr;
typedef ttIphcTcpContextPtr TM_FAR * ttIphcTcpContextPtrPtr;

/*
 * Vector containing information about the current state of IP header 
 * compression for this link.
 */
typedef struct tsIphcVect   
{   

/* 
 * Must be the first element in the structure!  Contains the options used for 
 * IPHC on this link.  Initialized to defaults by IPHC, modified by the link 
 * layer.
 */
    ttIphcOptions       iphcOpts;

/* List of outgoing UDP contexts. */
    ttIphcUdpContextPtr iphcUdpOutCtxPtr;

/* List of incoming UDP contexts. */
    ttIphcUdpContextPtr iphcUdpInCtxPtr;

/* List of outgoing TCP contexts. */
    ttIphcTcpContextPtr iphcTcpOutCtxPtr;

/* List of incoming TCP contexts. */
    ttIphcTcpContextPtr iphcTcpInCtxPtr;
    
} ttIphcVect;   

typedef ttIphcVect TM_FAR * ttIphcVectPtr;
typedef ttIphcVectPtr TM_FAR * ttIphcVectPtrPtr;


/*
 * PPP Options
 */
typedef struct tsLcpOptions
{
    tt32Bit lcpMagicNumber;
    tt32Bit lcpAccm;
#ifdef TM_PPP_LQM
    tt32Bit     lqmReportingPeriod;
#endif /* TM_PPP_LQM */
    tt16Bit lcpMaxReceiveUnit;
    tt16Bit lcpQualityProtocol;
    union
    {
/* in localwant struture, each bit represents an auth method */
        tt8Bit  lcpuAuthProtocolBits;
/* in localGot, localNeg strutures, the whole 8 bit represents
 * an auth method
 */
        tt8Bit  lcpuAuthMethod;
    }lcpAuthUnion;
#define lcpAuthProtocolBits   lcpAuthUnion.lcpuAuthProtocolBits
#define lcpAuthMethod         lcpAuthUnion.lcpuAuthMethod
    tt8Bit  lcpFiller[3];
} ttLcpOptions;

typedef ttLcpOptions TM_FAR * ttLcpOptionsPtr;

typedef struct tsIpcpOptions
{
    tt4IpAddress    ipcpIpAddress;
    tt4IpAddress    ipcpDnsPriAddress;
    tt4IpAddress    ipcpDnsSecAddress;
/* IPHC: Maximum value of TCP context identifier. */
    tt16Bit         ipcpCompTcpSpace;
/* IPHC: Maximum value of non-TCP context identifier. */
    tt16Bit         ipcpCompNonTcpSpace;
/* IPHC: Maximum interval between sending full headers. */
    tt16Bit         ipcpCompMaxPeriod;
/* IPHC: Maximum time, in seconds, between sending full headers. */
    tt16Bit         ipcpCompMaxTime;
/* IPHC: Largest header size that may be compressed, in bytes. */
    tt16Bit         ipcpCompMaxHeader;
/* IPv4 compression protocol.  Set to zero if compression is disabled (default).
   Otherwise, supported types are TM_PPP_COMP_TCP_PROTOCOL (Van Jacobson) and
   TM_PPP_IPHC_COMP_TCP. (RFC-2507 style IP header compression) */
    tt16Bit         ipcpCompressProtocol;
    tt8Bit          ipcpVjMaxSlots;
    tt8Bit          ipcpVjConnComp;
    tt8Bit          ipcpPadding[2];    
} ttIpcpOptions;

typedef ttIpcpOptions TM_FAR * ttIpcpOptionsPtr;

typedef struct tsPapOptions
{
/* Function to call when we get an authentication request */
    ttPapAuthenticateFunctPtr papAuthenticatePtr;
    tt8BitPtr                 papUserNamePtr;
    tt8BitPtr                 papPasswordPtr;
} ttPapOptions;

typedef ttPapOptions TM_FAR * ttPapOptionsPtr;

typedef struct tsChapOptions
{
/* Function to call when we get an authentication request */
    ttChapAuthenticateFunctPtr   chapAuthenticatePtr;
    tt8BitPtr                    chapUserNamePtr;
    tt8BitPtr                    chapSecretPtr;
    tt8Bit                     chapValue[TM_CHAP_MAX_VALUE_SIZE];
} ttChapOptions;

typedef  ttChapOptions  TM_FAR * ttChapOptionsPtr;

/* PPP state function pointer */
typedef int (*ttPppStateFunctPtr)(ttVoidPtr    pppVectPtr,
                                  ttPacketPtr  packetPtr);

/* PPP state function pointer */
/* This gives a compiler warning */
/*typedef int (*ttPppStateFunctPtr)(struct tsPppVect TM_FAR * pppVectPtr,
                                  ttPacketPtr               packetPtr);
*/
typedef struct tsCpState
{
/* For config and terminate */
    ttTimerPtr                cpsTmrPtr;
/* The state functions to be associated with the layer we are in */
    ttPppStateFunctPtr        TM_FAR *cpsStateFuncList;
/* A counter of how many times that we have left to try this send */
    tt8Bit                    cpsRetryCounter;
/* The initial value for our retry counter */
    tt8Bit                    cpsRetryInitialValue;
/* The number of seconds between retry attempts */
    tt8Bit                    cpsRetryTimeoutSeconds;
/* The state that we are currently working in */
    tt8Bit                    cpsState;
/* Identifier used for this packet */
    tt8Bit                    cpsIdentifier;
/* Max number of terminate requests before giving up */
    tt8Bit                    cpsInitTrmRetryCnt;
/* Max number of configuration requests before giving up */
    tt8Bit                    cpsInitCfgRetryCnt;
/* Max number of NAKs to be sent before rejecting an option (max-failures) */
    tt8Bit                    cpsInitMaxFailures;
/* Current number of NAK failures */
    tt8Bit                    cpsFailures;
/* Numbers of times this layer has negotiated */
    tt8Bit                    cpsNegotiateCounter;
    tt8Bit                    cpsMagicNumberFailures;
/* Fill structure to align to long word boundary */
    tt8Bit                    cpsFiller[1];
} ttCpState;

typedef ttCpState TM_FAR *ttCpStatePtr;

typedef struct tsLcpState
{
/* State Information MUST BE FIRST */
    ttCpState                 lcpsStateInfo;
/* Flag to tell us which LCP option to negotiate for us */
    tt32Bit                   lcpsLocalNegotiateOption;
/* Flag to tell us which LCP option to allow the remote to use */
    tt32Bit                   lcpsRemoteNegotiateOption;
/* Flag to tell us which LCP options that we ACKed for the remote */
    tt32Bit                   lcpsRemoteAckedOptions;
/* LCP Option Values we want to use for our side */
    ttLcpOptions              lcpsLocalWantOptions;
/* LCP Option Values the remote will let us use */
    ttLcpOptions              lcpsLocalNegOptions;
/* LCP Option Values we are using (ACKed) */
    ttLcpOptions              lcpsLocalGotOptions;
/* LCP Option Values that we let the remote side use */
    ttLcpOptions              lcpsRemoteAllowOptions;
/* LCP Option Values that other side is using (WE ACKed)*/
    ttLcpOptions              lcpsRemoteSetOptions;
} ttLcpState;

typedef ttLcpState TM_FAR * ttLcpStatePtr;

typedef struct tsIpcpState
{
/* State Information MUST BE FIRST */
    ttCpState                 ipcpsStateInfo;
/* Flag to tell us which IPCP option to negotiate for us */
    tt32Bit                   ipcpsLocalNegotiateOption;
/* Flag to tell us which IPCP option to allow the remote to use */
    tt32Bit                   ipcpsRemoteNegotiateOption;
/* Flag to tell us which IPCP options that we ACKed for the remote */
    tt32Bit                   ipcpsRemoteAckedOptions;
/* IPCP Option Values we want to use for our side */
    ttIpcpOptions             ipcpsLocalWantOptions;
/* IPCP Option Values the remote will let us use */
    ttIpcpOptions             ipcpsLocalNegOptions;
/* IPCP Option Values the remote side will let us use */
    ttIpcpOptions             ipcpsLocalGotOptions;
/* IPCP Option Values that we let the remote side use */
    ttIpcpOptions             ipcpsRemoteAllowOptions;
/* IPCP Option Values that other side is using */
    ttIpcpOptions             ipcpsRemoteSetOptions;
} ttIpcpState;

typedef ttIpcpState TM_FAR * ttIpcpStatePtr;

typedef struct tsPapState
{
/* State Information MUST BE FIRST */
    ttCpState                 papsStateInfo;
/* The info we will send when the peer requests PAP */
    ttPapOptions              papsLocalOurInfo;
/* The info we expect when we request PAP */
    ttPapOptions              papsRemoteAllowInfo;
/* Timer to close the connection if the peer does not send Auth-Reqs */
    ttTimerPtr                papsRemoteReqTimerPtr;

} ttPapState;

typedef ttPapState TM_FAR * ttPapStatePtr;

typedef struct tsChapState
{
/* State Information MUST BE FIRST */
    ttCpState                  chapsStateInfo;
/* The info we will send when the peer requests CHAP */
    ttChapOptions              chapsLocalOurInfo;
/* The info we expect when we request CHAP */
    ttChapOptions              chapsRemoteAllowInfo;
    ttPacketPtr                chapsResponsePacketPtr;
/* Extra timer for CHAP responses */
    ttTimerPtr                 chapsResponseTimerPtr;
/* Extra counter for CHAP responses */
    tt8Bit                     chapsResponseCounter;
    tt8Bit                     chapsFiller[3];
} ttChapState;

typedef ttChapState TM_FAR * ttChapStatePtr;


/*
 * VJ Header Compression data structure
 */

typedef struct tsVjCompData
{
    struct tsVjCompData TM_FAR * vjsNextPtr;
#define vjsIpHeader vjsUnion.vjsuIpHeader
    union
    {
        ttIpHeader               vjsuIpHeader;
        tt8Bit                   vjsuMaxHeader[
            TM_4PAK_IP_MAX_HDR_LEN+TM_PAK_MAX_TCP_HDR];
    } vjsUnion;
    tt8Bit                       vjsUsed;
    tt8Bit                       vjsConnId;
    tt8Bit                       vjsIpHdrLength;
    tt8Bit                       vjsTcpHdrLength;
} ttVjCompData;

typedef ttVjCompData TM_FAR *ttVjCompDataPtr;

#ifdef TM_PPP_LQM
/*
 * PPP LQM (Link Quality Management) State vector.
 */

typedef struct  tsLqmVect
{
    ttLqrCountDeltas    lqrCountDeltas;
    ttLqrCountsPtr      lastRcvdLqrPtr;
    ttLqmMonitorFuncPtr monitorFuncPtr;
    ttTimerPtr          lqrTimerPtr;
    ttTimerPtr          lqrReTxTimerPtr;
    tt32Bit             lastTickCount;
    tt32Bit             lqrTimerPeriodMsec;
    tt32Bit             negotiatedLqrTimerPeriodMsec;
    tt32Bit             peerLqrTimerPeriodMsec;
    tt32Bit             lqrReTxPeriodMsec;
    tt32Bit             txSaveInLqrs;
    tt16Bit             hysteresisMaxFailures;
    tt16Bit             failureCount;
    tt16Bit             hysteresisSamples;
    tt16Bit             sampleCount;
    tt8Bit              lqrReTxCount;
    tt8Bit              enabledStatus;
    tt8Bit              rcvLqrPendingStatus;
    tt8Bit              filler;
} ttLqmVect;
typedef ttLqmVect TM_FAR * ttLqmVectPtr;

/*
 * Link-Quality-Report Packet Type
 */
typedef struct tsPppLqrPacketType
{
    tt32Bit magicNumber;
    tt32Bit lastOutLqrs;
    tt32Bit lastOutPackets;
    tt32Bit lastOutOctets;
    tt32Bit peerInLqrs;
    tt32Bit peerInPackets;
    tt32Bit peerInDiscards;
    tt32Bit peerInErrors;
    tt32Bit peerInOctets;
    tt32Bit peerOutLqrs;
    tt32Bit peerOutPackets;
    tt32Bit peerOutOctets;
} ttPppLqrPacketType;

typedef ttPppLqrPacketType TM_FAR *ttPppLqrPacketTypePtr;
#endif /* TM_PPP_LQM */

/*
 * PPP State vector.
 */
typedef struct tsPppVect
{
/* Note: NextPtr need to be first for recycle list */
    struct tsPppVect TM_FAR * pppsNextPtr;
/* The packet we are working on now for receive */
    ttPacketPtr               pppsPacketPtr;
/* The device entry that we are linked to */
    ttDeviceEntryPtr          pppsDeviceEntryPtr;
/* Where are we in the packet data we are creating with receive */
    tt8BitPtr                 pppsWorkingPtr;

/*
 * Timer to handle waiting for the user to open another protocol family
 * if one has failed.  Only used in IPv4/IPv6 dual stack mode.
 */ 
    ttTimerPtr                pppsOpenFailTimerPtr;

/* Pointer to the most recently used entries in VJ data table */
    struct tsVjCompData TM_FAR * pppsVjSendDataPtr;
    struct tsVjCompData TM_FAR * pppsVjRecvDataPtr;
    
/*
 * Array of pointers to the state information for each layer
 * Size must allow for each of the pppsXxxState structs listed below
 */
    ttCpStatePtr              pppsStateInfoPtrArray[5];
    ttLcpState                pppsLcpState;
    ttPapState                pppsPapState;
    ttChapState               pppsChapState;
    ttIpcpState               pppsIpcpState;
    
/*
 * Pointer to send buffer. Either allocated on user demand or point inside
 * preallocated pppsSendBuf &pppsSendBuf[0].
 */
    ttCharPtr                 pppsSendBufPtr;

/* Pointer to the IP Header Compression state vector, if being used. */
    ttIphcVectPtr             pppsIphcVectPtr;
    
#ifdef TM_PPP_LQM
/* pointer to LQM state vector */
    ttLqmVectPtr              pppsLqmVectPtr;

/* pointer to user-defined function to handle Echo-Reply */
    ttEchoReplyFuncPtr        pppsEchoReplyFuncPtr;

/* [RFC1989].R2.3:10, per-frame count of InGoodOctets */
    tt32Bit                   pppsInGoodFrameOctets;
#endif /* TM_PPP_LQM */


    ttPktLen                  pppsHdrDecompressSpace;
    
/* Size of buffer to copy data into on transmit */
    tt16Bit                   pppsSendBufSize;
    tt16Bit                   pppsUserSendBufSize;

/* The Frame Check Sequence we have in this receive session */
    tt16Bit                   pppsWorkingFcs;

/* For input Processing (where are we in unstuffing) */
    tt8Bit                    pppsLastByteStuffState;

/* For input processing (where are we in padding the VJ Fields) */
    tt8Bit                    pppsVjCompFlag;
    tt8Bit                    pppsVjOptionFlags;
    tt8Bit                    pppsVjOffset;

/* Previous VJ compression connection */
    tt8Bit                    pppsLastSentConnId;
    tt8Bit                    pppsLastRecvConnId;

/*
 * Amount of time (in seconds) to wait for the user to open another protocol
 * family (such as IPv4) if the other one has failed.
 */ 
    tt8Bit                    pppsOpenFailTimeout;
    
/* Length of the last Conf-Req sent (used for incoming Conf-Ack verification */
    tt8Bit                    pppsLastConfReqLen;

    tt8Bit                    pppsPadding[2];


    
/*
 * Buffer containing the last Conf-Req sent (used for incoming Conf-Ack
 * verification).
 */
    tt8Bit                    pppsLastConfReq[TM_PPP_MAX_CONF_REQ];


/* This field has to be last */
    char                      pppsSendBuf[TM_PPP_DEF_BUF_SIZE];
} ttPppVect;

typedef ttPppVect TM_FAR * ttPppVectPtr;

/*
 * BOOTP/DHCP header
 */
typedef struct tsBootHeader
{
/* packet op code / message type. '1' = BOOTREQUEST, '2' = BOOTREPLY */
        tt8Bit      bootOp;
/*
 * hardware address type, see ARP section in "Assigned Numbers" RFC1700.
 * '1' = Ethernet (10 Mb)
 */
        tt8Bit      bootHtype;
/* hardware address length ('6' for 10mb ethernet).*/
        tt8Bit      bootHlen;
/*
 * client sets to zero, optionally used by gateways in cross-gateway
 * booting
 */
        tt8Bit      bootHops;
/*
 * transaction ID, a random number, used to match this boot request with the
 * responses it generates.
 */
        tt32Bit     bootXid;
/* filled in by client, seconds elapsed since client started trying to boot.*/
        tt16Bit     bootSecs;
/* Broadcast bit. */
        tt16Bit     bootFlags;
/* client IP address: should be set to zero */
        tt4IpAddress bootCiaddr;
/*
 * 'your' (client) IP address; filled by server if client doesn't
 * know its own address (ciaddr was 0).
 */
        tt4IpAddress bootYiaddr;
/*
 * server IP address; returned in BOOTREPLY by server.
 * DHCP clarifies the interpretation of the 'siaddr' field as the
 * address of the server to use in the next step of the client's
 * bootstrap process.  A DHCP server may return its own address in the
 * 'siaddr' field, if the server is prepared to supply the next
 * bootstrap service (e.g., delivery of an operating system executable
 * image), in DHCPOFFER, DHCPACK packets.  A DHCP server always returns
 * its own address in the 'server identifier' option.
 */
        tt4IpAddress bootSiaddr;
/* gateway IP address, used in optional BOOTP relay agent booting. */
        tt4IpAddress bootGiaddr;
/* client hardware address, filled in by client (16 bytes). */
        tt8Bit      bootChaddr[TM_BOOTHADDR_SIZE];
/* optional TFTP server host name, null terminated string (64 bytes). */
        tt8Bit      bootSname[TM_BOOTSNAME_SIZE];
/*
 * boot file name, null terminated string; 'generic' name or null in
 * BOOTREQUEST/DHCPDISCOVER, fully qualified directory-path name in
 * BOOTREPLY/DHCPOFFER. (128 bytes)
 */
        tt8Bit      bootFile[TM_BOOTFILENAME_SIZE];
/*
 * optional vendor-specific area, e.g. could be hardware type/serial on
 * request, or 'capability' / remote file system handle on reply.  This
 * info may be set aside for use by a third phase bootstrap or kernel.
 * Called options with DHCP protocol. (64 bytes for BOOTP, 312 bytes
 * for DHCP).
 */
        struct
        {
            tt32Bit     optMagic;
#define optFields    optFieldsUnion.optUnFields
#define optDhcpMsg   optFieldsUnion.optUnDhcpMsg
            union
            {
                tt8Bit      optUnFields[
                                TM_4PAK_DHCP_OPTIONS_LEN - sizeof(tt32Bit)];
/* 4-byte aligned DHCP message */
                tt32Bit     optUnDhcpMsg; /* DHCP message */
            } optFieldsUnion;
        } bootOptions;
} ttBootHeader;



typedef ttBootHeader TM_FAR * ttBootHeaderPtr;

/*
 * DNS Resolver Cache Entry Structure
 */
typedef struct tsDnsCacheEntry
{

/* Next entry in cache; set to zero if this entry is last */
    struct tsDnsCacheEntry TM_FAR * dnscNextEntryPtr;

/* Previous entry in cache; set to zero if this entry is first */
    struct tsDnsCacheEntry TM_FAR * dnscPrevEntryPtr;

/* Pointer to chain of address information structures. */
    struct addrinfo TM_FAR *        dnscAddrInfoPtr;
    
/* Hostname (eg, "server.treck.com") for this entry */
    ttCharPtr                       dnscHostnameStr;

/* String to contain hostname from reverse lookup */
    ttCharPtr                       dnscRevHostnameStr;

/* Retransmission timer */
    ttTimerPtr                      dnscTimerPtr;

/*
 * String used in DNS query packet: the requested hostname (ASCII) translated
 * to the DNS label format.
 */
    ttCharPtr                       dnscRequestStr;
#ifdef TM_PEND_POST_NEEDED
/* Semaphore used in blocking mode */
    ttPendEntry                     dnscPendEntry;
#endif /* TM_PEND_POST_NEEDED */

    
/* Error code (if any) returned from socket of DNS server */
    int                             dnscErrorCode;

/* Number of retransmissions left before request is aborted */
    int                             dnscRetriesLeft;

/* Index of the current DNS server */
    int                             dnscServerNum;

/* Value of 'tvTime' when this DNS response was recieved */
    tt32Bit                         dnscTimestamp;

/* Time in milliseconds,since 'dnscTimestamp',that this cache entry is valid */
    tt32Bit                         dnscTtl;

/* Indicates if this entry is completed and has been retrieved by the user */
    tt16Bit                         dnscFlags;

/* Indicates how many applications are pending or quering the entry */
    tt16Bit                         dnscCount;

/* ID of the last request sent that was associated with this cache entry */
    tt16Bit                         dnscRequestId;

/* Query type (Name, MX, reverse) */
    tt16Bit                         dnscQueryType;

} ttDnsCacheEntry;

typedef ttDnsCacheEntry TM_FAR * ttDnsCacheEntryPtr;
typedef ttDnsCacheEntryPtr TM_FAR * ttDnsCacheEntryPtrPtr;


/*
 * Common between FTP server and Telnet server.
 */

/*
 * Doubly linked Queue structure.
 */
typedef struct tsQueue
{
/* Pointer to next entry in list of connections */
    ttVoidPtr                     qNextEntryPtr;
    ttVoidPtr                     qPrevEntryPtr;
} ttQueue;

/* Server queue structure (for RUN Q and open connection list) */
typedef struct tsServerQueue
{
#define servNextQEntryPtr(qIndex)       servQEntry[qIndex].qNextEntryPtr
#define servPrevQEntryPtr(qIndex)       servQEntry[qIndex].qPrevEntryPtr
/* One queue for RUN Q, and one queue for open connection list */
    ttQueue                    servQEntry[2];
} ttServerQueue;

typedef ttServerQueue TM_FAR * ttServerQueuePtr;

/* Server head queue structure (for head of RUN Q and open connection list) */
typedef struct tsServerQueueHead
{
#define servHeadNextQEntryPtr(qIndex) servQHead.servQEntry[qIndex].qNextEntryPtr
#define servHeadPrevQEntryPtr(qIndex) servQHead.servQEntry[qIndex].qPrevEntryPtr
    ttServerQueue              servQHead;
    int                        servQHeadCount[2];
} ttServerQueueHead;
/* Node descriptor for Generic linked list */

typedef ttServerQueueHead TM_FAR * ttServerQueueHeadPtr;

#ifdef TM_SNMP_MIB
typedef struct tsIpMib
{
    ttS32Bit      ipForwarding;
    ttS32Bit      ipDefaultTTL;
    tt32Bit       ipInReceives;
    tt32Bit       ipInHdrErrors;
    tt32Bit       ipInAddrErrors;
    tt32Bit       ipForwDatagrams;
    tt32Bit       ipInUnknownProtos;
    tt32Bit       ipInDiscards;
    tt32Bit       ipInDelivers;
    tt32Bit       ipOutRequests;
    tt32Bit       ipOutDiscards;
    tt32Bit       ipOutNoRoutes;
    ttS32Bit      ipReasmTimeout;
    tt32Bit       ipReasmReqds;
    tt32Bit       ipReasmOKs;
    tt32Bit       ipReasmFails;
    tt32Bit       ipFragOKs;
    tt32Bit       ipFragFails;
    tt32Bit       ipFragCreates;
    tt32Bit       ipRoutingDiscards;
#ifdef U_ip
U_ip
#endif /* U_ip */
    char          valid[3];
    char          ipFiller;
} ttIpMib;

typedef struct tsIcmpMib
{
    tt32Bit       icmpInMsgs;
    tt32Bit       icmpInErrors;
    tt32Bit       icmpInDestUnreachs;
    tt32Bit       icmpInTimeExcds;
    tt32Bit       icmpInParmProbs;
    tt32Bit       icmpInSrcQuenchs;
    tt32Bit       icmpInRedirects;
    tt32Bit       icmpInEchos;
    tt32Bit       icmpInEchoReps;
    tt32Bit       icmpInTimestamps;
    tt32Bit       icmpInTimestampReps;
    tt32Bit       icmpInAddrMasks;
    tt32Bit       icmpInAddrMaskReps;
    tt32Bit       icmpOutMsgs;
    tt32Bit       icmpOutErrors;
    tt32Bit       icmpOutDestUnreachs;
    tt32Bit       icmpOutTimeExcds;
    tt32Bit       icmpOutParmProbs;
    tt32Bit       icmpOutSrcQuenchs;
    tt32Bit       icmpOutRedirects;
    tt32Bit       icmpOutEchos;
    tt32Bit       icmpOutEchoReps;
    tt32Bit       icmpOutTimestamps;
    tt32Bit       icmpOutTimestampReps;
    tt32Bit       icmpOutAddrMasks;
    tt32Bit       icmpOutAddrMaskReps;
#ifdef U_icmp
U_icmp
#endif /* U_icmp */
    char            valid[4];
} ttIcmpMib;

typedef struct ttTcpMib
{
    ttS32Bit      tcpRtoAlgorithm;
    ttS32Bit      tcpRtoMin;
    ttS32Bit      tcpRtoMax;
    ttS32Bit      tcpMaxConn;
    tt32Bit       tcpActiveOpens;
    tt32Bit       tcpPassiveOpens;
    tt32Bit       tcpAttemptFails;
    tt32Bit       tcpEstabResets;
    tt32Bit       tcpCurrEstab;
#ifdef TM_USE_NG_MIB2
    tt64Bit       tcpHCInSegs;
    tt64Bit       tcpHCOutSegs;
#else /* TM_USE_NG_MIB2 */
    tt32Bit       tcpInSegs;
    tt32Bit       tcpOutSegs;
#endif /* !TM_USE_NG_MIB2 */
    tt32Bit       tcpRetransSegs;
    tt32Bit       tcpInErrs;
    tt32Bit       tcpOutRsts;
#ifdef U_tcp
U_tcp
#endif /* U_tcp */
    char          valid[2];
    char          tcpFiller[2];
} ttTcpMib;

typedef struct tsUdpMib
{
    tt32Bit       udpNoPorts;
    tt32Bit       udpInErrors;
#ifdef TM_USE_NG_MIB2
    tt64Bit       udpHCInDatagrams;
    tt64Bit       udpHCOutDatagrams;
#else /* TM_USE_NG_MIB2 */
    tt32Bit       udpInDatagrams;
    tt32Bit       udpOutDatagrams;
#endif /* !TM_USE_NG_MIB2 */
#ifdef U_udp
U_udp
#endif /* U_udp */
    char          valid[1];
    char          udpFiller[3];
} ttUdpMib;

#endif /* TM_SNMP_MIB */


/* IP decapsulation handler function */
typedef void (TM_CODE_FAR *ttIpHandlerFuncPtr)( ttPacketPtr packetPtr,
                                                tt8Bit      ulp );

/* support for debug, error and trace logging to circular buffer */
typedef struct tsLogMsgHdr
{
/* messages are padded so that the msg hdr starts on a 32-bit boundary */
    tt8Bit lmhBeginHdr[4];  /* "HDR" indicates start of the hdr */
    union
    {
        struct tsLogMsgHdr TM_FAR * lmhMsgPrevPtr; /* pointer to prev msg */

/* pointer to next msg, used by tfLogWalk to walk the copy of the log buffer */
        struct tsLogMsgHdr TM_FAR * lmhMsgNextPtr;
    } lmhUnion;
    tt16Bit lmhMsgSeqNo;    /* 15-bit message sequence number */
    tt16Bit lmhMsgLen;      /* length of this message, including hdr and pad */
    tt8Bit lmhMsgData[TM_LOG_MSG_HDR_PAD_LEN]; /* start of message data */
} ttLogMsgHdr;

typedef ttLogMsgHdr TM_FAR * ttLogMsgHdrPtr;

typedef struct tsLogCtrlBlk
{
#ifdef TM_LOCK_NEEDED
    ttLockEntry lcbLockEntry;   /* lock for this structure */
#endif /* TM_LOCK_NEEDED */
    ttLogMsgHdrPtr lcbLastMsgPtr; /* pointer to the last message logged */
    tt8BitPtr lcbBufPtr;        /* pointer to head of circular buffer */
    tt32Bit lcbBufLen;          /* size of circular buffer, including pad */
    tt16Bit lcbBufEndPadLen;    /* padding at end of buffer */
    tt16Bit lcbMsgSeqNo;        /* next available message sequence number */
} ttLogCtrlBlk;

typedef ttLogCtrlBlk TM_FAR * ttLogCtrlBlkPtr;

/* The parameter structure for device driver ISR */
typedef struct tsDeviceIsrPara
{
    ttUserInterface deviInterfaceHandle;
#ifdef TM_MULTIPLE_CONTEXT
    ttUserContext   deviContextHandle;
#endif /* TM_MULTIPLE_CONTEXT */
} ttDeviceIsrPara;

typedef ttDeviceIsrPara TM_FAR * ttDeviceIsrParaPtr;

#ifdef TM_SNMP_CACHE

/* Union of pointers for pointer to first and last entry in the cache */
typedef union tuSnmpCachePtr
{
    ttSocketEntryPtr snmpcSocketPtr;
    ttRteEntryPtr    snmpcRtePtr;
    ttTcpVectPtr     snmpcTcpVectPtr;
    ttVoidPtr        snmpcVoidPtr;
} ttSnmpCachePtrUnion;

typedef struct tsSnmpdCacheHead
{
/* Pointer to first entry in the cache */
    ttSnmpCachePtrUnion snmpcFirstPtrU;
/* Pointer to last entry in the cache */
    ttSnmpCachePtrUnion snmpcLastPtrU;
/* Pointer to entry last read in the cache */
    ttSnmpCachePtrUnion snmpcLastReadPtrU;
/* Current Serial number to keep track of cache updates */
    tt32Bit             snmpcSerialNum;
/* Copy of Serial number at last cache reading */
    tt32Bit             snmpcSerialNumAtLastRead;
#ifdef TM_LOCK_NEEDED
    ttLockEntry         snmpcLockEntry;
#endif /* TM_LOCK_NEEDED */
} ttSnmpdCacheHead;

typedef ttSnmpdCacheHead TM_FAR * ttSnmpdCacheHeadPtr;

#endif /* TM_SNMP_CACHE */

/* IPsec AH header */
typedef struct tsAhHeader
{
    tt8Bit      ahNextHeader;       /* Next Header */
    tt8Bit      ahPayloadLen;       /* Length of data + 1, in 32bit */
    tt16Bit     ahReserve;          /* Reserved for future use */
    tt32Bit     ahSpi;              /* Security parameter index */
    tt32Bit     ahSeq;              /* Sequence number field */
/* variable size, 32bit bound*/ /* Authentication data */
} ttAhHeader;
typedef ttAhHeader TM_FAR * ttAhHeaderPtr;
typedef ttAhHeaderPtr TM_FAR * ttAhHeaderPtrPtr;

#ifdef TM_USE_IPSEC_TASK
typedef struct tsIpsecTaskEntry
{
    ttEventEntry   iteOutEvent;
    ttEventEntry   iteInEvent;
    ttList         iteCrypReqList;
#ifdef TM_LOCK_NEEDED
    ttLockEntry    iteLockEntry;
#endif /* TM_LOCK_NEEDED*/
    tt32Bit        iteOutQueueBytes;
    tt32Bit        iteInQueueBytes;
}ttIpsecTaskEntry;
typedef ttIpsecTaskEntry TM_FAR * ttIpsecTaskEntryPtr;
#endif /* TM_USE_IPSEC_TASK */

typedef struct tsTime {
        int tm_sec;     /* seconds after the minute - [0,59] */
        int tm_min;     /* minutes after the hour - [0,59] */
        int tm_hour;    /* hours since midnight - [0,23] */
        int tm_mday;    /* day of the month - [1,31] */
        int tm_mon;     /* months since January - [0,11] */
        int tm_year;    /* years since 1900 */
        int tm_wday;    /* days since Sunday - [0,6] */
        int tm_yday;    /* days since January 1 - [0,365] */
} ttTime;

typedef ttTime TM_FAR * ttTimePtr;


#ifdef __cplusplus
}
#endif

#endif /* _TRTYPE_H_ */

/* Processed by Treck Shipping Tool v2.37 
 * 2004/11/17 12:28:54      For: Marvell
 * File Name: trtype.h      File Revision: 4.1 
 * Project Name: tcpip.pj   Project Release: 4.1.2.1
 * Input checksum: 48002    Output checksum: 46023
 */
