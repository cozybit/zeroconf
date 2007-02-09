/******************************************************************************
 * httpd.h
 *
 * Simple http server
 ******************************************************************************/

#include "system.h"
#include "httpd.h"

/******************************************************************************
 * Parameters and variables
 ******************************************************************************/

static int httpd_running = 0;
static sys_thread httpd_main_thread;
static unsigned char httpd_stack[1048];
static char httpd_message_in[HTTPD_MAX_MESSAGE];
static char httpd_message_out[HTTPD_MAX_MESSAGE];
static int sock;
/* Debugging and logging */
#define LL_DEBUG
#ifdef LL_DEBUG
#include "log.h"
#define LOG(...) log(__VA_ARGS__)
#else
#define LOG(...) {}
#endif

/******************************************************************************
 * Private Functions
 ******************************************************************************/

sys_status httpd_process_request(char *request, char *response)
{
	/* Create http response */
	strcpy(response,
		   "HTTP/1.0 200 Not Found\r\nContent-Type: text/html\r\n\r\nHELLO WORLD\r\n");
	return SYS_SUCCESS;

}

sys_thread_return httpd_main(sys_thread_data data)
{
	int error;
	int conn = -1;
	int active_fds;
	fd_set fds;
	struct timeval timeout;
	struct sockaddr_in addr_listen;
	struct sockaddr_in addr_from;
	int addr_len;
	int addr_from_len;
	int len;
	int max_sock;
	int one = 1;
	sys_status ret;

	/* create listening TCP socket */  
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock < 0) {
		error = sys_socket_error(sock);
		return;
	}

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one));
	addr_listen.sin_family = AF_INET;
	addr_listen.sin_port = htons(HTTPD_PORT);
	addr_listen.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_len = sizeof(struct sockaddr_in);

	/* bind insocket */
	error = bind(sock, (struct sockaddr *)&addr_listen, addr_len);
	if ( error < 0 ) {
		error = sys_socket_error(sock);
		return;
	}

	/* make it non block, so we can timeout using select */
	sys_socket_blocking_off(sock);
	listen(sock, 5);
	LOG("HTTPD %d: Listening on port %d.\r\n", sys_time_get(), HTTPD_PORT);

	/* Prepare to wait for connections */
	addr_from_len = sizeof(addr_from);
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	while(1) {

		FD_ZERO(&fds);
		FD_SET(sock, &fds);

		max_sock = (sock > conn) ? sock : conn;

		if(conn != -1)
			FD_SET(conn, &fds);

		active_fds = select(max_sock + 1, &fds, NULL, NULL, &timeout);

		/* Error in select? */
		if(active_fds < 0) {
			LOG("HTTPD %d: select failed %d.\r\n", sys_time_get(), active_fds);
			return;
		}

		/* Time out */
		if(!active_fds)
			continue;

		/* New connection */
		if(FD_ISSET(sock, &fds)) {
			conn = accept(sock, (struct sockaddr *)&addr_from,
						  &addr_from_len);
			LOG("HTTPD %d: connected to %x.\r\n", sys_time_get(),
				addr_from.sin_addr.s_addr);
			continue;
		}

		/* Existing connection */
		if (FD_ISSET(conn, &fds)) {

			len = recv(conn, httpd_message_in, HTTPD_MAX_MESSAGE, 0);

			/* connection broken */
			if (len <= 0) {
				LOG("HTTPD %d: lost connected to %x.\r\n", sys_time_get(),
					addr_from.sin_addr.s_addr);
				sys_socket_close(conn);
				conn = -1;
				continue;
			}
			
			/* Get page URL */
			ret = httpd_process_request(httpd_message_in, httpd_message_out);
			if(ret != SYS_SUCCESS) {
				LOG("HTTPD %d: failed to process http request.\r\n",
					sys_time_get());
				continue;
			}

			/* Send http response */
			send(conn, httpd_message_out, strlen(httpd_message_out), 0);
			LOG("HTTPD %d: sent response to %x.\r\n", sys_time_get(),
				addr_from.sin_addr.s_addr);
			sys_socket_close(conn);
			conn = -1;			
		}
	}
}

/******************************************************************************
 * Public Functions
 ******************************************************************************/

sys_status httpd_init(void)
{
	sys_status ret;

	LOG("HTTPD %d: Initializing\r\n", sys_time_get());
	if(httpd_running != 0)
		return SYS_FAIL;

	/* Launch the httpd main thread. */
	ret = sys_thread_create(&httpd_main_thread, httpd_main, 0,
							(void *)&httpd_stack[0], sizeof(httpd_stack));
	if(ret != SYS_SUCCESS)
		return ret;

	httpd_running = 1;
	return ret;
}

sys_status httpd_shutdown(void)
{
	LOG("HTTPD %d: Shutting down.\r\n", sys_time_get());
	sys_thread_halt(&httpd_main_thread);
	sys_thread_delete(&httpd_main_thread);
	sys_socket_close(sock);
	httpd_running = 0;
	return SYS_SUCCESS;
}
