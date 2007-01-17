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
 * $Source: include/trpacket.h $
 *
 * Modification History
 * $Revision: 4.1 $
 * $Date: 2004/11/07 21:05:17PST $
 * $Author: xwang $
 * $ProjectName: /home/mks/proj/tcpip.pj $
 * $ProjectRevision: 4.1.2.1 $
 */
/*
 * Packet Driver Interface for Treck Real-Time TCP/IP
 *
 * This driver was taken from the NCSA Telnet Source
 * Code.  Since it is a derived work, it still remains
 * the property of NCSA.
 *
 * This version only supports ethernet
 *
 * Modification History
 * Date: $Date: 2004/11/07 21:05:17PST $
 * Author:
 * Version:  $Revision: 4.1 $
 * Release:  $ProjectRevision: 4.1.2.1 $
 * Description:
 */

#ifndef _TRPACKET_H_

#define _TRPACKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __BORLANDC__
#ifndef _MSC_VER
#error "This version of the packet driver supports only the Borland \
        and Microsoft C compilers"
#endif /* _MSC_VER */
#endif /* __BORLANDC__ */

/*#define TM_PD_DEBUG*/
#define TM_PD_INTERRUPT_NUMBER  0x60
#define TM_PD_RECV_BIG_BUFFERS     4
#define TM_PD_RECV_SMALL_BUFFERS   4
#define TM_PD_SMALL_BUFFER_SIZE  128

/*
 * IOCTL Defines
 */
#define TM_PD_IOCTL_REFILL         TM_DEV_IOCTL_REFILL_POOL_FLAG

/*
 * Types
 */

/*
 * Used for passing the ISR Function between functions
 */
#ifdef __BORLANDC__
typedef void TM_INTERRUPT (*ttPacketIsrFuncPtr)(unsigned short bp,
                                             unsigned short di,
                                             unsigned short si,
                                             unsigned short ds,
                                             unsigned short es,
                                             unsigned short dx,
                                             unsigned short cx,
                                             unsigned short bx,
                                             unsigned short ax);
#else /* _MSC_VER */
typedef void (_interrupt TM_FAR *ttPacketIsrFuncPtr)(unsigned int es,
                                                     unsigned int ds,
                                                     unsigned int di,
                                                     unsigned int si,
                                                     unsigned int bp,
                                                     unsigned int sp,
                                                     unsigned int bx,
                                                     unsigned int dx,
                                                     unsigned int cx,
                                                     unsigned int ax,
                                                     unsigned int ip,
                                                     unsigned int cs,
                                                     unsigned int flags);
#endif /* _MSC_VER */

/* Packet driver call back function typedef */
typedef void (*ttPacketWrapFuncPtr)(void);


/*
 * Driver Info
 */
typedef struct tsPacketDriverInfo
{
    unsigned short        pdVersion;
    unsigned short        pdType;
    unsigned char TM_FAR *pdNamePtr;
    unsigned char         pdClass;
    unsigned char         pdNumber;
    unsigned char         pdFunctionality;
}ttPacketDriverInfo;

/*
 * Macro to convert a segment and offset to a far pointer.  Borland and
 * Microsoft use different methods for doing this.  If the segment:offset
 * address was 0x1111:0x2222, Borland would represent this as 0x00113322 and
 * Microsoft would represent it as 0x11112222.
 */
#ifdef __BORLANDC__
/*
#define tm_make_far_ptr(segment,offset) ((void TM_FAR *)\
        ((unsigned long)((segment&0xffff)<<8) + \
        (unsigned long)(offset&0xffff)))

#define tm_make_far_ptr(segment,offset) ((void _far *)\
        ( ( ( (unsigned long) (segment&0xffff) ) << 8) + \
          ( (unsigned long)(offset & 0xffff) ) ) )
*/
#define tm_make_far_ptr(segment,offset) ((void _far *)\
        ( ( ( (unsigned long) (segment&0xffff) ) << 16) + \
          ( (unsigned long)(offset & 0xffff) ) ) )

#else /* _MSC_VER */
#define tm_make_far_ptr(segment,offset) ((void _far *)\
        ( ( ( (unsigned long) (segment&0xffff) ) << 16) + \
          ( (unsigned long)(offset & 0xffff) ) ) )
#endif /* _MSC_VER */


#define tm_far_ptr_segment(farPtr) FP_SEG(farPtr)
#define tm_far_ptr_offset(farPtr)  FP_OFF(farPtr)

#ifdef _MSC_VER
#if _MSC_VER >= 800
#define tm_software_irq_x    _int86x
#define tm_software_irq      _int86
#else
#define tm_software_irq_x    int86x
#define tm_software_irq      int86
#endif
#else
#define tm_software_irq_x    int86x
#define tm_software_irq      int86
#endif

#define tm_save_segment_regs segread

#define TM_REGS_TYPE       union REGS
#define TM_SEG_REGS_TYPE   struct SREGS

/*
 * Packet driver classes
 */

#define TM_PD_CLASS_ETHERNET     1

/* Packet driver types */
#define TM_PD_IP_LENGTH       2
#define TM_PD_ARP_LENGTH      2

#define TM_PD_ETHER_ADDR_LEN  6
#define TM_PD_ETHER_MAX_FRAME 1514


/*
 * Functionality Codes
 */
#define TM_PD_FUNC_NOT_INSTALLED   0xff

/*
 * Low Level Functions performed by packet driver
 */
#define TM_PD_FUNCTION_INFO         0x01FF
#define TM_PD_FUNCTION_ACCESS_TYPE  0x0200
#define TM_PD_FUNCTION_RELEASE_TYPE 0x0300
#define TM_PD_FUNCTION_SEND         0x0400
#define TM_PD_FUNCTION_TERMINATE    0x0500
#define TM_PD_FUNCTION_GET_ADDR     0x0600
#define TM_PD_FUNCTION_RESET        0x0700

#define TM_PD_ENOERROR 0
#define TM_PD_ERROR   -1

int tfPacketOpen(ttUserInterface interfaceHandle);

int tfPacketGetPhyAddr( ttUserInterface interfaceHandle,
                        char    TM_FAR * physicalAddress );

int tfPacketClose(ttUserInterface interfaceHandle);

int tfPacketSend(ttUserInterface interfaceHandle,
                 char    TM_FAR *dataPtr,
                 int             dataLength,
                 int             flag);

int tfPacketIoctl(ttUserInterface interfaceHandle, int flag,
                  void TM_FAR * optionPtr, int optionLen);

int tfPacketRecv(ttUserInterface  interfaceHandle,
                 char TM_FAR    **dataPtrPtr,
                 int TM_FAR      *dataLengthPtr,
                 ttUserBufferPtr  userBufferHandlePtr);

#ifdef __cplusplus
}
#endif

#endif /* _TRPACKET_H_ */

/* Processed by Treck Shipping Tool v2.37 
 * 2004/11/17 12:28:53      For: Marvell
 * File Name: trpacket.h    File Revision: 4.1 
 * Project Name: tcpip.pj   Project Release: 4.1.2.1
 * Input checksum: 42913    Output checksum: 57145
 */
