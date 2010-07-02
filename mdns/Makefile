CDEFS=-DMDNS_HOST_TEST
CFLAGS=-Wall

mdns: main.o mdns.o debug.o
	gcc main.o mdns.o debug.o -o mdns

test: test.o mdns.o
	gcc test.o mdns.o -o test

main.o: main.c
	gcc $(CFLAGS) $(CDEFS) -c main.c

mdns.o: mdns.c
	gcc $(CFLAGS) $(CDEFS) -c mdns.c

debug.o: debug.c
	gcc $(CFLAGS) $(CDEFS) -c debug.c

clean:
	rm -f *.o mdns
