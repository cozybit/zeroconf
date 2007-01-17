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
 * \file    dot11d.h
 * \brief   802.11d header file
 */

#ifndef _DOT11D_H_
#define _DOT11D_H_


#define MAX_CHANNELS 64
#define MAX_PWR      20
#define COUNTRY_CODE_SZ 3

#define MAX_CHANNELS_2_4GHZ 14
#define MAX_CHANNELS_5GHZ 35

typedef struct dot11MultiDomainCapabilityEntry_s  
{
    UINT8    countryCode[COUNTRY_CODE_SZ];
    UINT8    numChannels;
    UINT8    channelList[MAX_CHANNELS];
    UINT8    maxPwrDbm[MAX_CHANNELS];
} dot11MultiDomainCapabilityEntry_t;

typedef PACK_START struct channelPwrTriplet_t 
{
    UINT8    firstChan;
    UINT8    numChan;
    UINT8    chanPwr;
}  PACK_END channelPwrTriplet_t;

typedef struct StaPowerCapability_t {
    Boolean valid;
    uint8 minTxPwr;
    uint8 maxTxPwr;
} StaPowerCapability_t;

/* Global Functions */
extern Boolean isDot11dDomainSet(void);
extern Boolean isDot11dDomainSame(UINT8* countryStr); 
extern Boolean getChanIdx(UINT8 chan, UINT8* idx);
extern UINT8 makeCountryIE (uint8 *countryIE);
extern void initDot11CountryInfo (void);
extern Boolean ParseCountryIE (uint8 *countryIE);
extern UINT8 GetDot11DDomainInfo (uint8 *buf);
extern Boolean SetDot11DDomainInfo (uint8 *buf);
extern UINT8 dot11dPwrCheck (UINT8 dBm);

/* Global Data */
extern StaPowerCapability_t thisStaPwrCap;
extern dot11MultiDomainCapabilityEntry_t countryInfo;

#endif   /*_DOT11D_H_*/

