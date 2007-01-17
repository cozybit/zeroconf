#if !defined(_WLTYPES_H_)
#define _WLTYPES_H_ 
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
 * \file    wltypes.h
 * \brief   Basic types common to all the modules must be defined in this file.
 *
*/

 /** @defgroup DataTypes Data Types used in SDK
 *  Functions exported by wltypes.h
 *  @{
 */
/*! Create type names for native C types for portability reasons */
typedef unsigned long long  UINT64;		//!< 64 bit unsigned
typedef signed long long    SINT64;		//!< 64 bit signed
typedef unsigned long       UINT32;		//!< 32 bit unsigned
typedef signed long         SINT32;		//!< 32 bit signed
typedef unsigned short      UINT16;		//!< 16 bit unsigned
typedef signed short        SINT16;		//!< 16 bit signed
typedef unsigned char       UINT8;		//!< 8 bit unsigned
typedef signed char         SINT8;		//!< 8 bit signed

typedef unsigned long long  uint64;		//!< 64 bit unsigned
typedef signed long long    sint64;		//!< 64 bit signed
typedef unsigned long       uint32;		//!< 32 bit unsigned

typedef signed long         sint32;		//!< 32 bit signed
typedef unsigned short      uint16;		//!< 16 bit unsigned
typedef signed short        sint16;		//!< 16 bit signed
typedef unsigned char       uint8;		//!< 8 bit unsigned
typedef signed char         sint8;		//!< 8 bit signed
typedef signed char         int8;		//!< 8 bit signed
typedef int                 BOOLEAN;	//!< boolean	
typedef int                 Boolean;    //!< boolean	
typedef signed int          BIT_FIELD;	//!< bit field

typedef struct _REG8_ADDR_N_VAL_
{
    UINT8 reg;
    UINT8 val;
} REG8_ADDR_N_VAL;

#define INIT_REGS	REG8_ADDR_N_VAL

/*! Generic status code */
#define WL_STATUS_OK         0		//!< ok
#define WL_STATUS_ERR       -1		//!< error
#define WL_STATUS_BAD_PARAM -2		//!< bad parameter

/*! BOOLEAN values */
#define FALSE 0		//!< False
#define TRUE  1		//!< True

/**        
*** @brief Enumeration of Status type
**/        
typedef enum
{
    SUCCESS,	//!< 0
    FAIL		//!< 1
} Status_e;

typedef Status_e WL_STATUS;

/*! Value for NULL pointer */
#undef NULL
#define NULL ((void *)0)			//!< null

#define PACK_START   __packed		//!< packed structure
#define PACK_END					//!< end of packed structure

#define ALLIGNED_START(x) __align(x)	//!< allignment macro
#define ALLIGNED_END(x)					//!< allignment end

#define INLINE

#define BIT0	(0x00000001 << 0)
#define BIT1	(0x00000001 << 1)
#define BIT2	(0x00000001 << 2)
#define BIT3	(0x00000001 << 3)
#define BIT4	(0x00000001 << 4)
#define BIT5	(0x00000001 << 5)
#define BIT6	(0x00000001 << 6)
#define BIT7	(0x00000001 << 7)
#define BIT8	(0x00000001 << 8)
#define BIT9	(0x00000001 << 9)
#define BIT10	(0x00000001 << 10)
#define BIT11	(0x00000001 << 11)
#define BIT12	(0x00000001 << 12)
#define BIT13	(0x00000001 << 13)
#define BIT14	(0x00000001 << 14)
#define BIT15	(0x00000001 << 15)
#define BIT16	(0x00000001 << 16)
#define BIT17	(0x00000001 << 17)
#define BIT18	(0x00000001 << 18)
#define BIT19	(0x00000001 << 19)
#define BIT20	(0x00000001 << 20)
#define BIT21	(0x00000001 << 21)
#define BIT22	(0x00000001 << 22)
#define BIT23	(0x00000001 << 23)
#define BIT24	(0x00000001 << 24)
#define BIT25	(0x00000001 << 25)
#define BIT26	(0x00000001 << 26)
#define BIT27	(0x00000001 << 27)
#define BIT28	(0x00000001 << 28)
#define BIT29	(0x00000001 << 29)
#define BIT30	(0x00000001 << 30)
#define BIT31	(0x00000001 << 31)

// VOLT_RESOLUTION_50mV 
#define VOLT_0_5    10  // 10x50mV = 5x100mV
#define VOLT_0_6    12  // 12x50mV = 6x100mV
#define VOLT_0_7    14  // 14x50mV = 7x100mV
#define VOLT_0_7_5  15  // 15x50mV = 7.5x100mV
#define VOLT_0_8    16  // 16x50mV = 8x100mV
#define VOLT_0_9    18  // 18x50mV = 9x100mV
#define VOLT_1_0    20  // 20x50mV = 10x100mV
#define VOLT_1_0_5  21  // 21x50mV
#define VOLT_1_1    22  // 22x50mV = 11x100mV
#define VOLT_1_1_5  23  // 23x50mV
#define VOLT_1_2    24  // 24x50mV = 12x100mV
#define VOLT_1_2_5  25  // 25x50mV
#define VOLT_1_3    26  // 26x50mV = 13x100mV
#define VOLT_1_4    28  // 28x50mV = 14x100mV
#define VOLT_1_5    30  // 30x50mV = 15x100mV
#define VOLT_1_6    32  // 32x50mV = 16x100mV
#define VOLT_1_7    34  // 34x50mV = 17x100mV
#define VOLT_1_8    36  // 36x50mV = 18x100mV
#define VOLT_1_9    38  // 38x50mV = 19x100mV
#define VOLT_2_0    40  // 40x50mV = 20x100mV
#define VOLT_2_1    42  // 42x50mV = 21x100mV
#define VOLT_2_2    44  // 44x50mV = 22x100mV
#define VOLT_2_3    46  // 46x50mV = 23x100mV
#define VOLT_2_4    48  // 48x50mV = 24x100mV
#define VOLT_2_5    50  // 50x50mV = 25x100mV
#define VOLT_2_6    52  // 52x50mV = 26x100mV
#define VOLT_2_7    54  // 54x50mV = 27x100mV
#define VOLT_2_8    56  // 56x50mV = 28x100mV
#define VOLT_2_9    58  // 58x50mV = 29x100mV
#define VOLT_3_0    60  // 60x50mV = 30x100mV
#define VOLT_3_1    62  // 62x50mV = 31x100mV
#define VOLT_3_2    64  // 64x50mV = 32x100mV
#define VOLT_3_3    66  // 66x50mV = 33x100mV
#define VOLT_3_4    68  // 68x50mV = 34x100mV
#define VOLT_3_5    70  // 70x50mV = 35x100mV
#define VOLT_3_6    72  // 72x50mV = 36x100mV



#ifdef BE /* Big Endian */
#define SHORT_SWAP(X) (X)
#define WORD_SWAP(X) (X)
#define LONG_SWAP(X) ((l64)(X))
#else    /* Little Endian */

#define SHORT_SWAP(X) ((X <<8 ) | (X >> 8))		//!< swap bytes in a 16 bit short

#define WORD_SWAP(X) (((X)&0xff)<<24)+      \
                    (((X)&0xff00)<<8)+      \
                    (((X)&0xff0000)>>8)+    \
                    (((X)&0xff000000)>>24)		//!< swap bytes in a 32 bit word

#define LONG_SWAP(X) ( (l64) (((X)&0xffULL)<<56)+               \
                            (((X)&0xff00ULL)<<40)+              \
                            (((X)&0xff0000ULL)<<24)+            \
                            (((X)&0xff000000ULL)<<8)+           \
                            (((X)&0xff00000000ULL)>>8)+         \
                            (((X)&0xff0000000000ULL)>>24)+      \
                            (((X)&0xff000000000000ULL)>>40)+    \
                            (((X)&0xff00000000000000ULL)>>56))		//!< swap bytes in a 64 bit long
#endif
/*@}*/
#endif /* _WLTYPES_H_ */
