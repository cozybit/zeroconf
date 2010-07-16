TOP = $(CURDIR)
export TOP

# Build-time options can be overriden from command line or higher-level
# makefiles
OPTIONS ?= \
	MDNS_CHECK_ARGS \
	MDNS_LOG \
	MDNS_DBG \
	MDNS_TESTS \
	MDNS_SYSTEM_LINUX

CFLAGS += $(addprefix -D, $(OPTIONS))
export OPTIONS

CFLAGS += -Wall
ifneq ($(DEBUG),)
	CFLAGS += -g
endif
export CFLAGS

.PHONY: mdns linux

SYSTEM = $(filter MDNS_SYSTEM_%, $(OPTIONS))
ifeq (,$(SYSTEM))
	PROGRAM = systemnotdefined
endif

ifeq ($(SYSTEM), MDNS_SYSTEM_LINUX)
	PROGRAM = linux
else
	PROGRAM = nosuchsystem
endif

all: $(PROGRAM)

mdns:
	$(MAKE) -C mdns

linux: mdns
	$(MAKE) -C examples/linux

systemnotdefined:
	echo "No MDNS_SYSTEM_* defined"
	exit 1

nosuchsystem:
	echo "No such system: $(SYSTEM)"
	exit 1

clean:
	$(MAKE) -C examples/linux clean
	$(MAKE) -C mdns clean