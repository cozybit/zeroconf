TOP = $(CURDIR)
export TOP

CFLAGS += -Wall
ifneq ($(DEBUG),)
	CFLAGS += -g
endif
export CFLAGS

.PHONY: mdns linux

all: mdns linux

mdns/version.h:
	./scripts/version.sh

mdns: mdns/version.h
	$(MAKE) -C mdns

linux:
	$(MAKE) -C examples/linux

clean:
	$(MAKE) -C examples/linux clean
	$(MAKE) -C mdns clean
	- rm mdns/version.h
