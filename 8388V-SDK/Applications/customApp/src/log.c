/******************************************************************************
 * log.c
 *
 * 
 ******************************************************************************/

#include "log.h"
#include <stdarg.h>
int vsnprintf(char *str, size_t size, const char *format, va_list ap);

/******************************************************************************
 * Global Variables
 ******************************************************************************/

char log_buffer[LOG_SIZE];
unsigned long wp;
unsigned long rp;
sys_mutex log_mutex;
unsigned int log_running = 0;

#define LOG_INCREMENT(num, inc) (((num) + (inc)) & (LOG_SIZE - 1))

/******************************************************************************
 * Public Functions
 ******************************************************************************/

sys_status log_init(void)
{
	sys_status ret;
	if(log_running)
		return SYS_SUCCESS;
	memset(log_buffer, 0, LOG_SIZE);
	ret = sys_mutex_create(&log_mutex);
	if(ret == SYS_SUCCESS) {
		log_running = 1;
		rp = 0;
		wp = 0;
	}
	return ret;
}

sys_status log_purge(void)
{
	sys_status ret;
	ret = sys_mutex_get(&log_mutex, SYS_FOREVER);
	if(ret != SYS_SUCCESS)
		return ret;
	/* Totally reset the log */
	memset(log_buffer, 0, LOG_SIZE);
	wp = 0;
	rp = 0;
	ret = sys_mutex_release(&log_mutex);
	return ret;

}

sys_status log(char *str, ...)
{
	sys_status ret;
	int len, space;
	char final_str[MAX_LOG_LINE];
	va_list ap;

	if(!log_running)
		return SYS_SUCCESS;

	ret = sys_mutex_get(&log_mutex, SYS_FOREVER);
	if(ret != SYS_SUCCESS)
		return ret;
	
	/* Perform format conversion */
	va_start(ap, str);
	len = vsnprintf(final_str, MAX_LOG_LINE, str, ap);
	va_end(ap);
	if(len < 0) {
		sys_mutex_release(&log_mutex);
		return SYS_FAIL;
	}

	if(len > MAX_LOG_LINE) {
		final_str[MAX_LOG_LINE - 1] = '\0';
		len = MAX_LOG_LINE - 2;
	}

	/* Calculate space in buffer */
	if(rp == wp)
		space = LOG_SIZE - 1;
	else if(rp < wp)
		space = LOG_SIZE - (wp - rp) - 1;
	else
		space = rp - wp - 1;

	/* Write the data to the buffer */
	if(len > LOG_SIZE - 1)
		len = LOG_SIZE - 1;

	if(len < LOG_SIZE - wp) {
		memcpy(&log_buffer[wp], final_str, len);
		wp += len;
	} else {
		memcpy(&log_buffer[wp], final_str, LOG_SIZE - wp);
		memcpy(&log_buffer[0], &final_str[LOG_SIZE - wp], len - (LOG_SIZE - wp));
		wp = len - (LOG_SIZE - wp) + 1;
	}

	/* Update read pointer */
	if(len > space) {
		rp = LOG_INCREMENT(wp, 1);
	}

	ret = sys_mutex_release(&log_mutex);
	return ret;
}

sys_status log_dump(void)
{
	sys_status ret;
	int ptr = rp;
	ret = sys_mutex_get(&log_mutex, SYS_FOREVER);
	if(ret != SYS_SUCCESS)
		return ret;

	while(ptr != wp) {
		DEBUG("%c", log_buffer[ptr]);
		ptr = LOG_INCREMENT(ptr, 1);
	}

	ret = sys_mutex_release(&log_mutex);
	return ret;
}

sys_status log_shutdown(void)
{
	sys_status ret;
	ret = sys_mutex_get(&log_mutex, SYS_FOREVER);
	if(ret != SYS_SUCCESS)
		return ret;
	log_running = 0;
	return sys_mutex_delete(&log_mutex);
}

