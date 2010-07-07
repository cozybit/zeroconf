#ifndef MDNS_H
#define MDNS_H

#include <stdint.h>

#define MDNS_CTRL_PORT 12345

int mdns_launch(uint32_t ipaddr);

void mdns_halt(void);

#endif
