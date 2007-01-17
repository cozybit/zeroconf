/******************** (c) Marvell Semiconductor, Inc., 2006 *******************
 *
 * $Header:$
 *
 * Purpose:
 *    Eping
 *
 * Public Procedures:
 *
 * Private Procedures:
 *
 * Notes:
 *    None.
 *
 *****************************************************************************/

#include "wltypes.h"
#include "dbg_Facility.h"
#include "eping.h"
#include "mli.h"
#include "peersta_api.h"
#ifdef EMBEDDED_TCPIP
#include "tcpip_socki.h"
#endif
/*  Hard coded values*/
extern char ip_addr[];
extern char net_mask[];
extern char def_gtwy[];

char ping_ipaddr[4];
#define RAW_SOCKET_DATA 			0x07

int identifier = 0;
int datasize = 64;
extern UINT8 STAMACAddr[6];
IEEEtypes_MacAddr_t targetMACAddr;
extern IEEEtypes_MacAddr_t specificBSSID;

extern int send_ping;
extern int link_present;
int use_peer_sta_api;
#define PING_TICK_COUNT  2000
int tick_count = 0;
/******************************************************************************
 *
 * Name: computeCksum
 *
 * Description:
 * 	 This routine is used to compute the ICMP checksum
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   buffer and the length over which the checksum has to be computed
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

uint16 computeCksum(void *buf, int len)
{
	unsigned short int word16 = 0, i;
	unsigned int sum = 0;
	unsigned char *buffer = (unsigned char *) buf;

	for (i = 0; i < len; i+=2) {
		word16 = ((buffer[i] << 8) & 0xFF00) + (buffer[i + 1] & 0xFF);
		sum += (unsigned int) word16;
	}

	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	sum = ~sum;

	word16 = (unsigned short) sum;

	(unsigned char)i = *((unsigned char *) & word16 + 1);
	*((unsigned char *) & i + 1) = (unsigned char) word16;

	return i;
}

/******************************************************************************
 *
 * Name: frameICMPPacket
 *
 * Description:
 * 	 This routine is used to send the ping packets to the ETCP stack
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   IP address to be pinged
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/
#ifdef EMBEDDED_TCPIP
void frameICMPPacket(void)
{

	icmp_hdr *icmp;
	TCP_CNFG_SendTo *sendto;
	wcb_t    *buffer;
	UINT8 *x;

  if(send_ping && (tick_count == PING_TICK_COUNT) && link_present) {
	buffer = (wcb_t *)mli_GetTxFreeBuffer();

	if(buffer != 0) {
    DBG_P(( DBG_L0 "eping: request IP address %d.%d.%d.%d\r\n",
			ping_ipaddr[0],ping_ipaddr[1],ping_ipaddr[2],ping_ipaddr[3] ));

    buffer->TxPacketType = RAW_SOCKET_DATA;
    buffer->SocketId = 0;
    buffer->TcpFlags = 0;
    buffer->TxPacketLength = datasize + sizeof(TCP_CNFG_SendTo);

	/* Set Protocol Type to UDP*/
    x = (UINT8 *)buffer + sizeof(wcb_t);
	*x = 0x2;
    x++;

	/* Fill the Socket Configuration Information*/
    sendto = (TCP_CNFG_SendTo *)x;
    sendto->stToAddress.sin_len = sizeof(struct packed_sockaddr_in);;
    sendto->stToAddress.sin_family = AF_INET;
    sendto->stToAddress.sin_port = 0;
    sendto->stToAddress.sin_addr.s_addr = ping_ipaddr[0] + (ping_ipaddr[1]<<8) +
	                                      (ping_ipaddr[2]<<16) + (ping_ipaddr[3]<<24);
    sendto->nBufferLength = datasize;

	/* Fill up ICMP data*/
	x += sizeof(TCP_CNFG_SendTo);
    icmp = (icmp_hdr *)x;
	icmp->type = ECHO_REQ;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->id = ntohs(identifier);
	icmp->sequence = identifier;
	identifier++;

	/* Compute ICMP checksum */
	icmp->checksum = computeCksum((void *)icmp, datasize);

	/* Send the packet to ETCP Stack*/
	mli_SendTxBuffer((uint8*)buffer);
	}
  }
  tick_count++;
  if(tick_count == (PING_TICK_COUNT + 1)) tick_count = 0;
}
#endif
/******************************************************************************
 *
 * Name: eping_process_pkt
 *
 * Description:
 * 	 This routine processes the ping replys received from the WLAN interface
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   packet buffer
 *
 * Return Value:
 *   status
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

void  eping_process_pkt(uint8 * pkt)
{
	unsigned short protocol;
	ip_hdr *iphdr;
	icmp_hdr *icmphdr;
	arp_hdr *arphdr;
#if 0
	char * x;
	int i;
	x = (char *)(pkt);
	DBG_P(( DBG_L0 "packet"));
	for (i = 0 ; i < 40; i++) {
		if((i%8) == 0)
			DBG_P(( DBG_L0 "\r\n"));

		DBG_P(( DBG_L0 "0x%2x ", x[i]));
	}
    DBG_P(("\r\n"));
#endif
	protocol = *(unsigned short *)(pkt + 20);
	if(protocol == 0x08) {
		iphdr   = (ip_hdr *)(pkt + 22);
		if(iphdr->protocol == 0x1){
			icmphdr = (icmp_hdr *)(pkt + 42);	   	     
			if(icmphdr->code == ECHO_REP) {
#ifdef EMBEDDED_TCPIP
	      DBG_P(( DBG_L0 "eping: reply from IP address %d.%d.%d.%d\r\n",
				  iphdr->saddr[0],iphdr->saddr[1],iphdr->saddr[2],iphdr->saddr[3]));
#else
	      DBG_P(( DBG_L0 "Raw Data: Ping Reply from IP address %d.%d.%d.%d\r\n",
				  iphdr->saddr[0],iphdr->saddr[1],iphdr->saddr[2],iphdr->saddr[3]));
#endif
	     }
	  }
	} else if (protocol == 0x608) {
       arphdr = (arp_hdr *)(pkt + 22);
	   memcpy((void *)targetMACAddr, (void *)arphdr->ar_sha,sizeof(IEEEtypes_MacAddr_t)); 
	}
}
/******************************************************************************
 *
 * Name: send_raw_pkt
 *
 * Description:
 * 	 sends a raw 802.11 packet
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None
 *
 * Return Value:
 *   None
 *
 * Notes:
 *   None.
 *
 * PDL:
 *
 * END PDL
 *
 ****************************************************************************/

void send_raw_pkt(void)
{
	wcb_t    *buffer;
	UINT8 *x;
	ip_hdr *iphdr;
	icmp_hdr *icmp;
    arp_hdr *arp;

	if(send_ping && (tick_count == PING_TICK_COUNT) && link_present) {
	
	if(use_peer_sta_api) 
		buffer = (wcb_t *)peersta_GetTxFreeBuffer(specificBSSID);
    else
		buffer = (wcb_t *)mli_GetTxFreeBuffer();

	if(buffer != 0) {
	 memset((void*)buffer, 0x00, sizeof(wcb_t));

     buffer->TxPacketType = 0x8;
     buffer->TxPacketOffset = sizeof(wcb_t);

     x = (uint8 *)buffer + sizeof(wcb_t);

     if (identifier%10) {
	  memcpy((void *)(&buffer->TxDestAddrHigh),(void *)targetMACAddr, sizeof(IEEEtypes_MacAddr_t));
	  memcpy(x, (void *)targetMACAddr, sizeof(IEEEtypes_MacAddr_t));
	  x = x + 6;
	  memcpy(x, STAMACAddr, sizeof(IEEEtypes_MacAddr_t));
	  x = x + 6;

	  DBG_P(( DBG_L0 "Raw Data: Ping Request to IP Address %d.%d.%d.%d\r\n",
			  ping_ipaddr[0],ping_ipaddr[1],ping_ipaddr[2],ping_ipaddr[3]));
	  buffer->TxPacketLength = 98;
	  *(uint16 *)x = 0x8;
	  x = x + 2;
	  iphdr = (ip_hdr *)x;
	  iphdr->ihl = 0x5;
	  iphdr->version = 0x4;
	  iphdr->tos = 0x0;
	  iphdr->total_len = 0x5400;
	  iphdr->id = 1;
	  iphdr->frag_off = 0x40;
	  iphdr->ttl = 64;
	  iphdr->protocol = 1;
	  iphdr->check = 0;

	  memcpy((void *)iphdr->saddr, ip_addr, 4);
      memcpy((void *)iphdr->daddr, ping_ipaddr, 4);

     /* Compute IP checksum */
      iphdr->check = computeCksum((void *)iphdr, 20);

	 /* Fill up ICMP data*/
	  x += 20;
      icmp = (icmp_hdr *)x;
	  icmp->type = ECHO_REQ;
	  icmp->code = 0;
	  icmp->checksum = 0;
	  icmp->id = identifier;
	  icmp->sequence = identifier;

	 /* Compute ICMP checksum */
	  icmp->checksum = computeCksum((void *)icmp, 64);
    } else {
	  memset((void *)(&buffer->TxDestAddrHigh),0xFF, sizeof(IEEEtypes_MacAddr_t));
	  memset(x, 0xFF, sizeof(IEEEtypes_MacAddr_t));
	  x = x + 6;
	  memcpy(x, STAMACAddr, 6);
	  x = x + 6;
	  buffer->TxPacketLength = 64;
	  *(uint16 *)x = 0x0608;
	  x = x + 2;
	  /* Fill up the ARP header*/
	  arp = (arp_hdr *)x;
      arp->ar_hrd  = 0x100;
	  arp->ar_pro  = 0x0008;
	  arp->ar_hln  = 6;
	  arp->ar_pln  = 4;
	  arp->ar_op   = 0x0100;
	  memcpy((void *)arp->ar_sha, (void *)STAMACAddr, sizeof(IEEEtypes_MacAddr_t));
	  memcpy((void *)arp->ar_spa, (void *)ip_addr, 4);
	  memset((void *)arp->ar_tha, 0, sizeof(IEEEtypes_MacAddr_t));
	  memcpy((void *)arp->ar_tpa, (void *)ping_ipaddr, 4);
    }

    identifier++;

   /* Send the packet to Firmware*/
	if(use_peer_sta_api) 
		peersta_SendTxBuffer((uint8 *)buffer);
	else
		mli_SendTxBuffer((uint8*)buffer);
  }
 }
  tick_count++;
  if(tick_count == (PING_TICK_COUNT + 1)) tick_count = 0;

}

