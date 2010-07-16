# Copyright (C) cozybit, Inc. 2007-2010. All Rights Reserved.
#
# Use and redistribution subject to licensing terms.

CFLAGS += -I$(TOP)/include

MDNS_DIR = $(TOP)/mdns
LIBS = $(MDNS_DIR)/libmdns.a
OBJS = main.o

all: mdns

mdns: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o mdns

clean:
	rm -f mdns $(OBJS)
