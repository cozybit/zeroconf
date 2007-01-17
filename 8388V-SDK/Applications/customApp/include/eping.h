/******************** (c) Marvell Semiconductor, Inc., 2006 *******************
 *
 * $Header:$
 *
 * Purpose:
 *    Eping Header File
 *
 * Public Procedures:
 *
 * Private Procedures:
 *
 * Notes:
 *    None.
 *
 *****************************************************************************/
#ifndef EPING_H
#define EPING_H

#include "wltypes.h"
#include "IEEE_types.h"

#define ECHO_REQ  8
#define ECHO_REP  0

typedef PACK_START struct arp_hdr {
     unsigned short ar_hrd;	/* format of hardware address */
     unsigned short ar_pro;	/* format of protocol address */
     unsigned char	ar_hln;		/* length of hardware address */
     unsigned char	ar_pln;		/* length of protocol address */
     unsigned short ar_op;		/* one of: */
     unsigned char	ar_sha[6];	/* sender hardware address */
     unsigned char	ar_spa[4];	/* sender protocol address */
     unsigned char	ar_tha[6];	/* target hardware address */
     unsigned char	ar_tpa[4];	/* target protocol address */
} PACK_END arp_hdr;

typedef PACK_START struct ip_hdr {
	unsigned char	ihl:4,
			        version:4;
	unsigned char	tos;
	unsigned short  total_len;
	unsigned short  id;
	unsigned short  frag_off;
	unsigned char	ttl;
	unsigned char	protocol;
	unsigned short  check;
	unsigned char	saddr[4];
	unsigned char	daddr[4];
} PACK_END ip_hdr;

typedef PACK_START struct icmp_hdr {
	unsigned char	type;
	unsigned char	code;
	unsigned short  checksum;
    unsigned short  id;
	unsigned short  sequence;
} PACK_END icmp_hdr;

void eping_process_pkt(uint8* pkt);
void frameICMPPacket(void);
void send_raw_pkt(void);
#endif
