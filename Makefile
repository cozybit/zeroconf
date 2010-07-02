TOP = $(CURDIR)
export TOP

.PHONY: mdns linux

all: mdns linux

mdns:
	$(MAKE) -C mdns

linux:
	$(MAKE) -C examples/linux

clean:
	$(MAKE) -C examples/linux clean
	$(MAKE) -C mdns clean