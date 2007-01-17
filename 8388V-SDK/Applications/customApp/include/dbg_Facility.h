/***************** (c) Marvell Semiconductor, Inc., 2005 **********************
 *
 * $Header $
 *
 * Debug Print Utility
 *
 ******************************************************************************/

#ifndef _dbg_facility_h_
#define _dbg_facility_h_

#ifdef DBG_FACILITY
extern void dbg_Print(const char *format, ...);
extern void dbg_Monitor(void);

#define DBG_P(plist) dbg_Print##plist

#else
#define dbg_Print
#define dbg_Monitor

#define DBG_P(plist)

#endif

#ifdef DBG_FACILITY
#define DBG_ASSERT(expr)    do {    \
                                if(!(expr)) \
                                {   \
                                    dbg_OutputEnable(); \
                                    dbg_SwitchToPollingMode(); \
                                    while(1) { \
                                        dbg_Print("ASSERT %s line %d %s\r\n",  __FILE__, __LINE__, #expr); \
                                        dbg_FlushingOut(); \
                                    } \
                                }   \
                            } while(0)

                            /* Switching to polling mode should be done
                               because ASSERT fail could happen inside ISR,
                               where IRQ is disable.
                            */
#else
#define DBG_ASSERT(expr)
#endif

#define DBG_L0 ":0:"
#define DBG_L1 ":1:"
#define DBG_L2 ":2:"
#define DBG_L3 ":3:"

#define DBG_TXD ":001:"
#define DBG_RXD ":002:"
#define DBG_CBP ":003:"
#define DBG_NOG ":000:"

#define DBG_PORT_UART       1
#define DBG_PORT_HOST       2
#define DBG_PORT_DCC        3
#define DBG_PORT_MEM        4

extern void dbg_PutChar( UINT8 u8c );
extern void dbg_PutString( char *pStr, int len );
extern UINT8 dbg_GetChar( UINT8 *pu8c );
extern void dbg_FlushingOut(void);
extern void dbg_SwitchToIsrMode(void);
extern void dbg_SwitchToPollingMode(void);

extern void dbg_OutputEnable(void);
extern void dbg_OutputDisable(void);
extern UINT8 dbg_ChangePrintLevel( UINT8 newLevel );
extern void dbg_ChangeDbgPort( UINT8 newPort );
extern UINT8 dbg_GetCurrentDbgPort( void );


extern void dbg_HostPort_PutChar( UINT8 u8c );
extern void dbg_HostPort_PutString( char *pStr, int len );
extern UINT8 dbg_HostPort_GetChar( UINT8 *pu8c );
extern void dbg_HostPort_FlushingOut(void);

#if defined(DCC_DRV)
#define DEFAULT_DBG_PORT DBG_PORT_DCC
#elif defined(UART_DRV)
#define DEFAULT_DBG_PORT DBG_PORT_UART
#else
#define DEFAULT_DBG_PORT DBG_PORT_HOST
#endif

#define DEFAULT_DBG_PRINT_LEVEL 3
#define DEFAULT_DBG_PRINT_ENABLE 1

#endif // _dbg_facility_h_

