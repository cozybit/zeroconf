CFLAGS += -I$(TOP)/include
OBJS = mdns.o debug.o

all: libmdns.a

libmdns.a: $(OBJS)
	$(AR) rcs $@ $^

clean:
	rm -f *.o libmdns.a
