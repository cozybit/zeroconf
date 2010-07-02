#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include <stdint.h>
#define UINT8	uint8_t
#define UINT16	uint16_t
#define UINT32	uint32_t

#define DB_PRINT		printf
#define socket_close(s)	close(s)
#define socket_blocking_off(s)	fcntl(s, F_SETFL, ( fcntl(s, F_GETFL, 0) | O_NONBLOCK ) );
