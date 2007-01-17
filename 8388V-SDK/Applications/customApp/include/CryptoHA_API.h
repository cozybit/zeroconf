/******************** (c) Marvell Semiconductor, Inc., 2006 *******************
 * This code contains confidential information of Marvell Semiconductor, Inc.
 * No rights are granted herein under any patent, mask work right or copyright
 * of Marvell or any third party.
 * Marvell reserves the right at its sole discretion to request that this code
 * be immediately returned to Marvell. This code is provided "as is".
 * Marvell makes no warranties, expressed, implied or otherwise, regarding its
 * accuracy, completeness or performance. 
 *
 * Purpose:
 *    This file contains the function prototypes and definitions for the
 *    CryptoHA (Hardware Acceleration) API.
 *****************************************************************************/
#ifndef _CRYPTOHA_API_H_
#define _CRYPTOHA_API_H_

//=============================================================================
//                            PUBLIC DEFINITIONS
//=============================================================================

/* Error Codes */
#define E_CRYPTOHA_INVALID_SESSION_ID   0xFFFF0001
#define E_CRYPTOHA_INQ_FULL             0xFFFF0002
#define E_CRYPTOHA_NO_CALLBACK          0xFFFF0003
#define E_CRYPTOHA_INVALID_DESC         0xFFFF0004
#define E_CRYPTOHA_AUTHENTICATION_FAIL  0xFFFF0005
#define E_CRYPTOHA_NO_SESSION_AVAIL     0xFFFF0006
#define E_CRYPTOHA_INVALID_PARAM        0xFFFF0007
#define E_CRYPTOHA_UNKNOWN              0xFFFF0008

/* ProcessingOrder */
#define CRYPTOHA_ENCRYPT_THEN_HASHING  0x00010001
#define CRYPTOHA_HASHING_THEN_ENCRYPT  0x00010002
#define CRYPTOHA_DECRYPT_THEN_HASHING  0x00010003
#define CRYPTOHA_AUTHENTICATION_THEN_DECRYPT   0x00010004

/* OpFlag */
#define CRYPTOHA_NO_WAIT_INQ 0x00000001
#define CRYPTOHA_NO_WAIT_COMPLETION 0x00000002

#define CRYPTOHA_MAX_DATA_TOTAL_SPAN 1648    /* Max allowable total span of input data 
                                                This size is determined by the limited internal SRAM space
                                             */
                                            /* Due to the limitation in Kite3 IPS IDMA hardware,
                                               IDMA source and destination in SRAM must be
                                               16 bytes aligned, and the size of IDMA 
                                               also must be multiple of 16 bytes.
                                               According to ASIC engineer, he will fix this
                                               issue at the next SoC design because
                                               it is not feasible to fix by jusy ECO.
                                            */   
/* Max supported Sessions */
/* This must be even number to make DataBuf[] to be 16 byets aligned */
#define CRYPTOHA_MAX_SESSION 16

/* Encryption/Decryption algorithm */
typedef enum
{
    CRYPTOHA_CIPHER_DES        = 1,
    CRYPTOHA_CIPHER_TDES       = 2,
    CRYPTOHA_CIPHER_AES        = 3
} CRYPTOHA_CIPHER_ALGORITHM;

/* Hash algorithm */
typedef enum 
{   
    CRYPTOHA_HASH_MD5         = 0,
    CRYPTOHA_HASH_SHA1        = 1,
    CRYPTOHA_HASH_HMAC_MD5    = 2,
    CRYPTOHA_HASH_HMAC_SHA1   = 3,
} CRYPTOHA_HASH_ALGORITHM;

/* Encryption/Decryption Mode of Operation */
typedef enum 
{
    CRYPTOHA_CONFIDENTIALITY_MODE_ECB = 0,
    CRYPTOHA_CONFIDENTIALITY_MODE_CBC = 1,
} CRYPTOHA_CONFIDENTIALITY_MODE;

/* AES Key Length */
typedef enum 
{
    CRYPTOHA_AES_KEY_128 = 0,
    CRYPTOHA_AES_KEY_192 = 1,
    CRYPTOHA_AES_KEY_256 = 2,
} CRYPTOHA_AES_KEY_SIZE;

/* Triple DES Variant */
typedef enum 
{
    CRYPTOHA_TDES_EEE = 0,
    CRYPTOHA_TDES_EDE = 1,
} CRYPTOHA_TDES_VARIANT;

//=============================================================================
//                          PUBLIC TYPE DEFINITIONS
//=============================================================================

typedef struct _CryptoHA_CipherDesc
{
    UINT8 CipherAlgorithm;  /* Block cipher algorithm to be used for encryption
                               /decryption
                                1 = DES
                                2 = Triple-DES
                                3 = AES 
                            */
    UINT8 ConfidentialityMode;  /* Confidentiality mode of operation for use with an 
                                    underlying block cipher algorithm
                                    0 = ECB
                                    1 = CBC
                                */
    UINT8 AesKeySize;   /* Three Key Size are supported for AES encryption/decryption
                                    0 = 128 bit key
                                    1 = 192 bit key
                                    2 = 256 bit key 
                                This field is ignored in DES/TDES.
                        */
    UINT8 TripleDesVariant; /* Triple DES Variant
                                    0 = EEE
                                    1 = EDE
                                This field is ignored in AES.
                            */
} CryptoHA_CipherDesc;


typedef struct _CryptoHA_HashDesc
{
    UINT8 HashAlgorithm;    /* Hash algorithm
                                0 = MD5
                                1 = SHA1
                                2 = HMAC-MD5
                                3 = HMAC-SHA1
                            */
    UINT8 Truncation;   /* Hash result is truncated before comparing against 
                            HMAC authentication value.
                                0 = Full size (128 bit in MD5, 160 bit in SHA1)
                                1 = truncated 96 bit

                            This field is only applied to CryptoHA_DecryptAndHashing 
                                with CRYPTOHA_ AUTHENTICATION_THEN_DECRYPT.

                        */
} CryptoHA_HashDesc;

typedef struct _CryptoHA_CipherData
{
    UINT8 *InputDataAddr;   /* Start address of input data to be encrypted or decrypted
                                This buffer must be located in SQM (SQ Memory) area
                                for hardware DMA.
                            */
    UINT32 DataLen; /* Length of data in InputDataAddr buffer
                        It should be multiple of a cipher block size which is 
                           8 bytes for DES/TDES
                          16 bytes for AES
                        DataLen should be equal or less than 1632 bytes.
                    */
    UINT8 *OutputDataAddr;  /* Start address of the buffer to store output data after 
                               encryption or decryption.
                                This buffer must be located in SQM (SQ Memory) area
                                for hardware DMA.
                            */
    UINT8 *EIV;    /* Pointer to a byte stream that contains the initial values 
                        for encryption/decryption
                           DES/TDES
                               EIV[0] ~ EIV[7]
                           AES
                               EIV[0] ~ EIV[15]
                           
                           Ex) Initial Value = 1234567890abcdef
                              EIV [0] = 0x12
                              EIV [1] = 0x34
                              EIV [2] = 0x56
                              EIV [3] = 0x78
                              …
                              EIV [7] = 0xef
                    */
} CryptoHA_CipherData;


typedef struct _CryptoHA_HashData
{
    UINT8 *InputDataAddr;   /* Start address of input data to be hashed.
                                This buffer must be allocated in SQM (SQ Memory) area
                                for hardware DMA.
                            */
    UINT32 DataLen; /* Length of data in InputDataAddr buffer
                        It doesn't have to be multiple of hash block size because 
                        the padding will be made by crypto hardware automatically.
                        DataLen should be equal or less than 1632 bytes.
                    */
    UINT8 *OutputDataAddr;  /* Start address of the buffer to store output data after 
                                encryption or decryption. 
                                Size of output data
                                    MD5 / HMAC-MD5 = 16 bytes
                                    SHA1 / HMAC-SHA1 = 20 bytes
                                This buffer must be allocated in SQM (SQ Memory) area
                                for hardware DMA.
                            */
    UINT8 *AuthenticationData; /* Pointer to authentication data received from the peer
                                    MD5 / HMAC-MD5 = 16 bytes
                                    SHA1 / HMAC-SHA1 = 20 bytes

                                    This field is only applied to CryptoHA_DecryptAndHashing
                                    with CRYPTOHA_ AUTHENTICATION_THEN_DECRYPT.
                                */
} CryptoHA_HashData;

typedef void (*CRYPTOHA_COMPLETION_CALLBACK) ( UINT32 SessionId,
                                               UINT32 Status,
                                               UINT32 CallbackTag,
                                               UINT8 *CipherOutputDataAddr,
                                               UINT8 *HashOutputDataAddr );

//=============================================================================
//                    PUBLIC PROCEDURES (ANSI Prototypes)
//=============================================================================

extern void CryptoHA_Init(void);
extern UINT32 CryptoHA_OpenSession(UINT32 *SessionId,
                                   CryptoHA_CipherDesc *CipherDesc,
                                   CryptoHA_HashDesc *HashDesc);
extern UINT32 CryptoHA_CloseSession(UINT32 SessionId);
extern UINT32 CryptoHA_UpdateKeys(UINT32 SessionId,
                                  UINT8 *EKey,
                                  UINT8 *HKey);
extern UINT32 CryptoHA_Encrypt(UINT32 SessionId,
                               CryptoHA_CipherData * CipherData,
                               UINT32 CallbackTag,
                               UINT32 OpFlag);
extern UINT32 CryptoHA_Decrypt(UINT32 SessionId,
                               CryptoHA_CipherData * CipherData,
                               UINT32 CallbackTag,
                               UINT32 OpFlag);
extern UINT32 CryptoHA_Hashing(UINT32 SessionId,
                               CryptoHA_HashData * HashData,
                               UINT32 CallbackTag,
                               UINT32 OpFlag);
extern UINT32 CryptoHA_RegisterCompletionCallback(UINT32 SessionId,
                                                  CRYPTOHA_COMPLETION_CALLBACK CompletionCallback);
extern UINT32 CryptoHA_EncryptWithHashing(UINT32 SessionId,
                                          UINT32 ProcessingOrder,
                                          CryptoHA_CipherData * CipherData,
                                          CryptoHA_HashData * HashData,
                                          UINT32 CallbackTag,
                                          UINT32 OpFlag);
extern UINT32 CryptoHA_DecryptWithHashing(UINT32 SessionId,
                                          UINT32 ProcessingOrder,
                                          CryptoHA_CipherData * CipherData,
                                          CryptoHA_HashData * HashData,
                                          UINT32 CallbackTag,
                                          UINT32 OpFlag);
extern void CryptoHA_Deinit(void);

#endif /* _CRYPTOHA_API_H_ */
