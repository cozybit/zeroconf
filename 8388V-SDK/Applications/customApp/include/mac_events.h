#ifndef _MAC_EVENTS_H_
#define _MAC_EVENTS_H_
/*******************************************************************
*
*    DESCRIPTION: Central registration file for MAC events ID
*
*    AUTHOR: cvu@marvell.com
*
*   The process for obtaining a MAC event ID is as follows:
*       1) Developer send email to yhou@marvell.com to request a MAC event ID
*       2) A new MAC event ID is assigned and recorded in the central
*          registration file
*       3) The developer is notified via email of the MAC event ID
*       4) The cental MAC event ID registration file is checked into Perforce
*       5) Email is sent to all branch owner to sync their local MAC event ID
*          header file with the latest.
*
*       The branch owners are responsible for keeping the local MAC event ID
*          header file in sync with the central registration file
*
*    HISTORY:
*
*    DATE:6/23/2005 Created
*
*******************************************************************/
/* Define the MAC events that are send to host */
#define DMA_DONE                 1
#define BCN_LOST_NO_SCAN         3
#define LINK_SENSE               4
#define DEAUTHENTICATE           8
#define DISASSOCIATE             9
#define PS_AWAKE                 10
#define PS_ASLEEP                11
#define SDIO_INTF_TX_DONE        12
#define WPA_MIC_ERROR_GROUP      13
#define WPA_MIC_ERROR_UNICAST    14
#define HOST_AWAKE               15
#define DS_AWAKE                 16
#define ADHOC_BCN_LOST           17
#define HOSTSLEEP_WAKE_UP        18
#define STOP_TX                  19   // Ask the driver to stop transmission.
#define START_TX                 20   // Ask the driver to start transmission.
#define CHANNEL_SWITCHED         21
#define MEASUREMENT_RESPONSE_RDY 22
#define WMM_STATUS_CHANGE        23
#define BG_SCAN_REPORT           24   /* Use for Background Scan Reporting */
#define RSSI_LOW                 25
#define SNR_LOW                  26
#define MAX_FAIL                 27
#define RSSI_HIGH                28
#define SNR_HIGH                 29
#define IBSS_COALESCED           30
#define BG_SCAN_TMO              31

#define MAC_EVT_BIT5       (1<<5)   /* 32 - Reserved, do not overwrite this */
#define MAC_EVT_BIT6       (1<<6)   /* 64 - Reserved, do not overwrite this */
#define MAC_EVT_BIT7       (1<<7)   /* 128 - Reserved, do not overwrite this */

#endif

