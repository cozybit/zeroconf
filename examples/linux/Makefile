CFLAGS = -DMDNS_HOST_TEST -Wall -I$(TOP)/include

MDNS_DIR = $(TOP)/mdns
LIBS = $(MDNS_DIR)/libmdns.a
OBJS = main.o

all: mdns

mdns: $(OBJS)
	$(CC) $^ $(LIBS) -o mdns

clean:
	rm -f mdns $(OBJS)
