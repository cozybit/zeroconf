/*******************************************************************
*
*    DESCRIPTION: Command parser
*
*    AUTHOR:
*
*    HISTORY:
*
*    DATE:9/28/2006
*
*******************************************************************/

/** include files **/
#include "wltypes.h"
#include "dbg_Facility.h"
#include <string.h>
#include "IEEE_types.h"
#include "userif.h"
extern IEEEtypes_Bss_e  bss_type;
extern IEEEtypes_Bss_e  currbss_type;
extern int send_ping;
extern char ip_addr[4] ;  
extern char net_mask[4] ; 
extern char def_gtwy[4] ; 
extern int	FindSSIDinList(void);
extern int  FindBSSIDinList(void);
extern IEEEtypes_SsIdElement_t specificSSID;
extern char ping_ipaddr[4];
extern int link_present;
extern IEEEtypes_MacAddr_t specificBSSID;
extern int cmd_in_progress;

int string_pos = 0;
#define STRING_SIZE  100
#ifdef UART_DRV
#define CMD_END_CHAR     '\r'
#else
#define CMD_END_CHAR     ';'
#endif
char user_string[STRING_SIZE];

/** local definitions **/

/* default settings */

/** external functions **/

/** external data **/

/** internal functions **/

/**
 * Print the commands supported
 */
void print_usage(void)
{
	DBG_P(( DBG_L0 "Command List:\r\n")); 
	DBG_P(( DBG_L0 "	iwlist scan\r\n")); 
	DBG_P(( DBG_L0 "	iwconfig essid <essid>\r\n")); 
	DBG_P(( DBG_L0 "	iwconfig ap <bssid>\r\n")); 
	DBG_P(( DBG_L0 "	iwconfig mode ad-hoc/managed\r\n")); 
	DBG_P(( DBG_L0 "	econfig <clientip> <netmask> <gateway>\r\n")); 
	DBG_P(( DBG_L0 "	ping <ip address>\r\n")); 
	DBG_P(( DBG_L0 "	ping stop\r\n")); 
#ifdef UART_DRV
	DBG_P(( DBG_L0 ">")); 
#endif

}

/**
 * Parse MAC address from the string
 * 
 * @param s
 * @param mac_addr
 */
void get_macaddr(char *s, char * mac_addr)
{
	int i = 0;
	int j = 0;
	char val;
	
	for (i = 0; i<6; i++) {
		val = 0;
        while((s[j] != ':') && (s[j] != '\0') && (s[j] != ' ')) {
			if(s[j] >= 'a')
			 val = val*16 + (s[j] - 'a' + 10);
			else if (s[j] >= 'A')
			 val = val*16 + (s[j] - 'A' + 10);
			else
			 val = val*16 + (s[j] - '0');
			j++;
        }
		j++;
        mac_addr[i] = val;
	}
}

/**
 * Parse IP address from string
 * 
 * @param s
 * @param ip_addr
 */
void get_ipaddr(char *s, char * ip_addr)
{
	int i = 0;
	int j = 0;
	char val;
	
	for (i = 0; i < 4 ; i++) {
		val = 0;
        while((s[j] != '.') && (s[j] != '\0') && (s[j] != ' ')) {
			val = val*10 + (s[j] - '0');
			j++;
        }
		j++;
        ip_addr[i] = val;
	}
}

/**
 * Skip to next word in the string
 * 
 * @param s
 * @return 
 */
int get_next_word(char *s)
{
	int i;
	i = 0;
	while(s[i] != ' ') {
		i++;
	}
	while(s[i] == ' ') {
		i++;
	}
    return i;
}

/**
 * Asynchronously reads a line ending with ';'
 * 
 * @return 
 */
int cmd_parser_read_line(void)
{
	dbg_FlushingOut();

     while(dbg_GetChar((uint8 *)&user_string[string_pos]) == 1)
	 {
		 if (user_string[string_pos] == CMD_END_CHAR) {
			 user_string[string_pos] = '\0'; 
             string_pos = 0;
			 return 1;		 
		 }
#ifdef UART_DRV
	DBG_P(( DBG_L0 "%c",user_string[string_pos])); 
#endif
		 string_pos++;
	 }
	 return 0;
}

/**
 * Parses commands and executes them
 */
void cmd_parser(void)
{
   char * curr_pos = &user_string[0];
   dbg_FlushingOut();
   if(!cmd_in_progress && cmd_parser_read_line()) {   
     if(!memcmp(user_string,"iwlist",6)){
		userif_prepare_scan_cmd(0);
	 }     
     else if(!memcmp(user_string,"iwconf",6)){
        curr_pos = &user_string[get_next_word(user_string)];
		if(!memcmp(curr_pos,"essid",5)) {
			curr_pos = &curr_pos[get_next_word(curr_pos)];
			specificSSID.Len  = strlen(curr_pos);
			memcpy((void *)specificSSID.SsId, curr_pos, strlen(curr_pos));
			if(link_present) {
				if(currbss_type != BSS_INDEPENDENT)
					userif_prepare_deauth_cmd();
				else
                    userif_prepare_adhoc_stop_cmd();
			} else {
				    userif_prepare_scan_cmd(1);
			}
			link_present = 0;				
		} else if (!memcmp(curr_pos,"mode",4)) {
			curr_pos = &curr_pos[get_next_word(curr_pos)];
            if(!memcmp(curr_pos,"ad-hoc",6)) {
				bss_type = BSS_INDEPENDENT;
			} else if(!memcmp(curr_pos,"manage",6)) {
				bss_type = BSS_INFRASTRUCTURE;
			} else {
				bss_type = BSS_ANY;
			}
		} else if (!memcmp(curr_pos,"ap",2)) {
			curr_pos = &curr_pos[get_next_word(curr_pos)];
            get_macaddr(curr_pos,(char *)specificBSSID);
			if (FindBSSIDinList()) {
				userif_prepare_auth_cmd();			
			} else {
				if(link_present) {
					link_present = 0;
					userif_prepare_deauth_cmd();
				} else {
				    userif_prepare_scan_cmd(2);
				}
			}
		}
     }
     else if(!memcmp(user_string,"econfi",6)){
		curr_pos = &user_string[get_next_word(user_string)];
		get_ipaddr(curr_pos, ip_addr);
		curr_pos = &curr_pos[get_next_word(curr_pos)];
		get_ipaddr(curr_pos, net_mask);	    
		curr_pos = &curr_pos[get_next_word(curr_pos)];
		get_ipaddr(curr_pos, def_gtwy);	    		
#ifdef EMBEDDED_TCPIP		
		userif_prepare_config_etcp();
#endif     
	 }
     else if(!memcmp(user_string,"ping",4)){
		curr_pos = &user_string[get_next_word(user_string)];
        if(!memcmp(curr_pos,"stop",4)) {
		  send_ping = 0;
		} else {
		  get_ipaddr(curr_pos, ping_ipaddr);
#ifdef EMBEDDED_TCPIP
		  userif_prepare_open_raw_socket();
#else
          userif_prepare_macaddr_get_cmd();
#endif
          send_ping = 1;
		}
     }
	 else {
		 print_usage();
	 }
#ifdef UART_DRV
	 DBG_P(( DBG_L0 "\r\n>")); 
#endif

   }
}
