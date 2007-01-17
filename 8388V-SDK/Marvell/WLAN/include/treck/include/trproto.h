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
 * Description:     Function Prototypes
 * Filename:        trproto.h
 * Author:          Odile & Paul
 * Date Created:    9/22/97
 * $Source: include/trproto.h $
 *
 * Modification History
 * $Revision: 4.1 $
 * $Date: 2004/11/07 21:05:19PST $
 * $Author: xwang $
 * $ProjectName: /home/mks/proj/tcpip.pj $
 * $ProjectRevision: 4.1.2.1 $
 */

#ifndef _TRPROTO_H_

#define _TRPROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Global Treck stack initialization (no context initialization).
 */
TM_PROTO_EXTERN void tfInitTreckMultipleContext(void);

/*
 * First per context Treck Init function. Initializes context global variables
 * settable by the user.
 */
TM_PROTO_EXTERN void tfContextGlobalVarsInit(void);

/*
 * Second per context Treck Init function. Initializes context variables
 * non settable by the user, and calls all other modules init routines.
 */
TM_PROTO_EXTERN void tfContextInit(void);

/*
 * Initialize the Treck simple heap.
 */
TM_PROTO_EXTERN tt32BitPtr tfSheapCreate(void);

/*
 * Enable the Loopback Interface
 */
TM_PROTO_EXTERN int tfUseLoopBack(void);

/*
 * Memory allocation routines
 */
/* Used to initialize the static area that the buffer system uses */
TM_PROTO_EXTERN void tfBufferInit(void);

/*
 * Heap allocator collects all freed blocks, concatenating adjacent blocks,
 * and put them on the free lists.
 */
#ifdef TM_USE_DYNAMIC_MEMORY
TM_PROTO_EXTERN void tfSheapCollect(  ttRawMemoryPtrPtr rawMemListPtr
                                    , ttRcylPtrPtr      rcylListPtr
#ifdef TM_LOCK_NEEDED
                                    , tt8Bit            needLock
#endif /* TM_LOCK_NEEDED */
                                    );
#else /* !TM_USE_DYNAMIC_MEMORY */
#ifdef TM_LOCK_NEEDED
TM_PROTO_EXTERN void tfSheapCollect(tt8Bit needLock);
#else /* !TM_LOCK_NEEDED */
TM_PROTO_EXTERN void tfSheapCollect(void);
#endif /* !TM_LOCK_NEEDED */
#endif /* !TM_USE_DYNAMIC_MEMORY */

/*
 * Always Add 4 bytes to the Allocation request
 */
/* Get a Raw Memory Block from the list.  If none available then malloc it */
#if (       defined(TM_ERROR_CHECKING)      \
         && defined(TM_USE_ANSI_LINE_FILE)  \
         && (defined(TM_KERNEL_VISUAL_X86) || defined(TM_KERNEL_WIN32_X86) \
             || defined(TM_MEMORY_LOGGING)))
TM_PROTO_EXTERN ttRawBufferPtr tfGetRawBuffer(
    ttPktLen size, char TM_FAR * filename, int lineNumber);
#else
TM_PROTO_EXTERN ttRawBufferPtr tfGetRawBuffer(ttPktLen size);
#endif

/* Return a Raw Memory block to the list */
TM_PROTO_EXTERN void tfFreeRawBuffer(ttRawBufferPtr memoryBlockPtr);

#ifdef TM_USE_DYNAMIC_MEMORY
/* Given an allocation size, determine the size of the memory queue
   that it fits in. */
TM_PROTO_EXTERN ttPktLen tfGetMemoryQueueSize(ttPktLen size);
#endif /* TM_USE_DYNAMIC_MEMORY */

/* Used to Get a single packet/shared data Buffer */
TM_PROTO_EXTERN ttPacketPtr tfGetSharedBuffer( int      hdrSize,
                                               ttPktLen dataSize,
                                               tt16Bit  flag );

/* Used to Free a single packet/shared data Buffer */
TM_PROTO_EXTERN void tfFreeSharedBuffer( ttPacketPtr packetPtr,
                                         tt8Bit socketLockFlag );

/* Used to Duplicate a single packet/shared data Buffer */
TM_PROTO_EXTERN ttPacketPtr tfDuplicateSharedBuffer(ttPacketPtr packetPtr);

/*
 * Used to make data contiguous starting at packetPtr->pktLinkDataPtr for the
 * next length bytes on a scattered buffer.
 * packetPtr always points to the beginning of a scattered buffer.
 * length bytes data straddles between the end of the first packetPtr, and
 * the next ones in the link.
 * Returns pointer to same packetPtr, if there was room to copy the data at
 * the end of the first link.
 * Returns newly allocated packetPtr, if there was not enough room and a new
 * link had to be allocated to replace the first link.
 */
TM_PROTO_EXTERN ttPacketPtr tfContiguousSharedBuffer( ttPacketPtr packetPtr,
                                                      int         length );


/* Used to Duplicate a packet/shared data list (part of one datagram) */
TM_PROTO_EXTERN ttPacketPtr tfDuplicatePacket(ttPacketPtr packetPtr);

/* Used to copy a packet/shared data list (part of one datagram) */
TM_PROTO_EXTERN void tfCopyPacket( ttPacketPtr srcPacketPtr,
                                   ttPacketPtr destPacketPtr );

/* Used to Free a packet/shared data list (part of one datagram) */
TM_PROTO_EXTERN void tfFreePacket( ttPacketPtr packetPtr,
                                   tt8Bit socketLockFlag );

/* Used to Free a chain of packet/shared data list */
TM_PROTO_EXTERN void tfFreeChainPacket( ttPacketPtr packetPtr,
                                        tt8Bit      socketLockFlag );

/* Allocate memory, and zero it */
TM_PROTO_EXTERN ttVoidPtr tfBufferCalloc(unsigned int size);

/*
 * Creates or Uses Recycled Timer Block and Puts into Active List
 * This also starts the timer since it is in the active list
 * When Execute is run and the timer has expired the timer is deleted
 * and put into the recycle list
 */
TM_PROTO_EXTERN ttTimerPtr tfTimerAdd( ttTmCBFuncPtr  callBackFunctionPtr,
                                       ttGenericUnion userParm1,
                                       ttGenericUnion userParm2,
                                       tt32Bit        timerMillSec,
                                       tt8Bit         timerFlags );


/*
 * Re-initialize an automatic timer with a new interval of call back.
 */
TM_PROTO_EXTERN void tfTimerNewTime(ttTimerPtr tmrPtr, tt32Bit time);

/*
 * Only stops & deletes timers in the Active Timer List
 */
TM_PROTO_EXTERN void tfTimerRemove(ttTimerPtr timerBlockPtr);

/*
 * Stop and mark timer for deletion from another thread
 * besides the timer
 */
TM_PROTO_EXTERN void tfTimerLockedRemove(ttTimerPtr timerBlockPtr);

/*
 * Compute time remaining for a running/suspended timer
 */
TM_PROTO_EXTERN tt32Bit tfTimerMsecLeft(ttTimerPtr timerPtr);

/*
 * Pend/Post Macros to suspend and resume a task/thread/process
 */
TM_PROTO_EXTERN ttCountSemPtr tfCreatePendEntry(ttPendEntryPtr pendEntryPtr);
TM_PROTO_EXTERN int tfPend( ttPendEntryPtr pendEntryPtr,
                            ttLockEntryPtr lockEntryPtr
#ifdef TM_USE_PEND_TIMEOUT
                          , tt32Bit        tickTimeout /* in ticks */
#endif /* TM_USE_PEND_TIMEOUT */
                            );
TM_PROTO_EXTERN ttVoid tfPost( ttPendEntryPtr pendEntryPtr,
                               ttLockEntryPtr lockEntryPtr,
                               int            errorCode );

/*
 * Return allocated memory for a counting semaphore.
 * Allocate from a block of preallocated counting semaphores. New
 * pre-allocated block is allocated in this routine if
 * there is no more in the block.
 */
TM_PROTO_EXTERN ttCountSemPtr tfCountSemAlloc(void);

/*
 * Recycle a counting semaphore (used either for suspend/resume, or locking)
 */
TM_PROTO_EXTERN void tfRecycleSemaphore(ttCountSemPtr semaphorePtr);

/*
 * Check and Lock a LockEntry
 */
TM_PROTO_EXTERN int tfLock(ttLockEntryPtr entryToLock, tt8Bit flag);

/*
 * Unlock a LockEntry
 */
TM_PROTO_EXTERN void tfUnLock(ttLockEntryPtr entryToUnlock);

/*
 * Allocate entity of size bytes; check recycle list first for a free entry.
 */
TM_PROTO_EXTERN ttVoidPtr tfRecycleAlloc(int   recycleIndex);

/*
 * Recycle rcylPtr entity on *rcylListPtrPtr recycle list
 */
/*
 * Original:
 * TM_PROTO_EXTERN void tfRecycleFree(ttRcylPtr rcylPtr, int recycleIndex);
 */
TM_PROTO_EXTERN void tfRecycleFree(ttVoidPtr rcylPtr, int recycleIndex);

/*
 * Socket / Protocol Interface
 */

/* Initialization routine */
TM_PROTO_EXTERN void tfSocketInit(tt32Bit maxNumberSockets);
TM_PROTO_EXTERN void tfRandSeed(unsigned short seed1, unsigned short seed2);

/*
 * Generate a 32 Bit Random Number
 */
TM_PROTO_EXTERN tt32Bit tfGetRandom(void);
TM_PROTO_EXTERN tt8Bit tfGetRandomByte(void);
TM_PROTO_EXTERN void tfGetRandomBytes(tt8BitPtr  rPtr, int size);
TM_PROTO_EXTERN ttS32Bit tfGetRandomDitter(ttS32Bit   ceiling);


/*
 * Allocate a socket Entry
 */
TM_PROTO_EXTERN ttSocketEntryPtr tfSocketAllocate ( tt8Bit   protocol,
                                                    ttIntPtr errorCodePtr );

/*
 * Insert a socket into the tree
 */
TM_PROTO_EXTERN int tfSocketTreeInsert( ttSocketEntryPtr socketEntryPtr,
                                        tt16Bit          flag );

/*
 * Insert a socket into the tree with a random port number
 */
TM_PROTO_EXTERN ttIpPort tfRandPortInsert( ttSocketEntryPtr socketEntryPtr,
                                           tt16Bit          flag );

/*
 * Re-insert a socket that is already in the tree, at a different
 * location in the tree. (Used by bind(), connect(), setsockopt with
 * local IP address change.)
 */
TM_PROTO_EXTERN int tf4SocketTreeReInsert(
    ttSocketEntryPtr socketEntryPtr,
    tt4IpAddress     localIpAddress,
    ttIpPort         localPortNumber,
    tt4IpAddress     remoteIpAddress,
    ttIpPort         remotePortNumber,
    tt16Bit          flag );


/*
 * Remove a socket from the tree it belongs to.
 */
TM_PROTO_EXTERN int tfSocketTreeDelete( ttSocketEntryPtr socketEntryPtr,
                                        tt16Bit flag );

/* Insert a locked socket entry into the socket array */
TM_PROTO_EXTERN int tfSocketArrayInsert(ttSocketEntryPtr socketEntryPtr);

/* Remove socket entry from the array */
TM_PROTO_EXTERN void tfSocketArrayDelete(ttSocketEntryPtr socketEntryPtr);

/*
 * General send primitive (used by all send calls)
 */
TM_PROTO_EXTERN int tfSocketSend( ttSocketEntryPtr socketEntryPtr,
                                  ttPacketPtr      packetPtr,
                                  ttPktLen         bufferLength,
                                  int              flags );

/*
 * Looks up a socket entry from the tree
 */
TM_PROTO_EXTERN ttSocketEntryPtr tf4SocketLookup(
    tt8Bit              protocol,
    tt4IpAddress        localIpAddress,
    ttIpPort            localPortNumber,
    tt4IpAddress        remoteIpAddress,
    ttIpPort            remotePortNumber,
    tt16Bit             flag );
#define tfSocketLookup tf4SocketLookup


/*
 * Walk down the socket tree, calling
 * (*processFuncPtr)(socketEntryPtr, voidPtr) for each socket entry
 * in the tree.
 */
TM_PROTO_EXTERN int tfSocketWalk( ttIntSocFuncPtr processFuncPtr,
                                  ttVoidPtr       voidPtr );

/*
 * Record error to the socket, and call back user error function
 * if any.
 */
TM_PROTO_EXTERN void tfSocketErrorRecord(int socIndex, int error);

/* Conversion from socket descriptor to socket entry pointer */
TM_PROTO_EXTERN ttSocketEntryPtr tfSocketCheckValidLock( int socketDescriptor
                                                       );

/* Same as tfSocketCheckValidLock, but also validates sockaddr parameter */
TM_PROTO_EXTERN ttSocketEntryPtr tfSocketCheckAddrLock(
          int                      socketDescriptor,
    const struct sockaddr TM_FAR * addrPtr,
          int                      addrLen,
          int TM_FAR *             errorCodePtr
          );

TM_PROTO_EXTERN ttSocketEntryPtr tfSocketCheckAddrLenLock(
          int                      socketDescriptor,
    const struct sockaddr TM_FAR * addrPtr,
          int                      addrLen,
          int TM_FAR *             errorCodePtr
          );

/*
 * Socket call return, checkin and unlock. If error code is non zero,
 * register it for the user, and return TM_SOCKET_ERROR, otherwise
 * return retCode.
 */
TM_PROTO_EXTERN int tfSocketReturn( ttSocketEntryPtr socketEntryPtr,
                                    int              socketDescriptor,
                                    int              errorCode,
                                    int              retCode );

/*
 * Socket call return, checkin and unlock. If error code is non zero,
 * register it for the user, and return TM_SOCKET_ERROR, otherwise
 * return retCode + copy our IP address, or peer address depending on the
 * address flag value.
 */
TM_PROTO_EXTERN int tfSocketReturnAddr(
                                    ttSocketEntryPtr socketEntryPtr,
                                    int              socketDescriptor,
                                    struct sockaddr TM_FAR *myAddressPtr,
                                    int              errorCode,
                                    int              retCode,
                                    tt8Bit           addressFlag);

/*
 * Reset all the sockets single routing cache entries of type flag (either
 * indirect or ARP).
 */
TM_PROTO_EXTERN void tfSocketResetCaches (tt16Bit           flag,
                                          int               addressFamilyFlag
#ifdef TM_LOCK_NEEDED
                                        , tt8Bit            lockFlag
#endif /* TM_LOCK_NEEDED */
                                         );

/*
 * Do socket notification (for incoming packet, send complete, connect
 * complete, accept complete, reset, error, remote close, close complete)
 */
TM_PROTO_EXTERN void tfSocketNotify( ttSocketEntryPtr socketEntryPtr,
                                     int              selectFlags,
                                     int              socCBFlags,
                                     int              errorCode );
/*
 * Notify from the register socket CB functions, if the socket has
 * already incoming data, or EOF from the remote.
 */
TM_PROTO_EXTERN void tfSockCBNotify(ttSocketEntryPtr socketEntryPtr);

/*
 * Incoming Data from TCP/UDP/ICMP
 * packetPtr->pktChainDataLength gives the total length of incoming user
 * data.
 * packetPtr->pktLinkDataLength gives user data length in one element of
 * chain of scattered data
 * packetPtr->pktLinkDataPtr = UserData for one element of chain of
 * scattered data.
 * UDP datagram is to be inserted in receive queue using the
 * pktChainNextPtr, pktChainPrevPtr (boundaries for a single datagram) of
 * the first ttPacket element.
 * TCP data is to be inserted in the receive queue using the pktLinkNextPtr
 * of the last ttPacket element and pktLinkPrevPtr of the first ttPacket
 * element.
 */
TM_PROTO_EXTERN int tfSocketIncomingPacket(ttSocketEntryPtr incomingSocketPtr,
                                           ttPacketPtr      packetPtr);

TM_PROTO_EXTERN void tfSocketFree(ttSocketEntryPtr socketEntryPtr);

/* Decrease the number of open sockets */
TM_PROTO_EXTERN void tfSocketDecreaseOpenSockets(void);

/* Reset a protocol socket cache. Called when socket is closed. */
TM_PROTO_EXTERN void tfSocketFlushProtocolCache(
                                            ttSocketEntryPtr socketEntryPtr );
/*
 * Append small packets to the end of a previous buffers. Used
 * only by TCP send().
 */
TM_PROTO_EXTERN ttPktLen tfSendAppend(
                                        ttSocketEntryPtr    socketEntryPtr,
                                        const char TM_FAR * bufferPtr,
                                        ttPktLen            bufferLength,
                                        int                 flags);

/* Common code between bind(), and tfBindNoCheck() */
TM_PROTO_EXTERN int tfBindFlag(
                            int                            socketDescriptor,
                            const struct sockaddr TM_FAR * localAddrPtr,
                            int                            localAddressLength,
                            int                            flag );

/*
 * Allocate link of ttShared/ttPacket structures to point to user
 * scattered data buffers.
 */
TM_PROTO_EXTERN int tfLinkScatDataToHeader(
                                        ttPacketPtrPtr     headPacketPtrPtr,
                                        ttUserBlockPtr     userBlockPtr,
                                        int                userBlockCount,
                                        ttUserFreeFuncPtr  userFreeFunction,
                                        int                ipHdrIncl );

/*
 * Free scattered user buffers.
 */
TM_PROTO_EXTERN void tfFreeScatData( ttPacketPtr        headPacketPtr,
                                     ttUserBlockPtr     userBlockPtr,
                                     int                userBlockCount,
                                     ttUserFreeFuncPtr  userFreeFunction );

/* md4 */
TM_PROTO_EXTERN void tfMd4Init(ttVoidPtr contextPtr);
TM_PROTO_EXTERN void tfMd4Update(void     * contextPtr,
                                 tt8BitPtr  dataPtr,
                                 tt32Bit    length);
TM_PROTO_EXTERN void tfMd4Final(tt8BitPtr   digestPtr,
                                void      * contextPtr);

/*
 * copyright RSA
 * (see copyright in trmd5.c)
 */
TM_PROTO_EXTERN void tfMd5Init(ttVoidPtr mdContextPtr);
TM_PROTO_EXTERN void tfMd5Update(ttVoidPtr      mdContextPtr,
                                 tt8BitPtr      inBufPtr,
                                 tt32Bit        inLen,
                                 tt32Bit        offset);
TM_PROTO_EXTERN void tfMd5Final(tt8BitPtr outDigest,
                                ttVoidPtr mdContextPtr);

TM_PROTO_EXTERN void tfSha1Init(ttVoidPtr      shaContextPtr);
TM_PROTO_EXTERN void tfSha1Update(ttVoidPtr    shaContextPtr,
                                  tt8BitPtr    inBufPtr,
                                  tt32Bit      count,
                                  tt32Bit      offset);
TM_PROTO_EXTERN void tfSha1Final(tt8BitPtr     outputPtr,
                                 ttVoidPtr     shaContextPtr);

#ifdef TM_USE_RIPEMD
/* RIPEMD160 */
TM_PROTO_EXTERN void tfRmd160Init(ttVoidPtr   rmd160ContextPtr);
TM_PROTO_EXTERN void tfRmd160Update(ttVoidPtr rmd160ContextPtr,
                                    tt8BitPtr inBufPtr,
                                    tt32Bit   count,
                                    tt32Bit   offset);
TM_PROTO_EXTERN void tfRmd160Final(tt8BitPtr    outputPtr,
                                   ttVoidPtr    rmd160ContextPtr);
#endif /* TM_USE_RIPEMD*/

TM_PROTO_EXTERN void tfArc4EncryptMessage(const tt8BitPtr   srcPtr,
                                          tt32Bit           srcLength,
                                          tt8BitPtr         keyPtr,
                                          tt32Bit           keyLength,
                                          tt8BitPtr         destPtr);

/*
 * TCP Entry Points
 */
/* Initialize TCP code */
TM_PROTO_EXTERN void tfTcpInit(void);
/* Initialize TCP state vector */
TM_PROTO_EXTERN void tfTcpVectInit(ttTcpVectPtr tcpVectPtr);
/* Connect/listen */
TM_PROTO_EXTERN int tfTcpOpen(ttTcpVectPtr tcpVectPtr, tt8Bit event);
/* Accept a connection */
TM_PROTO_EXTERN int tfTcpAccept(ttTcpVectPtr listenTcpVectPtr);
/*
 * Let TCP know that window is larger by copyCount. Update receive queue size.
 * Update out of band mark. Try and check if we should send a window update
 * but only if flags is set to do so.
 */
TM_PROTO_EXTERN void tfTcpRecvCmplt( ttTcpVectPtr tcpVectPtr,
                                     ttPktLen     copyCount,
                                     int          flags );
/* Check whether it is ok to receive data (according to TCP state) */
TM_PROTO_EXTERN int tfTcpRecv(ttTcpVectPtr tcpVectPtr);
/* Tcp User Receive out of band data */
TM_PROTO_EXTERN int tfTcpRecvOobData( ttTcpVectPtr  tcpVectPtr,
                                      char TM_FAR * bufferPtr,
                                      int           flags );
/* Tcp User Close */
TM_PROTO_EXTERN int tfTcpClose(ttTcpVectPtr tcpVectPtr);
/* TCP Incoming packet (from the IP layer) */
TM_PROTO_EXTERN void tfTcpIncomingPacket(ttPacketPtr packetPtr);
/* Set Tcp option */
TM_PROTO_EXTERN int tfTcpSetOption(       ttTcpVectPtr  tcpVectPtr,
                                          int           optionName,
                                    const char TM_FAR * optionValuePtr,
                                          int           optionLength );
/* Get Tcp option */
TM_PROTO_EXTERN int tfTcpGetOption( ttTcpVectPtr  tcpVectPtr,
                                    int           optionName,
                                    char TM_FAR * optionValuePtr,
                                    int  TM_FAR * optionLengthPtr );


/* Send TCP packet from the send queue */
TM_PROTO_EXTERN int tfTcpSendPacket(ttTcpVectPtr tcpVectPtr);
/* Tcp User Abort */
TM_PROTO_EXTERN int tfTcpAbort(ttTcpVectPtr tcpVectPtr, int error);
/* Called from ICMP to report an error to TCP layer */
TM_PROTO_EXTERN void tfTcpIcmpErrInput(ttTcpVectPtr tcpVectPtr, int error);
/* Purge all TCP headers from the TCP header recycle queue */
TM_PROTO_EXTERN void tfTcpPurgeTcpHeaders(ttTcpVectPtr tcpVectPtr);
/*
 * Read Shutdown:
 * User cannot recv any more data.
 * Flush receive, reassembly queues and SEL ACK block entries.
 */
TM_PROTO_EXTERN void tfTcpShutRead(ttTcpVectPtr tcpVectPtr);
/*
 * Write Shutdown:
 * Send a FIN on the TCP connection if appropriate. User can no longer
 * send data.
 */
TM_PROTO_EXTERN int  tfTcpShutWrite(ttTcpVectPtr tcpVectPtr);
/*
 * Remove a connection request from the listening queue of non accepted
 * connection request. Closing indicates whether the socket is being closed.
 */
TM_PROTO_EXTERN void tfTcpRemoveConReq(ttTcpVectPtr tcpVectPtr, int closing);

/* SLIP common routine: Initialize SLIP state vector */
TM_PROTO_EXTERN int tfSlipVectInit(ttDeviceEntryPtr devEntryPtr);

/*
 * BOOTP/DHCP common routines
 */
/* Open a BOOTP cliend UDP socket, and register a receive call back */
TM_PROTO_EXTERN int tfBtOpenSocket(void);

/*
 * Called when tfConfigInterface is called (type == TM_BT_CONF), or when a
 * user wants to get a user DHCP IP address (type == TM_BT_USER). Allocate
 * a boot entry for boot information storage. call tfBtOpenSocket().
 */
TM_PROTO_EXTERN int tfBtInit( ttDeviceEntryPtr        devEntryPtr,
                              ttUserDhcpNotifyFuncPtr dhcpNotifyFuncPtr,
                              int                     index,
                              tt8Bit                  type );

/*
 * Called from tfBtInit(), or tfDhcpCommonSet()
 * Allocated a boot entry for boot information storage, if not already
 * allocated.
 */
TM_PROTO_EXTERN ttBtEntryPtr tfBtAlloc ( ttDeviceEntryPtr        devEntryPtr,
                                         int                     index,
                                         tt8Bit                  type );

/*
 * Map from device entry, index, type to pointer to a btEntry pointer
 */
TM_PROTO_EXTERN ttBtEntryPtr tfBtMap( ttDeviceEntryPtr devEntryPtr,
                                      int              index,
                                      tt8Bit           type );

/* Allocate a packet and fill in default values for BOOTP/DHCP */
TM_PROTO_EXTERN ttPacketPtr tfBtGetFillHeader(
                                      ttBtEntryPtr     btEntryPtr,
                                      int              bootHeaderSize,
                                      ttTmCBFuncPtr    retryTimerFuncPtr );

/* Send a BOOTP/DHCP packet to provided destIpAddress on the network */
TM_PROTO_EXTERN int         tfBtSendPacket( ttPacketPtr      packetPtr,
                                            tt4IpAddress      destIpAddress );

/* Finish configuration. Notify user of error status */
TM_PROTO_EXTERN int         tfBtFinishConfig( ttBtEntryPtr     btEntrPtr,
                                              int              errorCode );

/* Remove the exponential back off timer */
TM_PROTO_EXTERN void        tfBtRemoveRetryTimer(ttBtEntryPtr btEntryPtr);

/*
 * Dhcp entry points
 */
TM_PROTO_EXTERN int tfDhcpStart( ttDeviceEntryPtr deviceEntryPtr,
                                 int              index,
                                 tt8Bit           type );
TM_PROTO_EXTERN int tfDhcpStop( ttDeviceEntryPtr deviceEntryPtr,
                                int              index,
                                tt8Bit           type );
TM_PROTO_EXTERN void tfDhcpRecvCB( ttPacketPtr  packetPtr,
                                   ttBtEntryPtr btEntryPtr );
TM_PROTO_EXTERN int tfDhcpRenewLease(ttUserInterface interfaceHandle,
                                     int             mHomeIndex,
                                     ttUser32Bit     timeoutMilliSecs,
                                     unsigned char   retries);

/*
 * UDP Entry Points
 */
/* Reset the incoming socket cache if it matches the closing socket */
TM_PROTO_EXTERN void tfUdpClose(ttSocketEntryPtr socketEntryPtr);

TM_PROTO_EXTERN int tfUdpSendPacket( ttSocketEntryPtr socketPtr,
                                     ttPacketPtr      packetPtr );

TM_PROTO_EXTERN void tfUdpIncomingPacket(ttPacketPtr packetPtr);

/*
 * Raw socket entry points
 */
TM_PROTO_EXTERN void tf4RawIncomingPacket(ttPacketPtr packetPtr);

/*
 * IP Entry Points
 */
/* Incoming IP Packet */
TM_PROTO_EXTERN void tfIpIncomingPacket(ttPacketPtr packetPtr);

/* Sending IP packet */
TM_PROTO_EXTERN int tfIpSendPacket( ttPacketPtr    packetPtr,
                                    ttLockEntryPtr lockEntryPtr );

/* Reassembling IP Packets */
TM_PROTO_EXTERN ttPacketPtr tfIpReassemblePacket(ttPacketPtr packetPtr);

/* Fragment IP packets */
TM_PROTO_EXTERN int tfIpFragmentPacket( ttPacketPtr packetPtr,
                                        ttPktLen    devMtu );

/* Get and cache the routing entry corresponding to ipDestAddr in rtcPtr */
TM_PROTO_EXTERN int tfIpCacheRte(
    ttRtCacheEntryPtr rtcPtr,
    tt4IpAddress      ipDestAddr
    );

/* IP header checksum for 20 bytes */
TM_PROTO_EXTERN tt16Bit tfIpHdr5Checksum(tt16BitPtr ipHeaderPtr);

/* TCP/IP packet checksum */
TM_PROTO_EXTERN tt16Bit tfPacketChecksum(
    ttPacketPtr packetPtr,
    ttPktLen    length,
    tt16BitPtr  pshHdrPtr,
    tt16Bit     pshLen );

/* Update checksum when one 16bit value changes in the header. Diff is the
 * difference between old value and new value
 */
TM_PROTO_EXTERN tt16Bit tfIpUpdateChecksum(tt16Bit oldSum, tt32Bit diff);

#ifdef TM_DEV_RECV_OFFLOAD
/*
 * Compute checksum on passed checksumPtr, and remove it from the hardware
 * computed checksum passed in in the ttDevRecvOffload structure.
 */
TM_PROTO_EXTERN void tfDevoRecvPeelChecksum( ttPacketPtr    packetPtr,
                                             tt16BitPtr     checksumPtr,
                                             tt16Bit        length );
/*
 * Trim any excess checksum computed by the hardware on the data beyond
 * the end of the packet.
 */
TM_PROTO_EXTERN void tfDevoRecvTrimChecksum( ttPacketPtr packetPtr,
                                             tt16Bit     hdrLength,
                                             ttPktLen    pktLength );
/*
 * Compute checksum on passed pshPtr, and add it to the hardware computed
 * checksum (which has had the IP header checksum removed already in
 * tfDevoRecvPeelChecksum) passed in the ttDevRecvOffload structure.
 * Remove checksum on trailing data if any.
 */
TM_PROTO_EXTERN tt16Bit tfDevoRecvPacketChecksum(
                                    ttDevRecvOffloadPtr pktDevRecvOffloadPtr,
                                    tt16BitPtr  pshPtr,
                                    tt16Bit     pshLength );
#endif /* TM_DEV_RECV_OFFLOAD */

/*
 * If ipAddr is multicast or directed broadcast on device/mhome, map
 * to multicast/broadcast ethernet address. Returns
 * TM_ETHER_UCAST if ipAddr is a unicast address
 * TM_ETHER_BROAD if mapped to Ethernet Broadcast addrress
 * TM_ETHER_MCAST if mapped to Ethernet multicast address
 */
TM_PROTO_EXTERN int tfIpBroadMcastToLan( tt4IpAddress     ipAddr,
                                         ttDeviceEntryPtr devEntryPtr,
                                         tt16Bit          mhomeIndex,
                                         tt8BitPtr        etherPtr );

/*
 * Given a destination address, find the outgoing interface/route and
 * fill in the IP header TOS, Source and Destination IP addresses.
 * Return 0 on success, error value on failure
 */
TM_PROTO_EXTERN int tfIpDestToPacket( ttPacketPtr       packetPtr,
                                      tt4IpAddress      srcIpAddress,
                                      tt4IpAddress      peerIpAddress,
                                      tt8Bit            ipTos,
                                      ttRtCacheEntryPtr rtcPtr,
                                      ttIpHeaderPtr     iphPtr );

/*
 * Reset non socket IP send cache, and IP forward cache of type flag
 * (either indirect or ARP).
 */
TM_PROTO_EXTERN void tfIpResetCaches(tt16Bit           flag,
                                     int               addressFamilyFlag
#ifdef TM_LOCK_NEEDED
                                   , tt8Bit            lockFlag
#endif /* TM_LOCK_NEEDED */
                                    );

#ifdef TM_SINGLE_INTERFACE_HOME
/*
 * Check whether the single interface is configured. Return pointer to
 * the device.
 */
TM_PROTO_EXTERN ttDeviceEntryPtr tfIfaceConfig(tt16Bit flag);
#else /* TM_SINGLE_INTERFACE_HOME */

/* Get multi-home index of directed broadcast match for a given interface */
TM_PROTO_EXTERN int tfMhomeDBroadMatch( ttDeviceEntryPtr   devEntryPtr,
                                        tt4IpAddress       ipAddress,
                                        tt16BitPtr         ifaceMhomePtr );

/* Get multi-home index of network match for a given interface */
TM_PROTO_EXTERN int tfMhomeNetMatch( ttDeviceEntryPtr      devEntryPtr,
                                     tt4IpAddress          ipAddress,
                                     tt16BitPtr            ifaceMhomePtr );

/* Get multi-home index of first configured multihome for a given interface */
TM_PROTO_EXTERN int tfMhomeAnyConf( ttDeviceEntryPtr       devEntryPtr,
                                    tt4IpAddress           ipAddr,
                                    tt16BitPtr             ifaceMhomePtr );

/* Get multi-home index of address match for a given interface/IP address */
TM_PROTO_EXTERN int tfMhomeAddrMatch( ttDeviceEntryPtr     devEntryPtr,
                                      tt4IpAddress         ipAddress,
                                      tt16BitPtr           ifaceMhomePtr );

/* Get Device/multi-home index of address match for a given IP address */
TM_PROTO_EXTERN ttDeviceEntryPtr tfIfaceMatch(
                                      tt4IpAddress         ipAddr,
                                      ttMhomeFunctPtr      mHomeMatchFuncPtr,
                                      tt16BitPtr           ifaceMhomePtr );
#endif /* TM_SINGLE_INTERFACE_HOME */


/*
 * Update chain with a new smaller length 'newLength':
 * Update chain data length with new length. Free all packets
 * at the end of the chain that no longer belong to the chain.
 * Update link data length of the last link.
 */
TM_PROTO_EXTERN void tfPacketTailTrim( ttPacketPtr packetPtr,
                                       ttPktLen    newLength,
                                       tt8Bit      socketLockFlag );

/*
 * Add addLength to tail link. Update all lengths.
 */
TM_PROTO_EXTERN void tfPacketTailAdd(ttPacketPtr packetPtr, int addLength);

/*
 * Trim head pointer by trimLen. Free shared buffers that are trimmed
 * all the way, but keep the first one around, since it contains packet
 * information, and could contain a network header. Update all lengths,
 * and pointer fields.
 */
TM_PROTO_EXTERN void tfPacketHeadTrim( ttPacketPtr packetPtr,
                                       ttPktLen    trimLen );

/* test if the current shared buffer has enough space to hold
 * new header size and|or tail size, if not, add shared buffer to
 * the front |end of the shared data.
 * returns new packet pointer if header changes
 */
TM_PROTO_EXTERN int tfPktHeadTailAdd(ttPacketPtrPtr packetPtrPtr,
                                        ttPktLen       copyHead,
                                        ttPktLen       needHead,
                                        ttPktLen       needTail);


/*
 * ICMP Entry Points
 */
/* ICMP error packet generation */
TM_PROTO_EXTERN void tfIcmpErrPacket( ttPacketPtr      packetPtr,
                                      tt8Bit           icmpType,
                                      tt8Bit           icmpCode,
                                      tt8Bit           icmpParam,
                                      ttDeviceEntryPtr outDevEntryPtr,
                                      tt4IpAddress     ipAddr );

/* Incoming ICMP packet from the network */
TM_PROTO_EXTERN void tfIcmpIncomingPacket(ttPacketPtr packetPtr);

/* Creating an ICMP packet internally */
TM_PROTO_EXTERN int tfIcmpCreatePacket( ttDeviceEntryPtr   devEntryPtr,
                                        tt16Bit            mhomeIndex,
                                        tt16Bit            flags,
                                        tt4IpAddress       IpSrcAddr,
                                        tt4IpAddress       IpDestAddr,
                                        tt8Bit             icmpType,
                                        tt8Bit             icmpCode,
                                        tt4IpAddress       ipAddr );

/*
 * User call to send data on a raw socket. If raw socket is of type ICMP
 * for example, ICMP header filled by the user.
 * Destination IP address stored in ttPacket. (If source known, stored in
 * ttPacket)
 */
TM_PROTO_EXTERN int tfRawSendPacket( ttSocketEntryPtr      sockPtr,
                                     ttPacketPtr           packetPtr );

/*
 * IGMP entry points
 */
/* Initialize IGMP variables */
TM_PROTO_EXTERN void tfIgmpInit(void);

/* Per interface IGMP initialization, i.e joining 224.0.0.1, 224.0.0.2 */
TM_PROTO_EXTERN void tfIgmpDevInit( ttDeviceEntryPtr devEntryPtr,
                                    tt16Bit          mhomeIndex );

/* IGMP IPPROTO_IP level setsockopt() calls */
TM_PROTO_EXTERN int tfIgmpSetSockOpt ( ttSocketEntryPtr   socketEntryPtr,
                                       int                optionName,
                                       ttConstCharPtr     optionValuePtr,
                                       int                optionLength );
/* IGMP IPPROTO_IP level getsockopt() calls */
TM_PROTO_EXTERN int tfIgmpGetSockOpt ( ttSocketEntryPtr   socketEntryPtr,
                                       int                optionName,
                                       ttConstCharPtr     optionValuePtr,
                                       ttIntPtr           optionLengthPtr );

TM_PROTO_EXTERN int tfIgmpJoinHostGroup(
    ttSocketEntryPtr socketEntryPtr,
    tt4IpAddress     groupAddress,
    ttDeviceEntryPtr devEntryPtr,
    tt16Bit          mhomeIndex );

TM_PROTO_EXTERN int tfIgmpLeaveHostGroup(
    ttSocketEntryPtr socketEntryPtr,
    tt4IpAddress     groupAddress,
    ttDeviceEntryPtr devEntryPtr );

/* Find out if multicast group has been joined on a given interface */
#ifdef TM_SINGLE_INTERFACE_HOME
TM_PROTO_EXTERN int tfIgmpMember( ttDeviceEntryPtr devEntryPtr,
                                  tt4IpAddress     multiCastGroup );
#else /* !TM_SINGLE_INTERFACE_HOME */
TM_PROTO_EXTERN int tfIgmpMember( ttDeviceEntryPtr devEntryPtr,
                                  tt4IpAddress     multiCastGroup,
                                  tt16BitPtr       mhomeIndexPtr );
#endif /* TM_SINGLE_INTERFACE_HOME */

/* IGMP protocol incoming packet */
TM_PROTO_EXTERN void tfIgmpIncomingPacket(ttPacketPtr packetPtr);

/*
 * ARP Entry Points
 */
/* Incoming Arp Message */
TM_PROTO_EXTERN int tfArpIncomingPacket(ttPacketPtr  packetPtr);
/* Arp resolution */
TM_PROTO_EXTERN int tfArpResolve(ttPacketPtr packetPtr);
/* Free all packets waiting for ARP resolution on the device. */
TM_PROTO_EXTERN void tfArpResEntryRemoveAll(ttDeviceEntryPtr devEntryPtr);
/* Send an Arp Request */
TM_PROTO_EXTERN int tfArpSendRequest( ttVoidPtr    deviceEntryPtr,
                                      tt16Bit      mhomeIndex,
                                      tt4IpAddress ipAddress );
/* Find a proxy entry corresponding to ipAddress */
TM_PROTO_EXTERN ttProxyEntryPtr tfArpFindProxyEntry(tt4IpAddress ipAddress);

TM_PROTO_EXTERN int tfEtherMcast( ttVoidPtr        interfaceHandle,
                                  tt4IpAddress     groupAddress,
                                  int              command );

TM_PROTO_EXTERN int tfEtherSetMcastAddr( ttDeviceEntryPtr devEntryPtr,
                                         ttEnetAddress    enetAddress,
                                         tt8Bit           specGroupCmd,
                                         int              command);

TM_PROTO_EXTERN void tfNudSetReachable( ttRteEntryPtr rteEntryPtr );

/*
 * Device Entry Points (most of them are in trsocket.h).
 */

/*
 * Add interface to the routing table, and set its configuration flag.
 */
TM_PROTO_EXTERN int tfDeviceStart( ttDeviceEntryPtr devEntryPtr,
                                   tt16Bit          multiHomeIndex,
                                   int              protocolFamily );

/*
 * Remove all entries associated with devEntryPtr from the routing table
 * and close link layer and device
 */
TM_PROTO_EXTERN int tfDeviceClose (ttDeviceEntryPtr devEntryPtr);

/*
 * Remove a configured interface from the routing table, and reset its
 * configuration flag.
 */
TM_PROTO_EXTERN int tfRemoveInterface( ttDeviceEntryPtr devEntryPtr,
                                       tt16Bit          mhomeIndex,
                                       int              protocolFamily );

/* Send an ARP or IP packet to the interface */
TM_PROTO_EXTERN int tfDeviceSend(ttPacketPtr packetPtr);

/*
 * Send one packet to the device. This is the link post send function
 * used by Ethernet, Loop back, or Transparent link layer. Packet has
 * already been queued by tfDeviceSend().
 * Function called from tfDeviceSend() if no Treck transmit task is used,
 * or from tfXmitInterface() if a Treck transmit task is used.
 */
TM_PROTO_EXTERN int tfDeviceSendOnePacket(ttPacketPtr packetPtr);

/*
 * Function called from tfDeviceSend() if no Treck transmit task is used,
 * or from tfXmitInterface() if a Treck transmit task is used,
 * when the link post send function returns an error.
 * If no device transmit queue is used, or if the errorCode is TM_ENOBUFS
 * (i.e the device transmit queue logic dropped a whole packet),
 * it dequeues and frees the last packet queued to the device send queue.
 * It updates the interface SNMP MIB out error variables.
 */
TM_PROTO_EXTERN int tfDeviceSendFailed( ttDeviceEntryPtr devEntryPtr,
                                        ttPacketPtr      packetPtr,
                                        int              errorCode,
                                        tt8BitPtr        needFreePacketPtr );

/* Unlock device driver. Check if we need to call tfSendCompleteInterface() */
TM_PROTO_EXTERN void tfDeviceUnlockSendComplete(ttDeviceEntryPtr devEntryPtr);


/* Common code to open a non point to point Link Layer device */
TM_PROTO_EXTERN int tfLinkOpen( void  TM_FAR * interfaceId,
                                tt16Bit        multiHomeIndex,
                                int            protocolFamily);

/* Null link-layer functions */
TM_PROTO_EXTERN int tfNullLinkClose(void TM_FAR * interfaceId);
TM_PROTO_EXTERN int tfNullLinkSend(ttPacketPtr packetPtr);
TM_PROTO_EXTERN int tfNullLinkRecv(
    void TM_FAR *interfaceId, void TM_FAR *bufferHandle);
TM_PROTO_EXTERN int tfNullLinkIoctl(int operation);
TM_PROTO_EXTERN int tfNullLinkError(int type);

/*
 * Interface Loopback Driver entry points with non scattered recv and
 * multiple scattered send calls.
 */
TM_PROTO_EXTERN int tfIntfDrvSend( ttUserInterface  interfaceHandle,
                                   char     TM_FAR  *dataPtr,
                                   int              dataLength,
                                   int              flag );

TM_PROTO_EXTERN int tfIntfDrvRecv ( ttUserInterface    interfaceHandle,
                                    ttCharPtr TM_FAR  *dataPtrPtr,
                                    ttIntPtr           dataSizePtr,
                                    ttUserBufferPtr    packetPtrPtr
#ifdef TM_DEV_RECV_OFFLOAD
                                  , ttDevRecvOffloadPtr recvOffloadPtr
#endif /* TM_DEV_RECV_OFFLOAD */
                                   );

TM_PROTO_EXTERN int tfIntfDrvGetPhyAddr( ttUserInterface interfaceHandle,
                                         char TM_FAR *physicalAddress );

TM_PROTO_EXTERN int tfIntfDrvIoctl( ttUserInterface interfaceHandle, int flag,
                                    void TM_FAR * optionPtr, int optionLen );


/*
 * Wait for an event to occur on an interface. Used by
 * tfWaitReceiveInterface, tfWaitSentInterface, and tfWaitXmitInterface
 */
TM_PROTO_EXTERN int tfWaitEventInterface(ttEventEntryPtr eventEntryPtr);


/* Check whether a user device handle is valid */
TM_PROTO_EXTERN int tfValidInterface(ttDeviceEntryPtr checkDevEntryPtr);

/*
 * Check whether a user device handle and multi home index are valid,
 * and that the device is configured for that multi home index
 */
TM_PROTO_EXTERN int tfValidConfigInterface (
                                           ttDeviceEntryPtr checkDevEntryPtr,
                                           tt16Bit          mHomeIndex );

/*
 * Check whether an event (recv, xmit, or send on the interface) has
 * occured. Common function used by tfCheckReceiveInterface,
 * tfCheckXmitInterface, or tfCheckSentInterface.
 */
TM_PROTO_EXTERN int tfCheckEventInterface(ttEventEntryPtr eventEntryPtr);

/* Return device destination IP address for PT2PT */
TM_PROTO_EXTERN tt4IpAddress tfDeviceDestIpAddress(
                                           ttDeviceEntryPtr devEntryPtr );

/* Check whether a user link layer handle is valid */
TM_PROTO_EXTERN int tfValidLinkLayer(ttLinkLayerEntryPtr linkLayerHandle);

/*
 * Router Entry Points
 */
/* Init routine */
TM_PROTO_EXTERN void tfRtInit(void);

#ifdef TM_LITTLE_ENDIAN
TM_PROTO_EXTERN tt16Bit tfRtBitOffset( tt4IpAddress    key,
                                       tt4IpAddress    ipaddr,
                                       tt4IpAddressPtr bitMaskPtr );
#endif /* TM_LITTLE_ENDIAN */


/* Called by tfConfigInterface to add a local route */
TM_PROTO_EXTERN int tf4RtAddLocal( ttDeviceEntryPtr      devEntryPtr,
/* local destination network for Ethernet, Remote IP address for SLIP/PPP */
                                  tt4IpAddress          destination,
                                  tt4IpAddress          myNetMask,
/* IP address of the interface */
                                  tt4IpAddress          myIpAddress,
/* Multihome index of the interface */
                                  tt16Bit               myMultiHomeIndex );

/*
 * Called by tfDelDefaultGateway, or by tfDelStaticRoute, or by
 * tfUncongigInterface to delete a route
 */
TM_PROTO_EXTERN int tf4RtDelRoute(
    tt4IpAddress        destIpAddress,
    tt4IpAddress        destNetMask,
    tt16Bit             rteCreateFlag );

/* Get IPv4 default gateway IP address */
TM_PROTO_EXTERN int tf4RtGetDefault(tt4IpAddressPtr gatewayIpAddrPtr);

/*
 * Called by tfAddRoute(), BOOTP/DHCP, MOBILIP, ICMP, SNMP to lock the
 * routing table, allocate a new router entry and insert it in the
 * routing table.
 */
TM_PROTO_EXTERN int tf4RtAddRoute(
 /* route device */
                                 ttDeviceEntryPtr       devEntryPtr,
/* Destination Ip address */
                                 tt4IpAddress           ipAddr,
/* destination network address */
                                 tt4IpAddress           netMask,
/* gateway */
                                 tt4IpAddress           gwayAddr,
/* Tag for RIP */
                                 tt16Bit                tag,
/* multihome index */
                                 tt16Bit                multiHomeIndex,
/* metrics for non local routes */
                                 int                    hops,
                                 tt32Bit                ttl,
/* flag to indicate owner local/static/redirect/RIP */
                                 tt16Bit                flags,
                                 tt8Bit                 needLock);

TM_PROTO_EXTERN int tf4RtAddHost(
 /* route device */
                                 ttDeviceEntryPtr       devEntryPtr,
/* Destination Ip address */
                                 tt4IpAddress           ipAddr,
/* physical address */
                                 tt8BitPtr              physAddrPtr,
/* physical address len */
                                 tt8Bit                 physAddrLen,
/* multihome index */
                                 tt16Bit                multiHomeIndex,
/* metrics for non local routes */
                                 tt32Bit                ttl,
/* flag to indicate cloned/static/link-layer/mcast */
                                 tt16Bit                flags );

/* Called by RIP when a RIP entry has been received */
TM_PROTO_EXTERN void tfRtRip( ttDeviceEntryPtr devEntryPtr,
                              tt16Bit          mhomeIndex,
                              tt4IpAddress     destIpAddr,
                              tt4IpAddress     destNetMask,
                              tt4IpAddress     nextHop,
                              int              metric,
                              tt16Bit          ripTag );

/* Called by ICMP when a redirect has been received */
TM_PROTO_EXTERN void tf4RtRedirect( ttDeviceEntryPtr devEntryPtr,
                                    tt16Bit          mhomeIndex,
                                    tt4IpAddress     dest,
                                    tt4IpAddress     gateway,
                                    tt4IpAddress     src );
/* Called by ICMP when an echo reply has been received */
TM_PROTO_EXTERN void tfRtEchoReply(tt4IpAddress ipAddress);
/*
 * Called by ICMP when an UNREACH for fragment arrives
 * Also called from the user tfAddPmtuRoute() to either set a new
 * Path MTU discovery value, and/or disable path MTU discovery mechanism.
 */
TM_PROTO_EXTERN int tfRtNextHopMtu( tt4IpAddress   ipAddress,
                                    tt16Bit        nextHopMtu,
                                    ttIpHeaderPtr  iphPtr );



/*
 * Called by the IP forward function and all send functions to get an
 * outgoing route entry pointer for a given destination address. Destination
 * IP address is in routing cache structure. Resulting routing entry is
 * stored in cache, if no error.
 * Return value is 0 on success, error value otherwise.
 */
TM_PROTO_EXTERN int tfRtGet(ttRtCacheEntryPtr rtCachePtr);


/*
 * Decrease ownership count of route when done with it. Recycle the entry if
 * both ownership is zero and route is no longer in the tree.
 */
TM_PROTO_EXTERN void tfRtUnGet(ttRteEntryPtr rtePtr);

/*
 * Called by the socket interface when the user closes the socket
 * to release the socket routing cache entry, or by tfSocketResetCaches()
 * or tfIpResetCaches() (both called by tfRtResetCaches in the router code,
 * when all the routing cache entries in the system need to be reset,
 * because a either a new route has been added, or because we need some room
 * in the ARP cache).
 */
TM_PROTO_EXTERN void tfRtCacheReset( ttRtCacheEntryPtr rtcPtr,
                                     tt16Bit           rteFlag );

/* Set physical address mapping to a routing table ARP Entry */
TM_PROTO_EXTERN int tfRtArpAddByRte (
    ttRteEntryPtr       rtePtr,
    tt8BitPtr           physAddrPtr,
    tt32Bit             ttl,
    tt8Bit              physAddrLen );

/*
 * Add/Set an ARP Entry to routing table for a given destination IP address/
 * Physical address. Returns 0 on success, error value on failure.
 */
TM_PROTO_EXTERN int tf4RtArpAddEntry(
    tt4IpAddress        ipAddress,
    tt8BitPtr           physAddrPtr,
    tt32Bit             ttl,
    tt8Bit              physAddrLen );



/*
 * Execute a user ARP entry command as set in arpmCommand field of
 * arpMappingPtr:
 * TM_ARP_GET_BY_PHYS:
 * Get an ARP Entry mapping in routing table using Physical address to
 * locate.
 * TM_ARP_GET_BY_IP:
 * Get an ARP Entry mapping in routing table using IP address to locate
 * TM_ARP_DEL_BY_IP:
 * Erase an ARP Entry mapping in routing table using IP address to locate
 * TM_ARP_DEL_BY_PHYS:
 * Erase an ARP Entry mapping in routing table using Physical address to
 * locate.
 * TM_ARP_SET_TTL
 * Change ttl of ARP entry.
 */
TM_PROTO_EXTERN int tfRtArpCommand(ttArpMappingPtr arpMappingPtr);

#ifdef TM_SNMP_MIB
/* SNMP Agent calls tfSnmpRtFindIpRouteDest to get a copy of a routing entry */
TM_PROTO_EXTERN int tfSnmpRtFindIpRouteDest(
    ttRteEntryPtr ipRouteEntryPtr, tt32Bit ipRouteDest);
#ifdef TM_USE_NG_MIB2
TM_PROTO_EXTERN int tfSnmpNgRtFindIpRouteDest(
    ttRteEntryPtr ipRouteEntryPtr, ttIpAddressPtr  ipRouteDest);
#endif /* TM_USE_NG_MIB2 */
#endif /* TM_SNMP_MIB */

/*
 * Walk down the tree, and for each reached leaf call the routing entry
 * processing function
 */
TM_PROTO_EXTERN int tfRtTreeWalk( ttIntRtFuncPtr processRtePtr,
                                  ttVoidPtr      voidPtr,
                                  tt8Bit         needLock );

/* Print the content of the routing tree */
TM_PROTO_EXTERN void tfRtTreePrint (void);

/* remove gateway entries that has a certain gateway ip address */
TM_PROTO_EXTERN int tfRtRemoveGwEntry(ttIpAddressPtr ipAddrPtr);


/* Update the multihome index for the IPv4 default gateway routing entry. */
TM_PROTO_EXTERN int tf4RtModifyDefGatewayMhome(tt16Bit mhomeIndex);

/* Send a RIP request to update our routing table */
TM_PROTO_EXTERN int tfRipSendRequest( ttDeviceEntryPtr devEntryPtr,
                                      tt16Bit          mhomeIndex );

/* FTPD, FTP, NAT socket address to h1,h2,h3,h4,p1,p2 conversion */
TM_PROTO_EXTERN int tfFtpSockAddrToHxPxFormat( ttSockAddrInPtr   sockAddrPtr,
                                               ttCharPtr         bufferPtr );
/* FTPD, FTP, NAT h1,h2,h3,h4,p1,p2 to socket address conversion */
TM_PROTO_EXTERN int tfFtpHxPxFormatToSockAddr( ttCharPtr       bufferPtr,
                                               ttSockAddrInPtr sockAddrInPtr );
/*
 * convert unsigned long decimal from binary to ASCII. Returns pointer to end
 * of string containing the converted number.
 */
TM_PROTO_EXTERN ttCharPtr tfUlongDecimalToString(ttCharPtr setBufPtr,
                                                 tt32Bit   data);

/*
 * Common between FTP server and Telnet server.
 */
/*
 * Remove a server queue entry from either the run Q, or open connection
 * list as given by queueIndex, if entry is in the queue.
 */
TM_PROTO_EXTERN void tfServerRemoveFromQueue(
                                    ttServerQueueHeadPtr  servQueueHeadPtr,
                                    ttServerQueuePtr      servQueuePtr,
                                    int                   queueIndex );

/*
 * Insert a server queue entry in either the run Q, or open connection
 * list as given by queueIndex, if entry is not already in the queue.
 */
TM_PROTO_EXTERN void tfServerAppendToQueue(
                                    ttServerQueueHeadPtr  servQueueHeadPtr,
                                    ttServerQueuePtr      servQueuePtr,
                                    int                   queueIndex);

#ifdef TM_PPP_LQM
TM_PROTO_EXTERN void tfLqmEnable(
                        ttPppVectPtr    pppVectPtr,
                        tt32Bit         negotiatedLqrTimerHsec,
                        tt32Bit         peerLqrTimerHsec);

TM_PROTO_EXTERN void tfLqmDisable(
                        ttPppVectPtr    pppVectPtr);

TM_PROTO_EXTERN int tfLqmIsEnabled(
                        ttPppVectPtr    pppVectPtr);

/* called by PPP to process a Link-Quality-Report message */
TM_PROTO_EXTERN int tfLqmIncomingPacket(
                        ttPppVectPtr    pppVectPtr,
                        ttPacketPtr     packetPtr);
#endif /* TM_PPP_LQM */

/* User notification of a link event */
TM_PROTO_EXTERN void tfPppUserNotify(
                        ttDeviceEntryPtr devEntryPtr,
                        int              llEvent);

TM_PROTO_EXTERN int tfPppIsOpen(
                        ttPppVectPtr    pppVectPtr);

/* Common deviceSend routine when called from PPP */
TM_PROTO_EXTERN int tfPppDeviceSend(
                        ttPppVectPtr    pppVectPtr,
                        ttPacketPtr     packetPtr,
                        tt16Bit         pppProtocol);

/*
 * Allocate a PPP state vector
 */
TM_PROTO_EXTERN ttPppVectPtr tfPppVectAlloc(ttDeviceEntryPtr devEntryPtr);





/* support for debug, error and trace logging to circular buffer */
TM_PROTO_EXTERN ttLogCtrlBlkPtr tfLogAllocBuf(
    tt16Bit maxLogMsgLen, tt16Bit numLogMsgs);
TM_PROTO_EXTERN void tfLogFreeBuf(
    ttLogCtrlBlkPtr lcbPtr);
TM_PROTO_EXTERN int tfLogMsg(
    ttLogCtrlBlkPtr lcbPtr, const char TM_FAR *pszMsgPrefix,
    const char TM_FAR *pszFormat, ...);
TM_PROTO_EXTERN int tfLogWalk( ttLogWalkFuncPtr funcPtr,
                               ttLogCtrlBlkPtr lcbPtr,
                               int msgSeqNo,
                               ttUserGenericUnion genParam);
/* Use tfLogWalkNoAlloc instead of tfLogWalk when TM_MEMORY_LOGGING is
   enabled. NOTE: when using TM_MEMORY_LOGGING, avoid doing any operation
   that requires Treck memory allocation when walking the log buffer. For
   example, have your log walk function call the RTOS printf() function to
   output log messages, since that does not use any Treck memory. */
TM_PROTO_EXTERN int tfLogWalkNoAlloc( ttLogWalkFuncPtr funcPtr,
                                      ttLogCtrlBlkPtr lcbPtr,
                                      ttLogCtrlBlkPtr lcbCopyPtr,
                                      int lcbCopyLen,
                                      int msgSeqNo,
                                      ttUserGenericUnion genParam);
#if (defined(TM_OPTIMIZE_MANY_MHOMES) && !defined(TM_SINGLE_INTERFACE_HOME))
/* APIs for sorting IP addresses to improve performance of IP address lookup
   in the receive-path when many IP aliases are configured on a single
   interface. */

/* tfRemoveSortedIpAddr supports both IPv4 and IPv6 */
TM_PROTO_EXTERN void tfRemoveSortedIpAddr(
    ttDeviceEntryPtr removeDevEntryPtr, tt16Bit removeMHomeIndex,
    ttSortedIpAddrCachePtr ipAddrCachePtr);

TM_PROTO_EXTERN void tf4InsertSortedIpAddr(
    ttDeviceEntryPtr insertDevEntryPtr, tt16Bit insertMHomeIndex,
    ttSortedIpAddrCachePtr ipAddrCachePtr, tt8Bit dBroadFlag);
TM_PROTO_EXTERN ttIpAddrEntryPtr tf4LookupSortedIpAddr(
    tt4IpAddress ipAddr, int devIndex, ttSortedIpAddrCachePtr ipAddrCachePtr,
    tt8Bit dBroadFlag);
#endif /* TM_OPTIMIZE_MANY_MHOMES and not TM_SINGLE_INTERFACE_HOME */

/*
 * SNMP agent caching routines for ARP table cache, routing table cache,
 * UDP sockets table cache, and TCP vectors cache
 */
TM_PROTO_EXTERN void tfSnmpdCacheInsertRoute(
    ttRteEntryPtr rtePtr, int cacheIndex);
TM_PROTO_EXTERN void tfSnmpdCacheDeleteRoute(ttRteEntryPtr rtePtr);
TM_PROTO_EXTERN void tfSnmpdCacheDeleteSocket(ttSocketEntryPtr socketPtr);
TM_PROTO_EXTERN void tfSnmpdCacheInsertSocket(ttSocketEntryPtr socketPtr);

/* IPv4-specific functions supporting tfNg "Next Generation" APIs */
TM_PROTO_EXTERN int tf4ConfigInterface (
    ttUserInterface interfaceHandle,
    ttUserIpAddress ipAddress,
    ttUserIpAddress netMask,
    int             flags,
    int             buffersPerFrameCount,
    tt16Bit         mHomeIndex );

TM_PROTO_EXTERN int tf4UnConfigInterface (
    ttUserInterface interfaceHandle,
    tt16Bit         mHomeIndex );

/* NOTE: we don't yet have a tfNg function for tfAddInterfaceMhomeAddress */
TM_PROTO_EXTERN int tf4AddInterfaceMhomeAddress(
    ttUserInterface interfaceId,
    ttUserIpAddress ipAddress,
    tt16Bit mHomeIndex );


#ifdef TM_USE_NETSTAT
/* netstat related inernal functions */

/*
 * extract information from a routing entry for ARP
 * and store it in the ttNtArpEntry struct
 */
TM_PROTO_EXTERN int tfNtGetArpEntry(
    ttRteEntryPtr       rteEntryPtr,
    ttNtArpEntryPtr     ntArpEntryPtr);

/*
 * extract information from a routing entry
 * and store it in the ttNtRteEntry struct
 */
TM_PROTO_EXTERN int tfNtGetRteEntry(
    ttRteEntryPtr       rteEntryPtr,
    ttNtRteEntryPtr     ntRteEntryPtr);

/*
 * extract information from a socket entry
 * and store it in the ttNtTcpEntry struct
 */
TM_PROTO_EXTERN int tfNtGetTcpEntry(
    ttSocketEntryPtr    sockEntryPtr,
    ttNtTcpEntryPtr         ntTcpEntryPtr);

/*
 * extract information from a socket entry
 * and store it in the ttNtUdpEntry struct
 */
TM_PROTO_EXTERN int tfNtGetUdpEntry(
    ttSocketEntryPtr    sockEntryPtr,
    ttNtUdpEntryPtr         ntUdpEntryPtr);




/*
 * extract information from a devce entry
 * and store it in the ttNtDevEntry struct
 */
int tfNtGetDeviceEntry(
    ttDeviceEntryPtr    devPtr,
    ttNtDevEntryPtr     ntDevPtr);

/*
 * converts a HW address into a string in the format of FF:FF:...FF
 * returns the result string buffer if sucess,
 * returns NULL if input parameter is wrong */
TM_PROTO_EXTERN char TM_FAR * tfNtHwAddrToStr(
    ttUser8Bit TM_FAR * hwAddrPtr,
    int                 len,
    char TM_FAR *       buffer);

/* print route flags as a string */
TM_PROTO_EXTERN char TM_FAR * tfNtRteFlagToStr(
    ttUser16Bit     flags,
    char TM_FAR *   buffer);

/* print TCP state as a string */
TM_PROTO_EXTERN const char TM_FAR * tfNtGetTcpStateStr(ttUser8Bit state);
#endif /* TM_USE_NETSTAT */


#ifdef TM_USE_SNIFF
/* open a file, append an ethernet frame into it, and close it */
TM_PROTO_EXTERN int tfPcapWriteFrame(
    tt16Bit linkLayerProtocol,
    char*   dataPtr,
    int     dataLen);
TM_PROTO_EXTERN int tfPcapWritePacket(ttPacketPtr packetPtr);
#endif /* TM_USE_SNIFF */

/* if _WIN32 is defined, print in the right location with good color,
 * else do nothing
 */
int tfKernelPrint(int msgType, const char TM_FAR * format, ...);

/* Generic linked list, with count. */

/* Initilazing the list, must be called for a list before it is used */
void tfListInit(ttListPtr listPtr);

/* Add node at the head of a list */
void tfListAddToHead(ttListPtr listPtr, ttNodePtr nodePtr);

/* Add a node to the tail of a list */
void tfListAddToTail(ttListPtr listPtr, ttNodePtr nodePtr);

/* remove a node from the list */
void tfListRemove(ttListPtr listPtr, ttNodePtr nodePtr);

/* move a node to the front and keep the order of the rest of the nodes */
void tfListMoveToFront(ttListPtr listPtr, ttNodePtr nodePtr);

/* Replace a node with an new one */
void tfListReplace(ttNodePtr oldNodePtr, ttNodePtr newNodePtr);

/*
 * Traverse the list, for each node in the list, call the provided call
 * back function listCBFuncPtr with genParam,
 *
 * use to find node with customized matching cretiria, enum list,
 * update list, etc...
 *
 * continues if listCBFuncPtr returns TM_8BIT_NO,
 * stops and return the current node if the listCBFuncPtr returns TM_8BIT_YES
 * return TM_NODE_NULL_PTR if the whole list is Traversed without getting
 * a TM_8BIT_YES from the call back function.
 */
ttNodePtr tfListWalk(
    ttListPtr           listPtr,
    ttListCBFuncPtr     listCBFuncPtr,
    ttGenericUnion      genParam);

/* Free all the node in this list, you can only call tfListFree() if all the
 * nodes that have been allocated with tm_get_raw_buffer(), since tfListfree()
 * will call tm_free_raw_buffer for every node in the list.
 */
void tfListFree(ttListPtr listPtr);

typedef void (TM_CODE_FAR * ttAuthNotifyFuncPtr)(
    ttDeviceEntryPtr devEntryPtr,
    int              event,
    int              remoteLocalFlag,
    tt8Bit           needLockFlag);

TM_PROTO_EXTERN int tfEapOpen(ttVoidPtr  eapVectParmPtr,
                              tt8Bit     authPeerFlag);
TM_PROTO_EXTERN void tfEapIncomingPacket(ttVoidPtr    eapVectParmPtr,
                                         ttPacketPtr  packetPtr);
TM_PROTO_EXTERN int tfEapInit(ttVoidPtrPtr         eapVectParmPtrPtr,
                              ttDeviceEntryPtr     devEntryPtr,
                              ttAuthNotifyFuncPtr  notifyFuncPtr);
TM_PROTO_EXTERN int tfEapClose(ttVoidPtr eapVectParmPtr);



/*  Given the length of a buffer to hold a base64 encoded stream,
 *  compute the length of input buffer that can be accomodated.
 *  Note that this cannot be done acurately, since the actuall
 *  encode length of a buffer depends on the real content of the
 *  input buffer. This would be a estimation at the lower end.
 */
int tfBase64InputLen(int outLen);

/*  Given a buffer and length of the buffer, compute the length of
 *  the Base64 encoded stream.
 */
int tfBase64OutputLen(ttCharPtr inPtr, int inLen);

/*  BASE64 encoding routing */
int tfBase64Encode(ttCharPtr inPtr, int inLen, ttCharPtr outPtr, int outLen);

#ifdef TM_MULTIPLE_CONTEXT
/* Get a network ID that hasn't been used yet */
ttUser32Bit tfNetworkGetUnusedId(void);

/* Associate a interface to an network, this is to simulate plugging
 * a interface into a certain network
 */
int tfNetworkSetId(
    ttUserInterface interfaceHandle,
    ttUser32Bit     networkId);

#endif /* TM_MULTIPLE_CONTEXT */

/* Given the time lapsed since midnight 1970, Jan 1,
 * Get the time structure representation
 */
int tfGetUtcTime (
    ttConst32BitPtr daysPtr,
    ttConst32BitPtr secondsPtr,
    ttTimePtr        timePtr);

/* Convert the time strcuture to a string of the specified format
 * valid formats are
 * TM_TIME_FORMAT_HTTP     0
 * TM_TIME_FORMAT_SMTP     1
 */
ttCharPtr tfGetTimeString(ttTimePtr timePtr, ttCharPtr timeStrPtr,
                          int timeStrLen, int format);


/* tfHttpdGetCurrentTime function description
 *   Get the current time in HTTP date header format specified in
 *   [RFC1945]R3.3:20: UT format
 */
ttCharPtr tfHttpGetCurrentTime(ttCharPtr timeBufferPtr, int timeBufferLen);


#ifdef TM_USE_DOS_FS
/* We consider this directory our root, no files outside of this directory
 * is alloed to be acessed
 */
void tfSetDosRootDir(ttConstCharPtr rootDirPtr);

/* Set the DOS root directory to be the current working directory */
void tfInitDosFs(void);

#endif /* TM_USE_DOS_FS*/

/* Check whether a file matches to one of the filters provided as
 * a list of ttFileFilter
 */
int tfMatchFile (
    ttUserConstCharPtr  filePathPtr,
    ttListPtr           filterListPtr);


#ifdef __cplusplus
}
#endif

#endif /* _TRPROTO_H_ */

/* Processed by Treck Shipping Tool v2.37 
 * 2004/11/17 12:28:54      For: Marvell
 * File Name: trproto.h     File Revision: 4.1 
 * Project Name: tcpip.pj   Project Release: 4.1.2.1
 * Input checksum:  4957    Output checksum: 18929
 */
