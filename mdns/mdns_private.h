#ifndef __MDNS_PRIVATE_H__
#define __MDNS_PRIVATE_H__

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
extern char *statenames[];
extern char *eventnames[];
#endif

/* logging helpers */
#ifdef MDNS_LOG
#define LOG mdns_log
#ifdef MDNS_DBG
void debug_print_message(struct mdns_message *m);
void debug_print_name(struct mdns_message *m, char *name);
#define DBG mdns_log
#else
#define debug_print_message(m) do {} while (0)
#define debug_print_name(m, n) do {} while (0)
#define DBG(...) do {} while (0)
#endif

#else
#define debug_print_message(m) do {} while (0)
#define debug_print_name(m, n) do {} while (0)
#define DBG(...) do {} while (0)
#define LOG(...) do {} while (0)
#endif

/* helpers for accessing elements */
#define get_uint16(p) (ntohs(*((uint16_t *)(p))))
#define get_uint32(p) (ntohl(*((uint32_t *)(p))))
#define set_uint16(p, v) (*((uint16_t*)(p)) = htons((v)))
#define set_uint32(p, v) (*((uint32_t*)(p)) = htonl((v)))

/* helpers for handling dns names */
char *dname_put_label(char *dst, char *label);
int dname_size(char *dname);
int dname_increment(char *name);
int dname_cmp(char *p1, char *n1, char *p2, char *n2);

#ifdef MDNS_TESTS

#define FAIL_UNLESS(condition, ...) \
	do { \
		if (!(condition)) { \
			printf("FAIL: %s: %d: ", __FILE__, __LINE__); \
			printf(__VA_ARGS__); \
			printf("\n"); \
		} else { \
			printf("PASS: %s: %d\n", __FILE__, __LINE__); \
		} \
	} while (0)


#define test_title(s) mdns_log("========= %s =========\n", s)

#endif

void dname_tests(void);

#endif /* __MDNS_PRIVATE_H__ */
