/******************************************************************************
 * log.h
 *
 * Simple circular log for cozybit components
 ******************************************************************************/

#ifndef _LOG_H_
#define _LOG_H_

#include "system.h"

/******************************************************************************
 * Parameters
 ******************************************************************************/

#define LOG_SIZE 512 /* Must be a power of 2! */
#define MAX_LOG_LINE 128 /* Maximum length of a single log line */

/******************************************************************************
 * Public Functions
 ******************************************************************************/

sys_status log_init(void);
sys_status log_purge(void);
sys_status log(char *str, ...);
sys_status log_dump(void);
sys_status log_shutdown(void);

#endif
