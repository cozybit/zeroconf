# Copyright (C) cozybit, Inc. 2007-2010. All Rights Reserved.
# Use and redistribution subject to licensing terms.

CFLAGS += -I$(TOP)/include
OBJS = mdns.o mdns_respond.o mdns_query.o debug.o dname.o

all: libmdns.a

libmdns.a: $(OBJS)
	$(AR) rcs $@ $^

clean:
	rm -f *.o libmdns.a
