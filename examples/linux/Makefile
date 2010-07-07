CFLAGS += -Wall -I$(TOP)/include

MDNS_DIR = $(TOP)/mdns
LIBS = $(MDNS_DIR)/libmdns.a
OBJS = main.o

all: mdns

mdns: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o mdns

clean:
	rm -f mdns $(OBJS)
