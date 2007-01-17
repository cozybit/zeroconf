/**
 *  @file   tcpip_socki.h
 *
 *  @brief  This file contains the function prototypes and definitions for the
 *          tcpip_socki.c
 *
 *  Copyright © Marvell International Ltd. and/or its affiliates, 2003-2006
 */

#ifndef _SOCHI_H
#define _SOCHI_H

//=============================================================================
//                               INCLUDE FILES
//=============================================================================

#include "trsocket.h"
#include "wltypes.h"


//=============================================================================
//                            PUBLIC DEFINITIONS
//=============================================================================

#ifndef RX_BUF_SIZE1
#define RX_BUF_SIZE1    2500
#endif

#ifndef RX_LG_BUF_SIZE1
#define RX_LG_BUF_SIZE1  5700
#endif

#ifndef TX_CMD_BUF_SIZE
#define TX_CMD_BUF_SIZE     1500
#endif

#ifndef MAX_FQDN_LEN
#define MAX_FQDN_LEN                    (256 )
#endif

#ifndef MAX_USER_LEN
#define MAX_USER_LEN                    (256 )
#endif

#ifndef MAX_ENCODING_NAME_LEN
#define MAX_ENCODING_NAME_LEN           (32  )
#endif

#ifndef MAX_EVT_BODY
#define MAX_EVT_BODY                    (256 )
#endif

#ifndef TC_BUF_SIZE
#define TC_BUF_SIZE                     (512 )
#endif

/* Maximum length of buffer which we can  give it Treck stack*/
//#define MAX_SEND_PAYLOAD				(1700)

/* Maximum lenght of buffer which we can receive from Treck stack*/
#ifdef TX_LARGE
#define MAX_RECV_PAYLOAD				(RX_LG_BUF_SIZE1-80)
#else
#define MAX_RECV_PAYLOAD				(RX_BUF_SIZE1-80)
#endif
#define MAX_OPTION_VALUE_LENGTH			(16  )

/* size of the socket table*/
#define P_MAX_SOCKET_NUM                (16  )
#define ETCP_MAX_SOCKET_NUM             ( 8  )
#define ETCP_MAX_WINDOW_SIZE            (12288)

/* Maximum events we can save in the event queue*/
#define MAX_TCPIP_EVENT_EQUEUE          8

/* Maximum retries on Treck calls */
#define MAX_TRECK_RETRIES			    10

/* These Macros are used to set bits in the socket table*/
#define P_LOOPBACK_PORT                 (0x1111)
#define P_RB                            (0x8000)
#define P_WB                            (0x4000)
#define P_LB                            (0x2000)
#define P_TB                            (0x1000)
#define P_IS                            (0xFFFF)

/* Macros used to check particular bit in the socket table*/
#define IS_RB_SET(i)                    (gSockTable[(i)]&(P_RB))
#define IS_WB_SET(i)                    (gSockTable[(i)]&(P_WB))
#define IS_LB_SET(i)                    (gSockTable[(i)]&(P_LB))
#define IS_TB_SET(i)                    (gSockTable[(i)]&(P_TB))
#define IS_INVSOCK(i)                   (gSockTable[(i)]==(P_IS))
#define CK_TABLE_ENTRY(i,x)             ((gSockTable[(i)]&0xFFF)==(x))
#define GT_TABLE_ENTRY(i)               ((gSockTable[(i)]&0xFFF))

/* Macros used to set particular bit in the socket table*/
#define SET_RB(i)                       (gSockTable[i]|=P_RB)
#define SET_WB(i)                       (gSockTable[i]|=P_WB)
#define SET_LB(i)                       (gSockTable[i]|=P_LB)
#define SET_TB(i)                       (gSockTable[i]|=P_TB)
#define CLR_RB(i)                       (gSockTable[i]&=~P_RB)
#define CLR_WB(i)                       (gSockTable[i]&=~P_WB)
#define CLR_LB(i)                       (gSockTable[i]&=~P_LB)
#define CLR_TB(i)                       (gSockTable[i]&=~P_TB)
#define SET_INVSOCK(i)                  (gSockTable[i]|=P_IS)
#define SET_SOCKNUM(i,x)                (gSockTable[i]=((x)&0x0FFF))

/* Treck stack status macro*/
#define DEV_STAT_TCP_INITIALIZED        0x01

/* Length of socket event structure header*/
#define TRECK_SOCKETCMD_SIZE            4


/* call back function events.For these events
 * Treck stack calls call back function.
 */
#define SOCKET_CALLBACK_EVENTS          TM_CB_CONNECT_COMPLT        | \
                                        TM_CB_ACCEPT                | \
                                        TM_CB_CLOSE_COMPLT          | \
                                        TM_CB_RECV                  | \
                                        TM_CB_REMOTE_CLOSE          | \
                                        TM_CB_RESET


typedef unsigned long               ULONG;

/*socket address structure*/
PACK_START struct packed_in_addr
{
    unsigned long   s_addr;             /* 32bit netid/hostid address in network byte order */

}PACK_END;

PACK_START struct packed_sockaddr_in
{
    unsigned char       sin_len;                    /* length of struct sockaddr_in                 */
    unsigned char       sin_family;                 /* AF_INET                                      */
    unsigned short      sin_port;                   /* 16bit Port Number in network byte order      */
    struct              packed_in_addr sin_addr;    /* 32bit netid/hostid in network byte order     */
    char                sin_zero[8];                /* unused                                       */
} PACK_END;

/* Treck stack command numbers*/
typedef enum
{
	E_TCP_CMD_Configure		= 0x01,
	E_TCP_CMD_Socket		= 0x02,
	E_TCP_CMD_Listen		= 0x03,
	E_TCP_CMD_Bind			= 0x04,
	E_TCP_CMD_Accept		= 0x05,
	E_TCP_CMD_SetSockOpt	= 0x06,
	E_TCP_CMD_GetSockOpt	= 0x07,
	E_TCP_CMD_GetPeerName	= 0x08,
	E_TCP_CMD_GetSockName	= 0x09,
	E_TCP_CMD_Connect		= 0x0A,
	E_TCP_CMD_Shutdown		= 0x0B,
	E_TCP_CMD_Close			= 0x0C,
	E_TCP_CMD_TcpStatus		= 0x0D,
	E_TCP_CMD_DhcpStatus	= 0x0E,
	E_TCP_CMD_SocketStatus	= 0x0F,
	E_TCP_CMD_TreckStat		= 0x10,
	E_TCP_CMD_GetRsrvPort	= 0x11,
}TCP_CMD_SUB_TYPE;

/* Structure definition of Treck stack configuration command*/
typedef PACK_START struct TCP_CNFG_Configure
{
	UINT8		tTreckConfig;
	UINT8		uUseDHCP;
	UINT8		aIPaddr [4];
	UINT8	 	aNetMask [4];
	UINT8		aBroadcast [4];
	UINT8	 	aDefaultGateway [4];
	UINT8	 	aDnsIP [4];
    UINT8       uMultiHomeIndex;
} PACK_END TCP_CNFG_Configure;

/* Structure definition of Treck stack socket command*/
typedef PACK_START struct TCP_CNFG_Socket
{
    SINT32      nAddressFamily;
    SINT32      nSocketType;
    SINT32      nProtocol;
    SINT32      nSocketDescriptor;
    SINT32      nErrorCode;
} PACK_END TCP_CNFG_Socket;
/*Structure definition of Treck stack Listen command*/
typedef PACK_START struct TCP_CNFG_Listen
{
    SINT32      nSocketDescriptor;
    SINT32      nBackLog;
    SINT32      nErrorCode;
} PACK_END TCP_CNFG_Listen;
/*Structure definition of Treck stack bind command*/
typedef PACK_START struct TCP_CNFG_Bind
{
    SINT32      nSocketDescriptor;
    struct      packed_sockaddr_in stLocalAddress;
    SINT32      nErrorCode;
} PACK_END TCP_CNFG_Bind;

/*Structure definition of Treck stack accept command*/
typedef PACK_START struct TCP_CNFG_Accept
{
    SINT32		nSocketDescriptor;
    struct		packed_sockaddr_in stPeerAddress;
    SINT32		nNewSockFd;
    SINT32		nErrorCode;

} PACK_END TCP_CNFG_Accept;

/*Structure definition of Treck stack setsockopt command*/
typedef PACK_START struct TCP_CNFG_SetSockOpt
{
    SINT32      nSocketDescriptor;
    SINT32      nLevelOfLayer;
    SINT32      nOptionName;
    SINT32      nOptionLength;
    SINT32		nErrorCode;
    SINT8		aOptionValue[MAX_OPTION_VALUE_LENGTH];
} PACK_END TCP_CNFG_SetSockOpt;

/*Structure definition of Treck stack getsockopt command*/
typedef PACK_START struct TCP_CNFG_GetSockOpt
{
    SINT32      nSocketDescriptor;
    SINT32      nLevelOfLayer;
    SINT32      nOptionName;
    SINT32		nOptionLength;
    SINT32		nErrorCode;
    SINT8		aOptionValue [MAX_OPTION_VALUE_LENGTH];
} PACK_END TCP_CNFG_GetSockOpt;

/*Structure definition of Treck stack getpeername command*/
typedef PACK_START struct TCP_CNFG_GetPeerName
{
    SINT32      nSocketDescriptor;
    struct 		packed_sockaddr_in stFromAddress;
    SINT32		nErrorCode;
} PACK_END TCP_CNFG_GetPeerName;

/*Structure definition of Treck stack getsockname command*/
typedef PACK_START struct TCP_CNFG_GetSockName
{
    SINT32      nSocketDescriptor;
    struct		packed_sockaddr_in stMyAddress;
    SINT32		nErrorCode;
} PACK_END TCP_CNFG_GetSockName;

/*Structure definition of Treck stack connect command*/
typedef PACK_START struct TCP_CNFG_Connect
{
    SINT32      nSocketDescriptor;
    struct      packed_sockaddr_in stAddress;
    SINT32		nErrorCode;
} PACK_END TCP_CNFG_Connect;

/*Structure definition of Treck stack shutdown command*/
typedef PACK_START struct TCP_CNFG_ShutDown
{
    SINT32		nSocketDescriptor;
    SINT32		nHowToShutdown;
    SINT32		nErrorCode;
} PACK_END TCP_CNFG_ShutDown;

/*Structure definition of Treck stack close command*/
typedef PACK_START struct TCP_CNFG_Close
{
    SINT32		nSocketDescriptor;
    SINT32		nErrorCode;
} PACK_END TCP_CNFG_Close;

/*Structure definition of TCP status command*/
typedef PACK_START struct TCP_CNFG_TcpStatus
{
	UINT8       tTreckConfig;
	UINT8	 	aIPaddr [4];
	UINT8	 	aNetMask [4];
	UINT8	 	aDefaultGateway [4];
} PACK_END TCP_CNFG_TcpStatus;

/*Structure definition of DHCP command*/
typedef PACK_START struct TCP_CNFG_DhcpStatus
{
	UINT8       uUseDHCP;
	UINT8	 	aIPaddr [4];
	UINT8	 	aNetMask [4];
	UINT8	 	aDefaultGateway [4];
} PACK_END TCP_CNFG_DhcpStatus;

/*Structure definition of socket status command*/
typedef PACK_START struct TCP_CNFG_SocketStatus
{
    SINT32      nSocketDescriptor;
    SINT8       Status;
    UINT8       FreeSocket;
} PACK_END TCP_CNFG_SocketStatus;

/*Structure definition of TCP statitics command*/
typedef PACK_START struct TCP_CNFG_TcpStatitics
{
	UINT32		nTcpTxPktCnt;
	UINT32		nTcpRxPktCnt;
	UINT32		nTcpTxBytes;
	UINT32 		nTcpRxBytes;
	UINT32 		nUdpTxPktCnt;
	UINT32 		nUdpRxPktCnt;
	UINT32 		nUdpTxBytes;
	UINT32 		nUdpRxBytes;

}PACK_END TCP_CNFG_TcpStatitics;

/*Structure definition of reserve port command*/
typedef PACK_START struct TCP_CNFG_RsrvPort
{
	SINT32 		TcpRsrvPort;
	SINT32		nErrorCode;

}PACK_END TCP_CNFG_RsrvPort;

/* Structure definition of Treck stack command*/
typedef PACK_START struct TCP_CNFG
{
    UINT16 uSubType;
    UINT16 ErrorCode;
    PACK_START union {
        TCP_CNFG_Configure      stConfigure;
        TCP_CNFG_Socket         stSocket;
        TCP_CNFG_Listen         stListen;
        TCP_CNFG_Bind           stBind;
        TCP_CNFG_Accept         stAccept;
        TCP_CNFG_Connect        stConnect;
        TCP_CNFG_SetSockOpt     stSetSockOpt;
        TCP_CNFG_GetSockOpt     stGetSockOpt;
        TCP_CNFG_GetPeerName    stGetPeerName;
        TCP_CNFG_GetSockName    stGetSockName;
        TCP_CNFG_ShutDown       stShutDown;
        TCP_CNFG_Close          stClose;
        TCP_CNFG_TcpStatus      stTcpStatus;
        TCP_CNFG_DhcpStatus     stDhcpStatus;
        TCP_CNFG_SocketStatus   stSocketStatus;
        TCP_CNFG_RsrvPort       stTcpRsrvPort;
        TCP_CNFG_TcpStatitics   stTcpStatistics;
    } PACK_END subCmd;
} PACK_END TCP_CNFG;

/* Structure definition of sendto event*/
typedef PACK_START struct TCP_CNFG_SendTo
{
    struct      packed_sockaddr_in stToAddress;
    SINT32      nBufferLength;
} PACK_END TCP_CNFG_SendTo;

/* Structure definition of configure event*/
typedef PACK_START struct TCP_EVT_Configure
{	UINT8       tTreckConfig;
	UINT8		uUseDHCP;
	UINT8	 	aIPaddr [4];
	UINT8	 	aNetMask [4];
	UINT8		aBroadcast [4];
	UINT8	 	aDefaultGateway [4];
	UINT8	 	aDnsIP [4];
} PACK_END TCP_EVT_Configure;

/* Structure definition of receive event*/
typedef PACK_START struct TCP_EVT_Recv
{
    SINT32      nSocketDescriptor;
    SINT32      nBufferLength;
    SINT8       aBuffer [MAX_RECV_PAYLOAD];
} PACK_END TCP_EVT_Recv;

/* Structure definition of receivefrom event*/
typedef PACK_START struct TCP_EVT_RecvFrom
{
    SINT32      nSocketDescriptor;
    SINT32      nBufferLength;
    struct  packed_sockaddr_in stFromAddr;
    SINT8       aBuffer [MAX_RECV_PAYLOAD];
} PACK_END TCP_EVT_RecvFrom;
/* Structure definition of accept event*/
typedef PACK_START struct TCP_EVT_Accept
{
    SINT32		nSocketDescriptor;
    struct	packed_sockaddr_in stPeerAddress;
    SINT32		nNewSockFd;
    SINT32		nErrorCode;
} PACK_END TCP_EVT_Accept;

/* Structure definition of connect event*/
typedef PACK_START struct TCP_EVT_Connect
{
    SINT32      nSocketDescriptor;
    struct      packed_sockaddr_in stAddress;
    SINT32		nErrorCode;
} PACK_END TCP_EVT_Connect;

/* Structure definition of remote terminate event*/
typedef PACK_START struct TCP_EVT_RemoteTerminate
{
   SINT32       nSocketDescriptor;
} PACK_END TCP_EVT_RemoteTerminate;

/* Structure definition of socket error event*/
typedef PACK_START struct TCP_EVT_SocketError
{
   SINT32   nSocketDescriptor;
   SINT32   nErrorCode;
} PACK_END TCP_EVT_SocketError;

/* Structure definition of connection loss event*/
typedef PACK_START struct TCP_EVT_ConnectionLoss {
    SINT32      nSocketDescriptor;
    SINT32      nErrorCode;
} PACK_END TCP_EVT_ConnectionLoss;
/* Structure definition of close compelte event*/
typedef PACK_START struct TCP_EVT_CloseComplete {
    SINT32      nSocketDescriptor;
    SINT32      nErrorCode;
} PACK_END TCP_EVT_CloseComplete;


/* Structure definition of Dhcp Ack event*/
/*This value should be less than TX_CMD_BUF_SIZE(1500)*/
#define MAX_DHCP_ACK_LEN	(TX_CMD_BUF_SIZE - 100)

typedef PACK_START struct TCP_EVT_DhcpAck
{
	UINT8		aDhcpAck[MAX_DHCP_ACK_LEN];
} PACK_END TCP_EVT_DhcpAck;

/* Structure definition of Treck stack event*/
typedef PACK_START struct sdio_TCPIPEvt_t
{
   	UINT16		uEvtSubType;
   	UINT16		uEvtBodyLen;
   	PACK_START union {
   		TCP_EVT_Configure			stConfigure;
   		TCP_EVT_Recv				stRecv;
   		TCP_EVT_RecvFrom			stRecvFrom;
   		TCP_EVT_Accept				stAccept;
   		TCP_EVT_Connect				stConnect;
   		TCP_EVT_RemoteTerminate		stRemoteTerminate;
   		TCP_EVT_ConnectionLoss      stConnectLost;
   		TCP_EVT_CloseComplete       stCloseComplete;
 		TCP_EVT_DhcpAck				stDhcpAck;
		TCP_EVT_SocketError			stSockErr;
   	} PACK_END EvtBody;
} PACK_END sdio_TCPIPEvt_t;

/* Structure definition of event queue*/
typedef PACK_START struct TcpEventQueue {
   	UINT32		Magic;
   	SINT32		SocketId;
} PACK_END TcpEventQueue;




/* Treck stack event numbers*/
typedef enum
{
	E_TCP_EVT_AssignIPAddressSuccess	= 0x01,
	E_TCP_EVT_AssignIPAddressFail		= 0x02,
	E_TCP_EVT_Recv						= 0x03,
	E_TCP_EVT_RecvFrom					= 0x04,
	E_TCP_EVT_Accept					= 0x05,
	E_TCP_EVT_Connect					= 0x06,
	E_TCP_EVT_ConnectionLoss            = 0x07,
	E_TCP_EVT_RemoteTerminateEvent		= 0x08,
	E_TCP_EVT_Ack			        	= 0x09,
	E_TCP_EVT_CloseComplete	        	= 0x0A,
	E_TCP_EVT_DhcpAck					= 0x0B,
	E_TCP_EVT_SocketError				= 0x0C
}TCP_EVT_SUB_TYPE;

typedef PACK_START union {
	TCP_CNFG_Configure		*pTcpCnfg;
	TCP_CNFG_Socket			*pSocket;
	TCP_CNFG_Listen			*pListen;
	TCP_CNFG_Bind			*pBind;
	TCP_CNFG_Accept			*pAccept;
	TCP_CNFG_SetSockOpt		*pSetSockOpt;
	TCP_CNFG_GetSockOpt		*pGetSockOpt;
	TCP_CNFG_GetPeerName	*pGetPeerName;
	TCP_CNFG_GetSockName	*pGetSockName;
	TCP_CNFG_Connect		*pConnect;
	TCP_CNFG_ShutDown		*pShutDown;
	TCP_CNFG_Close			*pClose;
	TCP_CNFG_SocketStatus	*pSocketStatus;
	TCP_CNFG_TcpStatus		*pTcpStatus;
	TCP_CNFG_DhcpStatus		*pTcpDhcpStatus;
	TCP_CNFG_RsrvPort		*pTcpGetRsrvPort;
	TCP_CNFG_TcpStatitics	*pTcpStatitics;

}PACK_END TCP_CNFG_GENPTR;

typedef struct treck_conf_t
{
    unsigned long   ipaddr;
    unsigned long   netmask;
    unsigned long   defaultgateway;
    unsigned long   dnsserver;
    unsigned char   configured;
} treck_conf_t;


extern volatile UINT16 gSockTable[P_MAX_SOCKET_NUM];
extern UINT32   TcpTxPktCnt;
extern UINT32   TcpRxPktCnt;
extern UINT32   TcpTxBytes;
extern UINT32   TcpRxBytes;
extern UINT32   UdpTxPktCnt;
extern UINT32   UdpRxPktCnt;
extern UINT32   UdpTxBytes;
extern UINT32   UdpRxBytes;
extern UINT8    treck_init;
extern uint8    treck_ready;

extern UINT32   DeactivateTreckTimer();
extern UINT32   CloseTreckInterface(void);
extern void     TcpEventConnect(SINT32);
extern void     TcpEventRemoteClose(SINT32);
extern void     TcpEventAccept(SINT32);
extern UINT8    openInterface(UINT32 ip,  UINT32  mask, UINT32 gatewy);

extern UINT32   uninitSockI(void);
extern UINT32   initSockI(void);
extern SINT32   get_sock_count(void);

extern UINT32   process_tcp_cmd(UINT8 * pBuff,int *);
extern void     QueueTcpEvent(SINT32,UINT32);
extern void     SocketCBFunc(int , int );
extern void     QueueTcpEventInit(void);
extern SINT32   get_free_index(void);
extern SINT32   find_sock_index(SINT32 sd);

extern UINT32	DeactivateTreckTimer();
extern UINT32	CloseTreckInterface(void);
extern void 	TcpEventConnect(SINT32);
extern void 	TcpEventRemoteClose(SINT32);
extern void		TcpEventAccept(SINT32);
extern void 	SockErrorToHost(SINT32, SINT32);
extern UINT8 	openInterface(UINT32 ip,  UINT32  mask, UINT32 gatewy);
extern void 	events(UINT8);
extern void   	eventf(void);
extern void     etcpconfigs(UINT8);
extern void   	etcpconfigf(void);

extern void SocketCBFunc(int socketDescriptor, int eventFlags);


extern volatile UINT16  TcpEvtIn,TcpEvtOut;

#endif
