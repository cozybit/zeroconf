#ifndef DEBUG_H
#define DEBUG_H

#ifdef MDNS_HOST_TEST
#include "host.h"
#else
#include "target.h"
#endif
#include "mdns.h"

#define TEST_IP	0xC0A80150

void debug_print_message( struct mdns_message *m );

#endif
