/******************** (c) Marvell Semiconductor, Inc., 2001 *******************
 *
 * $Header: //depot/MRVL/EEBU_W8686_V5_DEV/wlan/prod/ecl/STA/Incl/sysinfo.h#21 $
 *
 * Purpose:
 *    This file contains the configuration parameters used for tasks, queues,
 *    and other operating system components.
 *
 * Public Procedures:
 *    None.
 *
 * Notes:
 *    None.
 *
 *****************************************************************************/

#ifndef _SYSINFO_H_
#define _SYSINFO_H_

/*============================================================================= */
/*                            PUBLIC DEFINITIONS */
/*============================================================================= */

/*--------------*/
/* System Sizes */
/*--------------*/
#define  sysinfo_MAX_STATIONS        64
#ifdef MFG_UART_FW
#define  sysinfo_TOTAL_TASKS         7
#else
#define  sysinfo_TOTAL_TASKS         6
#endif

/*
    Decrease sysinfo_TOTAL_QUEUES to 5 in order
	to free up DTCM memory space to resolve
    the following two problems.

	1. DTCM memory space was running out.
	 - When DCC is enabled, DCC buffers destroys
	    OS internal data structures.
	 - FIQ stack also could invade OS internal
	    data structures.

	2. CmdProc Task's stack overflow
	  - When DBG_FACILITY is included in build,
	    there is a stack overflow when printing
	    out some dbug message in MLME routines.
	  - Even without including DBG_FACILITY,
	    remaining stack space is too low.
	    It could be a potential risk in
	    stability.

	   It is required to have free DTCM space
	   to increase CmdProc stack size.

	Only the following five queue are being used
	in firmware. Delete other queue's #define
	and decrease sysinfo_TOTAL_QUEUES to 5 from
	15, so that prevent from allocating unused
	memory for queue data structures.

	sysinfo_TIMER_CALL_BACK_Q
	sysinfo_MAC_MGMT_802_11_MSG_Q
	sysinfo_MAC_MGMT_SME_MSG_Q
	sysinfo_CB_PROC_SME_MSG_Q
	sysinfo_802_11_MSG_TX_MGMT_Q

	By doing this, 768 bytes of DTCM memory
	newly became available, which is 10% of
	total DTCM size.
*/

#define  sysinfo_TOTAL_QUEUES        5

/* Definition of sysinfo_TOTAL_INTERRUPTS */
#if defined(ADHOC_PS) || defined(AUPSD)

  #define COUNT_TIMER2_INTERRUPTS   1
#else
  #define COUNT_TIMER2_INTERRUPTS   0
#endif
#ifdef BCNMISS_PATCH
  #define COUNT_TIMER3_INTERRUPTS   1
#else
  #define COUNT_TIMER3_INTERRUPTS   0
#endif
#ifdef UART_DRV
  #define COUNT_UART_INTERRUPTS   1
#else
  #define COUNT_UART_INTERRUPTS   0
#endif
#ifdef DCC_DRV
  #define COUNT_DCC_INTERRUPTS   2
#else
  #define COUNT_DCC_INTERRUPTS   0
#endif
#ifdef CRYPTOHA
  #define COUNT_IPS_INTERRUPTS   2
#else
  #define COUNT_IPS_INTERRUPTS   0
#endif
#ifdef BLUETOOTH
  #define COUNT_BLUETOOTH_INTERRUPTS   1
#else
  #define COUNT_BLUETOOTH_INTERRUPTS   0

#endif /* defined(ADHOC_PS) || defined(AUPSD) */


#define  sysinfo_TOTAL_INTERRUPTS  ( 9 \
            + COUNT_TIMER2_INTERRUPTS \
            + COUNT_TIMER3_INTERRUPTS \
            + COUNT_UART_INTERRUPTS \
            + COUNT_DCC_INTERRUPTS \
            + COUNT_IPS_INTERRUPTS \
            + COUNT_BLUETOOTH_INTERRUPTS )

#define  sysinfo_TOTAL_TIMERS        6    /*10 -> 5 -> 6 */

#ifdef MARVELL_SDK_SUPPORT
#define NO_HOST_SEM                  1
#else
#define NO_HOST_SEM                  0
#endif

#if defined(ADHOC_PS)
#ifdef FULL_BG_SCAN
#define  sysinfo_TOTAL_SEMAPHORES    (11 + NO_HOST_SEM)
#else
#define  sysinfo_TOTAL_SEMAPHORES    (10 + NO_HOST_SEM)
#endif   //FULL_BG_SCAN
#else
#ifdef FULL_BG_SCAN
#define  sysinfo_TOTAL_SEMAPHORES    (8 + NO_HOST_SEM)
#else
#define  sysinfo_TOTAL_SEMAPHORES    (7 + NO_HOST_SEM)
#endif   //FULL_BG_SCAN
#endif  /* ADHOC_PS */

#if defined(EMBEDDED_TCPIP) || defined(MARVELL_SDK_SUPPORT)
#define  sysinfo_TOTAL_EVENTS        11
#else
#define  sysinfo_TOTAL_EVENTS        8
#endif
#define  sysinfo_TOTAL_CONDITIONS    2

/*----------*/
/* Task IDs */
/*----------*/
#define  sysinfo_802_11_TX_NOTIFY_TASK    0
#define  sysinfo_IDLE_TASK                1
#define  sysinfo_CB_PROC_TASK             2
#define  sysinfo_SME_TASK                 3
#define  sysinfo_MAC_MGMT_TASK            4
#define  sysinfo_802_11_MSG_TX_TASK       5
#define  sysinfo_802_11_MSG_RX_TASK       6
#define  sysinfo_BRIDGE_TASK              7
#define	 sysinfo_UART_CMD			  	  sysinfo_802_11_MSG_RX_TASK


/*------------*/
/* Task Names */
/*------------*/
#define  sysinfo_ROOT_TASK_NAME              "Root"
#define  sysinfo_IDLE_TASK_NAME              "Idle"
#define  sysinfo_CB_PROC_TASK_NAME           "CB Proc"
#define  sysinfo_SME_TASK_NAME               "SME"
#define  sysinfo_MAC_MGMT_TASK_NAME          "MAC Mgmt"
#define  sysinfo_802_11_MSG_TX_TASK_NAME     "MAC Tx"
#define  sysinfo_802_11_MSG_RX_TASK_NAME     "MAC Rx"
#define  sysinfo_BCN_GENERATION_TASK_NAME    "BCN Gen"
#define  sysinfo_802_11_TX_NOTIFY_TASK_NAME  "MAC Tx Notify"
#define  sysinfo_BRIDGE_TASK_NAME            "Bridge"
#define  sysinfo_UART_CMD_TASK_NAME     	 "UART Cmd"


/*-----------------*/
/* Task Priorities */
/*-----------------*/
#define  sysinfo_CB_PROC_TASK_PRI             7 //9 //2 -> 7 -> 9
#define  sysinfo_802_11_TX_NOTIFY_TASK_PRI    5 //5 -> 2 -> 5
#define  sysinfo_802_11_MSG_TX_TASK_PRI       7 //7 -> 5 -> 7
#define  sysinfo_802_11_MSG_RX_TASK_PRI       2 //9 -> 2
#define  sysinfo_MAC_MGMT_TASK_PRI            7
#define  sysinfo_SME_TASK_PRI                 19
#define  sysinfo_ROOT_TASK_PRI                25
#define  sysinfo_IDLE_TASK_PRI                30
#define	 sysinfo_UART_CMD_TASK_PRI			  9

/*------------------*/
/* Task Stack Sizes */
/*------------------*/
#ifdef EMBEDDED_TCPIP
#define  sysinfo_IDLE_TASK_STACK_SIZE            1024 //440  /*360 -> 460 */
#define  sysinfo_CB_PROC_TASK_STACK_SIZE         2500 //672
#define  sysinfo_SME_TASK_STACK_SIZE             0    /* 440 -> 0 */
#define  sysinfo_MAC_MGMT_TASK_STACK_SIZE        1100 /*1256 -> 1200 -> 1100 */
#define  sysinfo_802_11_MSG_TX_TASK_STACK_SIZE   (524 + 440)  /*424 -> 524 -> (524+440) */
#define  sysinfo_802_11_MSG_RX_TASK_STACK_SIZE   556 //0  /* 456 -> 556 -> 0 */
#define  sysinfo_802_11_TX_MAC_DATA_TASK_STACK_SIZE 1200/*1048 -> 648 -> 748 */
#else
#define  sysinfo_IDLE_TASK_STACK_SIZE            440  /*360 -> 460 */
#ifdef MFG_FW
#define  sysinfo_CB_PROC_TASK_STACK_SIZE         1672
#else
#define  sysinfo_CB_PROC_TASK_STACK_SIZE         672
#endif //#ifdef MFG_FW
#define  sysinfo_SME_TASK_STACK_SIZE             0    /* 440 -> 0 */
#define  sysinfo_MAC_MGMT_TASK_STACK_SIZE        1100 /*1256 -> 1200 -> 1100 */
#define  sysinfo_802_11_MSG_TX_TASK_STACK_SIZE   (524 + 440)  /*424 -> 524 -> (524+440) */
#define  sysinfo_802_11_MSG_RX_TASK_STACK_SIZE   0  /* 456 -> 556 -> 0 */
#ifdef MFG_FW
#define  sysinfo_802_11_TX_MAC_DATA_TASK_STACK_SIZE 200/*1048 -> 648 -> 748 */
#else
#define  sysinfo_802_11_TX_MAC_DATA_TASK_STACK_SIZE 1200/*1048 -> 648 -> 748 */
#endif //#ifdef MFG_FW
#define  sysinfo_UART_CMD_TASK_STACK_SIZE 		 1500
#endif

#define  sysinfo_TOTAL_STACK_SIZE                     \
            sysinfo_IDLE_TASK_STACK_SIZE +            \
            sysinfo_CB_PROC_TASK_STACK_SIZE +         \
            sysinfo_SME_TASK_STACK_SIZE +             \
            sysinfo_MAC_MGMT_TASK_STACK_SIZE +        \
            sysinfo_802_11_MSG_TX_TASK_STACK_SIZE +   \
			sysinfo_802_11_TX_MAC_DATA_TASK_STACK_SIZE
            /* To reduce stack size; With WPA enabled, every byte is important;
               There is no RX task; */
            // sysinfo_802_11_MSG_RX_TASK_STACK_SIZE

/*-----------*/
/* Queue IDs */
/*-----------*/
#define  sysinfo_CB_PROC_SME_MSG_Q        0
#define  sysinfo_MAC_MGMT_802_11_MSG_Q    1 // 6
#define  sysinfo_MAC_MGMT_SME_MSG_Q       2 // 7
#define  sysinfo_802_11_MSG_TX_MGMT_Q     3 // 11
#define  sysinfo_TIMER_CALL_BACK_Q        4 // 14

/*-----------*/
/* Event IDs */
/*-----------*/
#define  sysinfo_CB_PROC_Q_EVENT             0
#define  sysinfo_SME_MAIN_Q_EVENT            1
#define  sysinfo_MAC_MGMT_MAIN_Q_EVENT       2
#define  sysinfo_802_11_TX_NOTIFY_Q_EVENT    3
#define  sysinfo_802_11_RX_EVENT             4
#define  sysinfo_802_11_TX_Q_EVENT           5
#define  sysinfo_BCN_FREE_EVENT              6
#define  sysinfo_CRYPTOHA_COMPLETE_EVENT     7

#if defined(EMBEDDED_TCPIP) && !defined(MARVELL_SDK_SUPPORT)
#define  sysinfo_TRECK_RxTx_EVENT            8
#define  sysinfo_TRECK_Host_EVENT            9
#endif
#ifdef MARVELL_SDK_SUPPORT
#define  sysinfo_SDK_RX_EVENT            	8
#define  sysinfo_SDK_TX_EVENT            	9
#endif

#define sysinfo_NO_HOST_EVENT            	10

/*---------------*/
/* Semaphore IDs */
/*---------------*/
#define  sysinfo_EXT_STA_SEM   0
#define  sysinfo_TIMER_SEM     1
#define  sysinfo_TIMER_DB_SEM  2
#define  sysinfo_PWRSAV_SEM    3
#define  sysinfo_DIAG_SEM      4

#if defined(ADHOC_PS)
#define sysinfo_adhoc_ps_tx_SEM     5
#define sysinfo_adhoc_ps_cb_SEM     6
#define sysinfo_tx_SEM              7
#endif

#ifdef FULL_BG_SCAN
#ifdef ADHOC_PS
#define  sysinfo_BG_SCAN       8
#else
#define  sysinfo_BG_SCAN       5
#endif
#endif

#if defined(ADHOC_PS)
#ifdef FULL_BG_SCAN
#define  sysinfo_CryptoHA_FRQ_SEM    9
#else
#define  sysinfo_CryptoHA_FRQ_SEM    8
#endif   //FULL_BG_SCAN
#else
#ifdef FULL_BG_SCAN
#define  sysinfo_CryptoHA_FRQ_SEM    6
#else
#define  sysinfo_CryptoHA_FRQ_SEM    5
#endif   //FULL_BG_SCAN
#endif



#if defined(ADHOC_PS)
#ifdef FULL_BG_SCAN
#define  sysinfo_TKIP_WEP_AES_SEM    10
#else
#define  sysinfo_TKIP_WEP_AES_SEM    9
#endif   //FULL_BG_SCAN
#else
#ifdef FULL_BG_SCAN
#define  sysinfo_TKIP_WEP_AES_SEM    7
#else
#define  sysinfo_TKIP_WEP_AES_SEM    6
#endif   //FULL_BG_SCAN
#endif




/*---------------*/
/* Condition IDs */
/*---------------*/
#define  sysinfo_TX_BUF_COND   0
#define  sysinfo_TX_INFO_COND  1


/*---------------*/
/* Interrupt IDs */
/*---------------*/
#define  sysinfo_RX_MGMT_MSG_INT          0
#define  sysinfo_RX_DATA_MSG_INT          1
#define  sysinfo_RX_CTL_MSG_INT           2
#define  sysinfo_RX_BCN_INT               3
#define  sysinfo_RX_HOST_MSG_INT          4
#define  sysinfo_TX_NOTIFICATION_INT      5
#define  sysinfo_PWR_MODE_UPDATE_INT      6


#endif /* _SYSINFO_H_ */
