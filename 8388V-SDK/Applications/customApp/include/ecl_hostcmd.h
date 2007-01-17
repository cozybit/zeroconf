#ifndef _ECL_HOSTCMD_H_
#define _ECL_HOSTCMD_H_
/**
 * @file    ecl_hostcmd.h
 * @brief   command code and command response descriptor
 * @author (c) Marvell Semiconductor, Inc.
 * Created 10/21/2004
 *
 * REVISION HISTORY:
 *
 */

/*******************************************************************
*
*    DESCRIPTION:
*    This file contains the command code for commands between the host
*    driver and the WLAN Firmware
*
*    AUTHOR:
*
*    HISTORY:
*
*    DATE:10/21/2004
*
*******************************************************************/
/*
   Command code (16-bit)
*/
typedef enum
{
    host_MSG_NONE                          = 0x0000,
    host_CMD_CODE_DNLD                     = 0x0001,
    host_CMD_OP_PARAM_DNLD                 = 0x0002,
    host_CMD_GET_HW_SPEC                   = 0x0003,
    host_CMD_EEPROM_UPDATE                 = 0x0004,
    host_CMD_802_11_RESET                  = 0x0005,
    host_CMD_802_11_SCAN                   = 0x0006,
    host_CMD_802_11_QUERY_TRAFFIC          = 0x0009,
    host_CMD_802_11_QUERY_STATUS           = 0x000a,
    host_CMD_802_11_GET_LOG                = 0x000b,
    host_CMD_MAC_MULTICAST_ADR             = 0x0010,
    host_CMD_802_11_AUTHENTICATE           = 0x0011,
    host_CMD_802_11_ASSOCIATE_OLD          = 0x0012,
    host_CMD_802_11_SET_WEP                = 0x0013,
    host_CMD_802_11_GET_STAT               = 0x0014,
    host_CMD_802_3_GET_STAT                = 0x0015,
    host_CMD_802_11_SNMP_MIB               = 0x0016,
    host_CMD_MAC_REG_MAP                   = 0x0017,
    host_CMD_BBP_REG_MAP                   = 0x0018,
    host_CMD_RF_REG_MAP                    = 0x0023,
    host_CMD_MAC_REG_ACCESS                = 0x0019,
    host_CMD_BBP_REG_ACCESS                = 0x001a,
    host_CMD_RF_REG_ACCESS                 = 0x001b,
    host_CMD_802_11_RADIO_CONTROL          = 0x001c,
    host_CMD_802_11_RF_CHANNEL             = 0x001d,
    host_CMD_802_11_RF_TX_POWER            = 0x001e,
    host_CMD_802_11_RSSI                   = 0x001f,
    host_CMD_802_11_RF_ANTENNA             = 0x0020,
    host_CMD_802_11_PS_MODE                = 0x0021,
    host_CMD_802_11_DATA_RATE              = 0x0022,
    host_CMD_802_11_DEAUTHENTICATE         = 0x0024,
    host_CMD_802_11_REASSOCIATE            = 0x0025,
    host_CMD_802_11_DISASSOCIATE           = 0x0026,
    host_CMD_MAC_CONTROL                   = 0x0028,
    host_CMD_802_11_QUERY_SCAN_RESULT      = 0x0029,
    host_CMD_802_11_GM                     = 0x002a,
    host_CMD_802_11_AD_HOC_START           = 0x002b,
    host_CMD_802_11_AD_HOC_JOIN            = 0x002c,
    host_CMD_802_11_QUERY_RSN_OPTION       = 0x002d,
    host_CMD_802_11_QUERY_TKIP_REPLY_CNTRS = 0x002e,
    host_CMD_802_11_ENABLE_RSN             = 0x002f,
    host_CMD_802_11_CONFIG_RSN             = 0x0030,    // removed
    host_CMD_802_11_UNICAST_CIPHER         = 0x0031,
    host_CMD_802_11_MCAST_CIPHER           = 0x003a,
    host_CMD_802_11_RSN_AUTH_SUITES        = 0x0032,
    host_CMD_802_11_RSN_STATS              = 0x0033,    // removed
    host_CMD_802_11_PWK_KEY                = 0x0034,
    host_CMD_802_11_GRP_KEY                = 0x0035,
    host_CMD_802_11_PAIRWISE_TSC           = 0x0036,
    host_CMD_802_11_GROUP_TSC              = 0x0037,
    host_CMD_802_11_ENABLE_QOS_WME         = 0x0038,
    host_CMD_802_11_WME_AC_PARAMS          = 0x0039,
    host_CMD_SET_GPIO_PIN_STATE            = 0x003b,    // removed
    host_SET_AFC_CMD                       = 0x003c,
    host_GET_AFC_CMD                       = 0x003d,
    host_CMD_802_11_DEEP_SLEEP             = 0x003e,
    host_CMD_802_11_AD_HOC_STOP            = 0x0040,
    host_CMD_802_11_GPIO_CFG               = 0x0041,    // removed
    host_CMD_802_11_GPIO_OP                = 0x0042,    // removed
    host_CMD_802_11_HOST_SLEEP_CFG         = 0x0043,
    host_CMD_802_11_WAKEUP_CONFIRM         = 0x0044,
    host_CMD_802_11_HOSTSLEEP_ACTIVE       = 0x0045,
    host_CMD_802_11_DIS_GM_DATA_TX         = 0x0046,
    host_CMD_802_11_PRE_TBTT               = 0x0047,    // removed
    host_CMD_802_11_PASSTHROUGH            = 0x0048,
    host_CMD_802_11_DIS_BCN_TX             = 0x0049,
    host_CMD_802_11_TX_MODE                = 0x004a,
    host_CMD_802_11_RX_MODE                = 0x004b,
    host_CMD_802_11_RGN_CODE               = 0x004c,
    host_CMD_802_11_MAC_ADDR               = 0x004d,
    host_CMD_802_11_LED_CONTROL            = 0x004e,
    host_CMD_802_11_MULTIPLE_DTIM          = 0x004f,    // removed
    host_CMD_802_11_ASSOCIATE              = 0x0050,
    host_CMD_802_11_BNC_TUNE_OPT           = 0x0051,
    host_CMD_802_11_KEY_DECRYPT            = 0x0053,
    host_CMD_802_11_KEY_ENCRYPT            = 0x0054,
    host_CMD_ATIM_TX                       = 0x0055,
    host_CMD_BCN_CW                        = 0x0056,
    host_CMD_802_11_CAL_DATA               = 0x0057,
    host_CMD_802_11_BAND_CONFIG            = 0x0058,
    host_CMD_EEPROM_ACCESS                 = 0x0059,
    host_CMD_GSPI_BUS_CONFIG               = 0x005a,
    host_CMD_802_11_D_DOMAIN               = 0x005b,
    host_CMD_WMM_ACK_POLICY                = 0x005c,   // removed
    host_CMD_WMM_PRIO_PKT_AVAIL            = 0x005d,
    host_CMD_802_11_KEY_MATERIAL           = 0x005e,
    host_CMD_802_11_H_TPC_INFO             = 0x005f,
    host_CMD_802_11_H_TPC_ADAPT_REQ        = 0x0060,

    host_CMD_802_11_H_CHANNEL_SWITCH       = 0x0061,
    host_CMD_802_11_H_MEASUREMENT_REQ      = 0x0062,
    host_CMD_802_11_H_GET_MEASUREMENT_REPORT  = 0x0063,

    host_CMD_802_11_BCA_CONFIG             = 0x0065,
    host_CMD_802_11_SLEEP_PARAMS           = 0x0066,
    host_CMD_802_11_INACTIVITY_TIMEOUT     = 0x0067,
    host_CMD_802_11_SLEEP_PERIOD           = 0x0068,
    host_CMD_BCA_CONFIG_TIMESHARE          = 0x0069,
    host_CMD_CODE_UPGRADE                  = 0x006a,

    host_CMD_802_11_BG_SCAN_CONFIG         = 0x006b,
    host_CMD_802_11_BG_SCAN_QUERY          = 0x006c,
    host_CMD_802_11_CAL_DATA_EXT           = 0x006d,
    host_CMD_WMM_ADDTS_REQ                 = 0x006e,
    host_CMD_WMM_DELTS_REQ                 = 0x006f,
    host_CMD_WMM_QUEUE_CONFIG              = 0x0070,
    host_CMD_WMM_GET_STATUS                = 0x0071,
    host_CMD_802_11_TPC_CFG                = 0x0072,
    host_CMD_802_11_PA_CFG                 = 0x0073,
    host_CMD_802_11_FW_WAKE_METHOD         = 0x0074,
    host_CMD_802_11_SUBSCRIBE_EVENT        = 0x0075,
    host_CMD_802_11_RATE_ADAPT_RATESET     = 0x0076,
    host_CMD_MRVL_POWER_TBL                = 0x0077,
    host_CMD_802_11_MRVL_CRYPTO            = 0x0078,
    host_CMD_ED_THRESHOLD                  = 0x0079,
    host_CMD_GM_CCA_MODE                   = 0x007a,
    host_CMD_CCM_MODE                      = 0x007b,
    host_CMD_ADHOC_BSSID_FILTER            = 0x007c,
    host_CMD_BCN_LOSS_THRESHOLD            = 0x007d,
    host_CMD_802_11_PA_CFG_EXT             = 0x007e,
    host_CMD_TX_RATE_QUERY                 = 0x007f,
    host_CMD_GET_TSF                       = 0x0080,
    host_CMD_WMM_QUEUE_STATS               = 0x0081,
    host_CMD_AUTO_TX                       = 0x0082,
    host_CMD_802_11_IBSS_COALESCING_STATUS   = 0x0083,
    host_CMD_WSC_ADHOC_REGISTRAR_COMMAND   = 0x0084,
    host_CMD_TCP                           = 0x0085,    
    host_CMD_MEM_ACCESS                    = 0x0086,    
    host_CMD_DFT_ACCESS                    = 0x0087,
    host_CMD_SDIO_INTERRUPT_CONFIG         = 0x0088,
    host_CMD_MFG_COMMAND                   = 0x0089,
    host_CMD_802_11_LOOPBACK               = 0x0090,
    host_CMD_BCN_LOST_TIMEOUT              = 0x0091,
    host_CMD_PMU_CFG                       = 0x0092,
    host_CMD_SDIO_PULL_CTRL                = 0x0093,
    host_CMD_ECL_SYSTEM_CLOCK_CONFIG       = 0x0094,
    host_CMD_FWT_ACCESS                    = 0x0095,
    host_CMD_LDO_CFG                       = 0x0096,
    host_CMD_VERSION_EXT                   = 0x0097,
    host_CMD_802_11_SNIFFER_MODE           = 0x0098,

    /*Reserved for special custom usage, see Bug # 6200 */
    host_CMD_B6200_1             = 0x00F0,    
    host_CMD_B6200_2             = 0x00F1,    
    host_CMD_B6200_3             = 0x00F2,    
    host_CMD_B6200_4             = 0x00F3,    
    host_CMD_B6200_5             = 0x00F4,  

    /*Reserved for SDK support, 0x4000 to 0x40FF, see Bug # 11721 */
    host_CMD_SDK_VER             = 0x4000,    


    /* Reaponse codes are defined below */
    host_RET_CODE_DNLD                      = 0x8001,
    host_RET_OP_PARAM_DNLD                  = 0x8002,
    host_RET_GET_HW_SPEC                    = 0x8003,
    host_RET_EEPROM_UPDATE                  = 0x8004,
    host_RET_802_11_RESET                   = 0x8005,
    host_RET_802_11_SCAN                    = 0x8006,
    host_RET_802_11_QUERY_TRAFFIC           = 0x8009,
    host_RET_802_11_QUERY_STATUS            = 0x800a,
    host_RET_802_11_GET_LOG                 = 0x800b,
    host_RET_MAC_MULTICAST_ADR              = 0x8010,
    host_RET_802_11_AUTHENTICATE            = 0x8011,
    host_RET_802_11_ASSOCIATE               = 0x8012,
    host_RET_802_11_SET_WEP                 = 0x8013,
    host_RET_802_11_GET_STAT                = 0x8014,
    host_RET_802_3_GET_STAT                 = 0x8015,
    host_RET_802_11_SNMP_MIB                = 0x8016,
    host_RET_MAC_REG_MAP                    = 0x8017,
    host_RET_BBP_REG_MAP                    = 0x8018,
    host_RET_RF_REG_MAP                     = 0x8023,
    host_RET_MAC_REG_ACCESS                 = 0x8019,
    host_RET_BBP_REG_ACCESS                 = 0x801a,
    host_RET_RF_REG_ACCESS                  = 0x801b,
    host_RET_802_11_RADIO_CONTROL           = 0x801c,
    host_RET_802_11_RF_CHANNEL              = 0x801d,
    host_RET_802_11_RF_TX_POWER             = 0x801e,
    host_RET_802_11_RSSI                    = 0x801f,
    host_RET_802_11_RF_ANTENNA              = 0x8020,
    host_RET_802_11_PS_MODE                 = 0x8021,
    host_RET_802_11_DATA_RATE               = 0x8022,
    host_RET_802_11_DEAUTHENTICATE          = 0x8024,
    host_RET_802_11_REASSOCIATE             = 0x8025,
    host_RET_802_11_DISASSOCIATE            = 0x8026,
    host_RET_MAC_CONTROL                    = 0x8028,
    host_RET_802_11_QUERY_SCAN_RESULT       = 0x8029,
    host_RET_802_11_GM                      = 0x802a,
    host_RET_802_11_AD_HOC_START            = 0x802b,
    host_RET_802_11_AD_HOC_JOIN             = 0x802c,
    host_RET_802_11_QUERY_RSN_OPTION        = 0x802d,
    host_RET_802_11_QUERY_TKIP_REPLY_CNTRS  = 0x802e,
    host_RET_802_11_ENABLE_RSN              = 0x802f,
    host_RET_802_11_CONFIG_RSN              = 0x8030,
    host_RET_802_11_UNICAST_CIPHER          = 0x8031,
    host_RET_802_11_MCAST_CIPHER            = 0x803a,
    host_RET_802_11_RSN_AUTH_SUITES         = 0x8032,
    host_RET_802_11_RSN_STATS               = 0x8033,
    host_RET_802_11_PWK_KEY                 = 0x8034,
    host_RET_802_11_GRP_KEY                 = 0x8035,
    host_RET_802_11_PAIRWISE_TSC            = 0x8036,
    host_RET_802_11_GROUP_TSC               = 0x8037,
    host_RET_802_11_ENABLE_QOS_WME          = 0x8038,
    host_RET_802_11_WME_AC_PARAMS           = 0x8039,
    host_RET_SET_GPIO_PIN_STATE             = 0x803b,
    host_RET_SET_AFC_CMD                    = 0x803c,
    host_RET_GET_AFC_CMD                    = 0x803d,
    host_RET_802_11_DEEP_SLEEP              = 0x803e,
    host_RET_802_11_AD_HOC_STOP             = 0x8040,
    host_RET_802_11_GPIO_CFG                = 0x8041,
    host_RET_802_11_GPIO_OP                 = 0x8042,
    host_RET_802_11_HOST_SLEEP_CFG          = 0x8043,
    host_RET_802_11_WAKEUP_CONFIRM          = 0x8044,
    host_RET_802_11_HOSTSLEEP_ACTIVE        = 0x8045,
    host_RET_802_11_DIS_GM_DATA_TX          = 0x8046,
    host_RET_802_11_PRE_TBTT                = 0x8047,
    host_RET_802_11_PASSTHROUGH             = 0x8048,
    host_RET_802_11_DIS_BCN_TX              = 0x8049,
    host_RET_802_11_TX_MODE                 = 0x804a,
    host_RET_802_11_RX_MODE                 = 0x804b,
    host_RET_802_11_RGN_CODE                = 0x804c,
    host_RET_802_11_MAC_ADDR                = 0x804d,
    host_RET_802_11_LED_CONTROL             = 0x804e,
    host_RET_802_11_MULTIPLE_DTIM           = 0x804f,     // removed
    host_RET_802_11_ASSOCIATE_EXT           = 0x8050,
    host_RET_802_11_BNC_TUNE_OPT            = 0x8051,
    host_RET_802_11_KEY_DECRYPT             = 0x8053,
    host_RET_802_11_KEY_ENCRYPT             = 0x8054,
    host_RET_ATIM_TX                        = 0x8055,
    host_RET_BCN_CW                         = 0x8056,
    host_RET_802_11_CAL_DATA                = 0x8057,
    host_RET_802_11_BAND_CONFIG             = 0x8058,
    host_RET_EEPROM_ACCESS                  = 0x8059,
    host_RET_GSPI_BUS_CONFIG                = 0x805a,
    host_RET_802_11_D_DOMAIN                = 0x805b,
    host_RET_WMM_ACK_POLICY                 = 0x805c,     // removed
    host_RET_WMM_PRIO_PKT_AVAIL	            = 0x805d,
    host_RET_802_11_KEY_MATERIAL            = 0x805e,
    host_RET_802_11_H_TPC_INFO              = 0x805f,
    host_RET_802_11_H_TPC_ADAPT_REQ         = 0x8060,

    host_RET_802_11_H_CHANNEL_SWITCH        = 0x8061,
    host_RET_802_11_H_MEASUREMENT_REQ       = 0x8062,
    host_RET_802_11_H_GET_MEASUREMENT_REPORT  = 0x8063,

    host_RET_802_11_BCA_CONFIG              = 0x8065,
    host_RET_802_11_SLEEP_PARAMS            = 0x8066,
    host_RET_802_11_INACTIVITY_TIMEOUT      = 0x8067,
    host_RET_802_11_SLEEP_PERIOD            = 0x8068,
    host_RET_BCA_CONFIG_TIMESHARE           = 0x8069,
    host_RET_CODE_UPGRADE                   = 0x806a,
    host_RET_802_11_BG_SCAN_CONFIG          = 0x806b,
    host_RET_802_11_BG_SCAN_QUERY           = 0x806c,
    host_RET_802_11_CAL_DATA_EXT            = 0x806d,
    host_RET_WMM_ADDTS_REQ                  = 0x806e,
    host_RET_WMM_DELTS_REQ                  = 0x806f,
    host_RET_WMM_QUEUE_CONFIG               = 0x8070,
    host_RET_WMM_GET_STATUS                 = 0x8071,
    host_RET_802_11_TPC_CFG                 = 0x8072,
    host_RET_802_11_PA_CFG                  = 0x8073,
    host_RET_802_11_FW_WAKE_METHOD          = 0x8074,
    host_RET_802_11_SUBSCRIBE_EVENT         = 0x8075,
    host_RET_802_11_RATE_ADAPT_RATESET      = 0x8076,
    host_RET_MRVL_POWER_TBL                 = 0x8077,
    host_RET_802_11_MRVL_CRYPTO             = 0x8078,
    host_RET_ED_THRESHOLD                   = 0x8079,
    host_RET_GM_CCA_MODE                    = 0x807a,
    host_RET_CCM_MODE                       = 0x807b,
    host_RET_ADHOC_BSSID_FILTER             = 0x807c,
    host_RET_BCN_LOSS_THRESHOLD             = 0x807d,
    host_RET_802_11_PA_CFG_EXT              = 0x807e,
    host_RET_TX_RATE_QUERY                  = 0x807f,
    host_RET_GET_TSF                        = 0x8080,
    host_RET_WMM_QUEUE_STATS                = 0x8081,
    host_RET_AUTO_TX                        = 0x8082,
    host_RET_802_11_IBSS_COALESCING_STATUS  = 0x8083,
    host_RET_WSC_ADHOC_REGISTRAR_COMMAND    = 0x8084,
    host_RET_CMD_TCP		            = 0x8085,    
    host_RET_MEM_ACCESS                     = 0x8086,
    host_RET_DFT_ACCESS                     = 0x8087,
    host_RET_SDIO_INTERRUPT_CONFIG          = 0x8088,
    host_RET_MFG_COMMAND                    = 0x8089, 
    host_RET_802_11_LOOPBACK                = 0x8090,
    host_RET_BCN_LOST_TIMEOUT               = 0x8091,
    host_RET_PMU_CFG                        = 0x8092,
    host_RET_SDIO_PULL_CTRL                 = 0x8093,
    host_RET_ECL_SYSTEM_CLOCK_CONFIG        = 0x8094,
    host_RET_FWT_ACCESS                     = 0x8095,
    host_RET_LDO_CFG                        = 0x8096,
    host_RET_VERSION_EXT                    = 0x8097,
    host_RET_802_11_SNIFFER_MODE            = 0x8098,

    /*Reserved for special custom usage, see Bug # 6200 */ 
    host_RET_B6200_1              = 0x80F0,    
    host_RET_B6200_2              = 0x80F1,    
    host_RET_B6200_3              = 0x80F2,    
    host_RET_B6200_4              = 0x80F3,    
    host_RET_B6200_5              = 0x80F4,  

    /*Reserved for SDK support, 0xC000 to 0xC0FF, see Bug # 11721 */
    host_RET_SDK_VER             = 0xC000,    

} host_Msg_e;

/*
Define result code for each command
*/
typedef enum
{
    host_RESULT_OK = 0x0000,   /*OK*/
    host_RESULT_ERROR = 0x0001,   /*Genenral error*/
    host_RESULT_NOT_SUPPORT = 0x0002,   /*Command is not valid*/
    host_RESULT_PENDING = 0x0003,   /*Command is pending (will be processed)*/
    host_RESULT_BUSY = 0x0004,   /*System is busy (command ignored)*/
    host_RESULT_PARTIAL_DATA = 0x0005  /*Data buffer is not big enough*/
} host_Result_e;

#ifdef MARVELL_SDK_SUPPORT			// To build SDK use build target "buildsdk"
#define HOST_CMD_CUSTOM  0x4000		// if bit 30 is set, it is a custom command
#endif	//MARVELL_SDK_SUPPORT

/* Macro to make code for command response */
#define CMD_RSP_CODE(cmd) (cmd | 0x8000)

/*
Definition of action or option for each command
*/

/*Define general purpose action */
#define host_ACT_READ     0x0000
#define host_ACT_WRITE    0x0001

#define host_ACT_GET      0x0000
#define host_ACT_SET      0x0001

#define host_ACT_OFF      0x0000
#define host_ACT_ON       0x0001

/*Define action or option for CMD_GET_HW_SPEC*/
/*Define action or option for CMD_EEPROM_UPDATE*/

/*Define action or option for CMD_802_11_AUTHENTICATE*/
#define host_ACT_AUTHENTICATE     0x0001
#define host_ACT_DEAUTHENTICATE   0x0002

/*Define action or option for CMD_802_11_ASSOCIATE*/
#define host_ACT_ASSOCIATE        0x0001
#define host_ACT_DISASSOCIATE     0x0002
#define host_ACT_REASSOCIATE      0x0003

/*Define action or option for CMD_802_11_SET_WEP*/
#define host_ACT_ADD              0x0002
#define host_ACT_REMOVE           0x0004
#define Host_ACT_USE_DEFAULT      0x0008

#define host_TYPE_WEP_40_BIT      0x0001   /*40 bit*/
#define host_TYPE_WEP_104_BIT     0x0002   /*128 bit*/
#define host_TYPE_WEP_TX_KEY      0x0004   /*TX WEP*/

#define host_NUM_OF_WEP_KEYS      4

#define host_WEP_KEY_INDEX_MASK   0x3fffffff

/*Define action or option for CMD_802_11_RESET*/
#define host_ACT_NOT_REVERT_MIB   0x0001
#define host_ACT_REVERT_MIB       0x0002
#define host_ACT_HALT             0x0003

/*Define action or option for CMD_802_11_SCAN*/
#define host_TYPE_BSS             0x0001
#define host_TYPE_IBSS            0x0002

/*Define action or option for CMD_802_11_QUERY_STATUS*/
#define host_STATUS_FW_INIT           0x0000
#define host_STATUS_FW_IDLE           0x0001
#define host_STATUS_FW_WORKING        0x0002
#define host_STATUS_FW_ERROR          0x0003
#define host_STATUS_FW_POWER_SAVE     0x0004

#define host_STATUS_MAC_RX_ON         0x0001
#define host_STATUS_MAC_TX_ON         0x0002
#define host_STATUS_MAC_LOOP_BACK_ON  0x0004
#define host_STATUS_MAC_WEP_ENABLE    0x0008
#define host_STATUS_MAC_INT_ENABLE    0x0010

/*  Define action or option for CMD_MAC_CONTROL */
#define host_ACT_MAC_RX_ON                   0x0001
#define host_ACT_MAC_TX_ON                   0x0002
#define host_ACT_MAC_LOOPBACK_ON             0x0004
#define host_ACT_MAC_WEP_ENABLE              0x0008
#define host_ACT_MAC_INT_ENABLE              0x0010
#define host_ACT_MAC_MULTICAST_ENABLE        0x0020
#define host_ACT_MAC_BROADCAST_ENABLE        0x0040
#define host_ACT_MAC_PROMISCUOUS_ENABLE      0x0080
#define host_ACT_MAC_ALL_MULTICAST_ENABLE    0x0100

/*Define action or option or constant for CMD_MAC_MULTICAST_ADR*/
#define host_SIZE_MAC_ADR        6
#define host_MAX_MCAST_ADRS      32

/*Define action or option for CMD_802_11_SNMP_MIB*/
#define host_TYPE_MIB_FLD_BOOLEAN      0x0001  /* BOOLEAN */
#define host_TYPE_MIB_FLD_INTEGER      0x0002  /* 32 byte unsigned integer */
#define host_TYPE_MIB_FLD_COUNTER      0x0003  /* Counter */
#define host_TYPE_MIB_FLD_OCT_STR      0x0004  /* Octet string */
#define host_TYPE_MIB_FLD_DISPLAY_STR  0x0005  /* String */
#define host_TYPE_MIB_FLD_MAC_ADR      0x0006  /* MAC address */
#define host_TYPE_MIB_FLD_IP_ADR       0x0007  /* IP address */
#define host_TYPE_MIB_FLD_WEP          0x0008  /* WEP */

/*Define action or option for CMD_802_11_RADIO_CONTROL*/
#define host_TYPE_LONG_PREAMBLE    0x0002
#define host_TYPE_SHORT_PREAMBLE   0x0003

/*Define action or option for CMD_802_11_RF_CHANNEL*/
#define host_TYPE_802_11A          0x0001
#define host_TYPE_802_11B          0x0002

/*Define action or option for CMD_802_11_RSSI*/
/*Define action or option for CMD_802_11_RF_TX_POWER*/

/*Define action or option for CMD_802_11_RF_ANTENNA*/
/*Define action or option for CMD_802_11_DATA_RATE*/
#define host_ACT_SET_RX             0x0001
#define host_ACT_SET_TX             0x0002
#define host_ACT_SET_BOTH           0x0003
#define host_ACT_GET_RX             0x0004
#define host_ACT_GET_TX             0x0008
#define host_ACT_GET_BOTH           0x000c

#define host_TYPE_ANTENNA_DIVERSITY   0xffff

/*Define action or option for CMD_802_11_PS_MODE*/
#define host_TYPE_CAM               0x0000
#define host_TYPE_MAX_PSP           0x0001
#define host_TYPE_FAST_PSP          0x0002

/*Define action or option for CMD_802_11_DATA_RATE*/
#define host_ACT_SET_TX_AUTO_RATE   0x0000
#define host_ACT_SET_TX_FIX_RATE    0x0001
#define host_ACT_GET_TX_RATES       0x0002
/*Define action or option for CMD_802_11_DATA_RATE*/
#define host_ACT_SET_TX_RATES_SET   0x0001
#define host_ACT_GET_TX_RATES_SET   0x0000


#define MAX_MULTI_DTIM      5

/*Define command code for CMD_802_11_PS_MODE*/
#define   PS_CMD_ENTER        0x30
#define   PS_CMD_EXIT         0x31
#define   PS_CMD_TX_PENDING   0x32  // removed
#define   PS_CMD_CHANGE_LEVEL 0x33  // removed
#define   PS_CMD_SLEEP_CFM    0x34
#define   PS_CMD_FULL_POWERDOWN  0x35
#define   PS_CMD_FULL_POWERUP    0x36

#endif
