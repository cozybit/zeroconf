#if !defined (_WL_MACROS_H_)
#define _WL_MACROS_H_
/*
 *                Copyright 2003, Marvell Semiconductor, Inc.
 * This code contains confidential information of Marvell Semiconductor, Inc.
 * No rights are granted herein under any patent, mask work right or copyright
 * of Marvell or any third party.
 * Marvell reserves the right at its sole discretion to request that this code
 * be immediately returned to Marvell. This code is provided "as is".
 * Marvell makes no warranties, express, implied or otherwise, regarding its
 * accuracy, completeness or performance.
 */

/*!
 * \file  wl_macros.h
 * \brief Common macros are defined here
 *
 *  Must include "wltypes.h" before this file 
 */

#define MACRO_START          do {
#define MACRO_END            } while (0)

#define WL_REGS8(x)     (*(volatile unsigned char *)(x))
#define WL_REGS16(x)    (*(volatile unsigned short *)(x))
#define WL_REGS32(x)    (*(volatile unsigned long *)(x))

#define WL_READ_REGS8(reg,val)      ((val) = WL_REGS8(reg))
#define WL_READ_REGS16(reg,val)     ((val) = WL_REGS16(reg))
#define WL_READ_REGS32(reg,val)     ((val) = WL_REGS32(reg))
#define WL_READ_BYTE(reg,val)       ((val) = WL_REGS8(reg))
#define WL_READ_HWORD(reg,val)      ((val) = WL_REGS16(reg)) /*half word; */
/*16bits */
#define WL_READ_WORD(reg,val)       ((val) = WL_REGS32(reg)) /*32 bits */
#define WL_WRITE_REGS8(reg,val)     (WL_REGS8(reg) = (val))
#define WL_WRITE_REGS16(reg,val)    (WL_REGS16(reg) = (val))
#define WL_WRITE_REGS32(reg,val)    (WL_REGS32(reg) = (val))
#define WL_WRITE_BYTE(reg,val)      (WL_REGS8(reg) = (val))
#define WL_WRITE_HWORD(reg,val)     (WL_REGS16(reg) = (val))  /*half word; */
/*16bits */
#define WL_WRITE_WORD(reg,val)      (WL_REGS32(reg) = (val))  /*32 bits */
#define WL_REGS8_SETBITS(reg, val)  (WL_REGS32(reg) |= (UINT8)(val))
#define WL_REGS16_SETBITS(reg, val) (WL_REGS32(reg) |= (UINT16)(val))
#define WL_REGS32_SETBITS(reg, val) (WL_REGS32(reg) |= (val))

#define WL_REGS8_CLRBITS(reg, val)  (WL_REGS32(reg) = \
                                     (UINT8)(WL_REGS32(reg)&~(val)))

#define WL_REGS16_CLRBITS(reg, val) (WL_REGS32(reg) = \
                                     (UINT16)(WL_REGS32(reg)&~(val)))

#define WL_REGS32_CLRBITS(reg, val) (WL_REGS32(reg) = \
                                     (WL_REGS32(reg)&~(val)))

/*!
 * Macro to put the WLAN SoC into sleep mode
 */
#define WL_GO_TO_SLEEP   asm volatile ("MCR p15, 0, r3, c7, c0, 4;")

/*!
 * Alignment macros
 */
#define ALIGN4BYTE(x)   (x = (( x+4-1)/4) * 4)
 
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef NELEMENTS
#define NELEMENTS(x) (sizeof(x)/sizeof(x[0]))
#endif

#endif /* _WL_MACROS_H_ */
