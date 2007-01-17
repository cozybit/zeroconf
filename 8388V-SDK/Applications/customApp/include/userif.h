 /*******************************************************************
 *
 *    DESCRIPTION: user interface header
 *
 *    AUTHOR:
 *
 *    HISTORY:
 *
 *    DATE:10/9/2006
 *
 *******************************************************************/

#ifndef USERIF_H
#define USERIF_H 
 /** include files **/
 
 /** local definitions **/
 
 /* default settings */
 
 /** external functions **/
 
 /** external data **/
 
 /** internal functions **/
 
 /** public data **/
 
 /** private data **/
 
 /** public functions **/
/**
 * Prepares the scan command
 */
void userif_prepare_scan_cmd(int);

/**
 * Prepares the authentication command
 */
void userif_prepare_auth_cmd(void);

/**
 * Prepares the deauthentication command
 */
void userif_prepare_deauth_cmd(void);
/**
 * Prepares the adhoc stop command
 */
void userif_prepare_adhoc_stop_cmd(void);

/**
 * Prepares the association command
 */
void userif_prepare_assoc_cmd(void);

/**
 * Prepares the adhoc join command 
 */
void userif_prepare_adhoc_join_cmd(void);

/**
 * Prepares the adhoc start command
 */
void userif_prepare_adhoc_start_cmd(void);

#ifdef EMBEDDED_TCPIP
/**
 * Prepares the etcp config command
 */
void userif_prepare_config_etcp(void);

/**
 * Prepares the ICMP open socket command
 */
void userif_prepare_open_raw_socket(void);
#endif

/**
 * Prepares the command to get the mac address from
 * firmware
 */
void userif_prepare_macaddr_get_cmd(void);

/**
 * Deals with the timeout event for reassociation
 * 
 * @param data
 * @return 
 */
SINT32 userif_ReAssocActTimeOut(UINT8 *data);

/**
 * Finds a  bssid in the scan list
 * 
 * @return 
 */
int	FindBSSIDinList(void);

/**
 * Finds an SSID in the scan list
 * 
 * @return 
 */
int	FindSSIDinList(void);

 /** private functions **/

#endif
