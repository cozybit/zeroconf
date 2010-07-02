CFLAGS = -DMDNS_HOST_TEST -Wall -I$(TOP)/include

MDNS_DIR = $(TOP)/mdns
LIBS = $(MDNS_DIR)/libmdns.a

all: mdns

mdns: main.o os_linux.o
	$(CC) $^ $(LIBS) -o mdns

clean:
	rm -f mdns
