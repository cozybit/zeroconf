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
#include "trsocket.h"
#include "tcpip_socki.h"
#include "linklocal.h"
#include "mdns_responder.h"
#include "log.h"

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

/* Borrowed from treck congifuration handler */
extern treck_conf_t *stTreckConf;

extern int GetMACAddr(void *, char *);

int tcp_ready = 0;

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
	DBG_P(( DBG_L0 "	help\r\n")); 
	DBG_P(( DBG_L0 "	iwlist scan\r\n")); 
	DBG_P(( DBG_L0 "	iwconfig essid <essid>\r\n")); 
	DBG_P(( DBG_L0 "	iwconfig ap <bssid>\r\n")); 
	DBG_P(( DBG_L0 "	iwconfig mode ad-hoc/managed\r\n")); 
	DBG_P(( DBG_L0 "	econfig <clientip> <netmask> <gateway>\r\n")); 
	DBG_P(( DBG_L0 "	printip -- prints ip, netmask, and gateway\r\n"));
	DBG_P(( DBG_L0 "	printmac -- prints mac address\r\n"));
	DBG_P(( DBG_L0 "	ping <ip address>\r\n")); 
	DBG_P(( DBG_L0 "	ping stop\r\n")); 
	DBG_P(( DBG_L0 "	linklocal start\r\n")); 
	DBG_P(( DBG_L0 "	linklocal stop\r\n")); 
	DBG_P(( DBG_L0 "	mdns start\r\n")); 
	DBG_P(( DBG_L0 "	mdns stop\r\n")); 
	DBG_P(( DBG_L0 "	log [init|shutdown|dump|purge|write]\r\n")); 
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

/* This function is called every tick.  It pushes any output to the serial port,
 * and reads any input to the user_string. 
 */
#define SERIAL_BUFFER_SIZE 256 /* Must be power of 2 */
static char from_serial[SERIAL_BUFFER_SIZE];
static int read_ptr = 0;
static int write_ptr = 0;

void cmd_handle_serial(void)
{
	unsigned char c;
	int tmp_ptr;

	dbg_FlushingOut();

	while(1) {
		if(dbg_GetChar(&c) == 1) {
			tmp_ptr = write_ptr + 1;
			tmp_ptr &= SERIAL_BUFFER_SIZE - 1;
			if(tmp_ptr == read_ptr) {
				/* buffer is full.  Drop character. */
				return;
			} else {
				from_serial[write_ptr] = c;
				write_ptr = tmp_ptr;
			}
		} else {
			return;
		}
	}	
}

/**
 * Asynchronously reads a line ending with ';'
 * 
 * @return 
 */
int cmd_parser_read_line(void)
{
	int index = 0;

	while(1) {

		if(read_ptr == write_ptr) {
			/* buffer is empty.  Block. */
			os_TaskDelay(2);
			continue;
		}

		if(index == STRING_SIZE-1) {
			/* no more room in buffer.  Just return what we can. */
			user_string[index] = '\0';
			return 1;
		}

		if( (from_serial[read_ptr] == '\b') && 
			(index > 0) ) {
			/* handle backspace */
			index--;
			read_ptr++;	
			read_ptr &= SERIAL_BUFFER_SIZE - 1;
			continue;
		}

		if(from_serial[read_ptr] == '\r') {
			/* We found the end of a line.  Return. */
			user_string[index] = '\0';
			read_ptr++;
			read_ptr &= SERIAL_BUFFER_SIZE - 1;
			return 1;
		}

		user_string[index++] = from_serial[read_ptr++];
		read_ptr &= SERIAL_BUFFER_SIZE - 1;
	}

	return 0;
}

/**
 * Parses commands and executes them
 */

void cmd_parser(unsigned long data)
{
	char *curr_pos = &user_string[0];
	int ret;

	while(1) {

		while(cmd_in_progress)
			os_TaskDelay(10);
		DBG_P(( DBG_L0 "\r\n> "));
		cmd_parser_read_line();
	   
		if(strlen(user_string) == 0) {
			/* Do Nothing. */
		}
	   
		else if(!memcmp(user_string,"iwlist",6)){
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
			unsigned int ip;
			unsigned int nm;
			unsigned int gw;
			curr_pos = &user_string[get_next_word(user_string)];
			get_ipaddr(curr_pos, (char *)&ip);
			curr_pos = &curr_pos[get_next_word(curr_pos)];
			get_ipaddr(curr_pos, (char *)&nm);
			curr_pos = &curr_pos[get_next_word(curr_pos)];
			get_ipaddr(curr_pos, (char *)&gw);
			sys_tcpip_init(ip, nm);
		}
	   
		else if(!memcmp(user_string, "printip", 7)){
			unsigned char ip[4] = {0, 0, 0, 0};
			unsigned char nm[4] = {0, 0, 0, 0};
			unsigned char gw[4] = {0, 0, 0, 0};
		   
			if(treck_init != 0) {
				memcpy((void *)ip, ip_addr, 4);
				memcpy((void *)nm, net_mask, 4);
				memcpy((void *)gw, def_gtwy, 4);
			}
		   
			DBG_P(( DBG_L0 "%d.%d.%d.%d,", ip[0], ip[1], ip[2], ip[3]));
			DBG_P(( DBG_L0 "%d.%d.%d.%d,", nm[0], nm[1], nm[2], nm[3]));
			DBG_P(( DBG_L0 "%d.%d.%d.%d\r\n", gw[0], gw[1], gw[2], gw[3]));
		}
	   
		else if(!memcmp(user_string, "printmac", 8)){
			char mac[6];
			GetMACAddr(NULL, mac);
			DBG_P(( DBG_L0 "%02x:%02x:%02x:%02x:%02x:%02x\r\n", \
					mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]));
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

		/* Link-local address manager. */
		else if(!memcmp(user_string, "linklocal", 9)) {
			curr_pos = &user_string[get_next_word(user_string)];
			if(!memcmp(curr_pos, "start", 5)) {
				/* Launch the link local manager */
				ret = ll_init();
				if(ret)
					DBG_P(( DBG_L0 "Error launching link local: %d.\r\n", ret));
			} else if(!memcmp(curr_pos, "stop", 4)) {
				/* Kill the link local manager. */
				ret = ll_shutdown();
				if(ret)
					DBG_P(( DBG_L0 "Error killing link local: %d.\r\n", ret));
			}
		}

		/* mDNS responder */
		else if(!memcmp(user_string, "mdns", 4)) {
			curr_pos = &user_string[get_next_word(user_string)];
			if(!memcmp(curr_pos, "start", 5)) {
				/* launch the mDNS responder */
				ret = mdns_responder_init();
				if(ret)
					DBG_P(( DBG_L0 "Error launching mDNS responder: %d.\r\n",ret));
			}
			else if(!memcmp(curr_pos, "stop", 4)) {
				/* stop the mDNS responder */
				ret = mdns_responder_shutdown();
				if(ret)
					DBG_P(( DBG_L0 "Error stopping mDNS responder: %d.\r\n", ret));
			}
		}

		else if(!memcmp(user_string, "mcast", 5 )){
			curr_pos = &user_string[get_next_word(user_string)];
			if(!memcmp(curr_pos, "get", 3 )) {
				userif_prepare_mcast_cmd();
			}
			else if(!memcmp(curr_pos, "set", 3 )) {
				userif_prepare_mcast_add_cmd();
			}
		}

		else if(!memcmp(user_string, "help", 4)) {
			print_usage();
		}
	   
		/* log interface */
		else if(!memcmp(user_string, "log", 3)) {
			curr_pos = &user_string[get_next_word(user_string)];
			if(!memcmp(curr_pos, "init", 5)) {
				ret = log_init();
				if(ret)
					DBG_P(( DBG_L0 "Error launching logger: %d.\r\n",ret));
			}
			else if(!memcmp(curr_pos, "shutdown", 8)) {
				ret = log_shutdown();
				if(ret)
					DBG_P(( DBG_L0 "Error stopping logger: %d.\r\n", ret));
			}
			else if(!memcmp(curr_pos, "dump", 4)) {
				ret = log_dump();
				if(ret)
					DBG_P(( DBG_L0 "Error dumping log: %d.\r\n", ret));
			}
			else if(!memcmp(curr_pos, "purge", 5)) {
				ret = log_purge();
				if(ret)
					DBG_P(( DBG_L0 "Error purging log: %d.\r\n", ret));
			}
			else if(!memcmp(curr_pos, "write", 5)) {
				curr_pos = &curr_pos[get_next_word(curr_pos)];
				ret = log(curr_pos);
				if(ret)
					DBG_P(( DBG_L0 "Failed to write log: %d.\r\n", ret));
			}
			else {
				DBG_P(( DBG_L0 "No such log command: %s.\r\n", curr_pos));
			}
		}
	   
		else if(!memcmp(user_string, "help", 4)) {
			print_usage();
		}
	   
		else {
#ifdef UART_DRV
			DBG_P(( DBG_L0 "Unknown command.\r\n")); 
#endif		 
		}	   
	}
}

TX_THREAD cmd_thread;
static unsigned int cmd_stack[2048];

void cmd_init(void)
{
	/* Turn on logging by default */
	log_init();
	tx_thread_create(&cmd_thread, "cmd_thread", cmd_parser, 0,
					 (void *)&cmd_stack[0], sizeof(cmd_stack), 21, 21, 2, 
					 TX_AUTO_START);
}
