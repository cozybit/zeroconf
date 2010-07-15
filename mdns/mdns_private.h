#include "mdns_message.h"

enum mdns_status_t {
	INIT,
	FIRST_PROBE_SENT,	/* wait a random amount of time and probe */
	SECOND_PROBE_SENT,	/* wait 250ms and probe */
	THIRD_PROBE_SENT,	/* wait 250ms and probe */
	IDLE,				/* we have claimed our name */
};

enum mdns_event_t {
	EVENT_RX,		/* recieved a DNS packet */
	EVENT_CTRL,		/* recieved a control message */
	EVENT_TIMEOUT,	/* timed out waiting for DNS packet */
};

#ifdef MDNS_DBG
static char *statenames[] = {
	"FIRST_PROBE_SENT",
	"SECOND_PROBE_SENT",
	"THIRD_PROBE_SENT",
	"IDLE",
};

static char *eventnames[] = {
	"EVENT_RX",
	"EVENT_CTRL",
	"EVENT_TIMEOUT",
};
#endif

/* logging helpers */
#ifdef MDNS_LOG
#define LOG mdns_log
#ifdef MDNS_DBG
void debug_print_message(struct mdns_message *m);
#define DBG mdns_log
#else
#define debug_print_message {}
#define DBG {}
#endif

#else
#define debug_print_message {}
#define DBG {}
#define LOG {}
#endif

/* helpers for accessing elements */
#define get_uint16(p) (ntohs(*((uint16_t *)(p))))
#define get_uint32(p) (ntohl(*((uint32_t *)(p))))
#define set_uint16(p, v) (*((uint16_t*)(p)) = htons((v)))
#define set_uint32(p, v) (*((uint32_t*)(p)) = htonl((v)))
