CFLAGS += -I$(TOP)/include
OBJS = mdns.o debug.o dname.o

# Build-time options can be overriden from command line or higher-level
# makefiles
OPTIONS ?= \
	MDNS_CHECK_ARGS \
	MDNS_LOG \
	MDNS_DBG \
	MDNS_TESTS \
	MDNS_SYSTEM=LINUX

CFLAGS += $(addprefix -D, $(OPTIONS))

all: libmdns.a

libmdns.a: $(OBJS)
	$(AR) rcs $@ $^

clean:
	rm -f *.o libmdns.a
