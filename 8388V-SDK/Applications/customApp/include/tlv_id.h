#ifndef _TLV_ID_H_
#define _TLV_ID_H_
/*******************************************************************
*
*   DESCRIPTION: TLV identifiers
*   This is the central registration file for TLV ID.
*
*   The process for obtaining a TLV ID is as follows:
*       1) Developer send email to yhou@marvell.com to request a TLV ID
*       2) A new TLV ID is assigned and recorded in the central registration
*          file
*       3) The developer is notified via email of the TLV ID
*       4) The cental TLV ID registration file is checked into Perforce
*       5) Email is sent to all branch owner to sync their local tlv id header
*          file with the latest.
*
*          The branch owners are responsible for keeping the local tlv id
*          header file in sync with the central registration file
*
*    AUTHOR: cvu@marvell.com
*
*    HISTORY:
*
*    DATE:6/23/2005 Created
*
*******************************************************************/

#define PROPRIETARY_TLV_BASE_ID         0x0100

/* Terminating TLV Type */
#define MRVL_TERMINATE_TLV_ID           0xffff

/* Defines for MRVL TLV IDs*/

/* IEEE TLVs*/
#define MRVL_SSID_TLV_ID                0x0000
#define MRVL_RATES_TLV_ID               0x0001
#define MRVL_PHYPARAMFHSET_TLV_ID       0x0002
#define MRVL_PHYPARAMDSSET_TLV_ID       0x0003
#define MRVL_CFPARAMSET_TLV_ID          0x0004
#define MRVL_IBSSPARAMSET_TLV_ID        0x0006
#define MRVL_COUNTRY_TLV_ID             0x0007
#define MRVL_PWR_CONSTRAINT_TLV_ID      0x0020
#define MRVL_PWR_CAPABILITY_TLV_ID      0x0021
#define MRVL_SUPPORTEDCHANNELS_TLV_ID   0x0024
#define MRVL_QUIET_TLV_ID               0x0028
#define MRVL_IBSSDFS_TLV_ID             0x0029
#define MRVL_QOSCAPABILITY_TLV_ID       0x002E
#define MRVL_RSN_TLV_ID                 0x0030
#define MRVL_VENDORSPECIFIC_TLV_ID      0x00DD

/* Proprietary TLVs */
#define MRVL_KEYPARAMSET_TLV_ID         (PROPRIETARY_TLV_BASE_ID + 0)
#define MRVL_CHANNELLIST_TLV_ID         (PROPRIETARY_TLV_BASE_ID + 1)
#define MRVL_NUMPROBES_TLV_ID           (PROPRIETARY_TLV_BASE_ID + 2)
#define MRVL_OMNI_TLV_ID                (PROPRIETARY_TLV_BASE_ID + 3)
#define MRVL_RSSITHRESHOLD_TLV_ID       (PROPRIETARY_TLV_BASE_ID + 4)
#define MRVL_SNRTHRESHOLD_TLV_ID        (PROPRIETARY_TLV_BASE_ID + 5)
#define MRVL_LOWRSSITHRESHOLD_TLV_ID    MRVL_RSSITHRESHOLD_TLV_ID
#define MRVL_LOWSNRTHRESHOLD_TLV_ID     MRVL_SNRTHRESHOLD_TLV_ID
#define MRVL_FAILURECOUNT_TLV_ID        (PROPRIETARY_TLV_BASE_ID + 6)
#define MRVL_BEACONMISSED_TLV_ID        (PROPRIETARY_TLV_BASE_ID + 7)
#define MRVL_LEDGPIO_TLV_ID             (PROPRIETARY_TLV_BASE_ID + 8)
#define MRVL_LEDBEHAVIOR_TLV_ID         (PROPRIETARY_TLV_BASE_ID + 9)
#define MRVL_PASSTHROUGH_TLV_ID         (PROPRIETARY_TLV_BASE_ID + 10)
#define MRVL_REASSOCAP_TLV_ID           (PROPRIETARY_TLV_BASE_ID + 11)
#define MRVL_POWER_TBL_2_4GHZ_TLV_ID    (PROPRIETARY_TLV_BASE_ID + 12)
#define MRVL_POWER_TBL_5GHZ_TLV_ID      (PROPRIETARY_TLV_BASE_ID + 13)
/* sending Brocast SSID */
#define MRVL_BCASTPROBE_TLV_ID          (PROPRIETARY_TLV_BASE_ID + 14)
/* number of SSIDs for which directed probes need to be generated */
#define MRVL_NUMSSIDPROBE_TLV_ID        (PROPRIETARY_TLV_BASE_ID + 15)
#define MRVL_WMMQSTATUS_TLV_ID          (PROPRIETARY_TLV_BASE_ID + 16)
#define MRVL_CRYPTO_TLV_ID              (PROPRIETARY_TLV_BASE_ID + 17)
#define MRVL_WILDCARD_SSID_TLV_ID       (PROPRIETARY_TLV_BASE_ID + 18)
#define MRVL_TSFTIMESTAMP_TLV_ID        (PROPRIETARY_TLV_BASE_ID + 19)
#define MRVL_POWER_ADAPT_CFG_TLV_ID     (PROPRIETARY_TLV_BASE_ID + 20)
#define MRVL_HOSTSLEEP_FILTER_TYPE1_TLV_ID  (PROPRIETARY_TLV_BASE_ID + 21)
#define MRVL_HIGHRSSITHRESHOLD_TLV_ID   (PROPRIETARY_TLV_BASE_ID + 22)
#define MRVL_HIGHSNRTHRESHOLD_TLV_ID    (PROPRIETARY_TLV_BASE_ID + 23)
#define MRVL_AUTOTX_TLV_ID              (PROPRIETARY_TLV_BASE_ID + 24)
#define MRVL_WSC_SELECTED_REGISTRAR_TLV (PROPRIETARY_TLV_BASE_ID + 25)
#define MRVL_WSC_ENROLLEE_TMO_TLV       (PROPRIETARY_TLV_BASE_ID + 26)
#define MRVL_WSC_ENROLLEE_PROBE_REQ_TLV (PROPRIETARY_TLV_BASE_ID + 27)
#define MRVL_WSC_REGISTRAR_BEACON_TLV   (PROPRIETARY_TLV_BASE_ID + 28)
#define MRVL_WSC_REGISTRAR_PROBE_RESP_TLV   (PROPRIETARY_TLV_BASE_ID + 29)
#define MRVL_STARTBGSCANLATER_TLV_ID    (PROPRIETARY_TLV_BASE_ID + 30)
#define MRVL_AUTHTYPE_TLV_ID            (PROPRIETARY_TLV_BASE_ID + 31)
#define MRVL_STA_MAC_TLV_ID             (PROPRIETARY_TLV_BASE_ID + 32)

#endif
