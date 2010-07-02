#include "os_if.h"
#include "sysinfo.h"
#include "tx_api.h"

#include <trsocket.h>
#include "tcpip_socki.h"

#include "dbg_Facility.h"

#define DB_PRINT(list)        DBG_P( ( DBG_L0 list ) )
#define socket_close(s) tfClose(s)
#define socket_blocking_off(s) tfBlockingState(s,TM_BLOCKING_OFF) 
