CDEFS=-DMDNS_HOST_TEST
CFLAGS=-Wall

main: main.o mdns.o debug.o
	gcc main.o mdns.o debug.o -o main

test: test.o mdns.o
	gcc test.o mdns.o -o test

main.o: main.c
	gcc $(CFLAGS) $(CDEFS) -c main.c

test.o: test.c
	gcc $(CFLAGS) $(CDEFS) -c test.c

mdns.o: mdns.c
	gcc $(CFLAGS) $(CDEFS) -c mdns.c

debug.o: debug.c
	gcc $(CFLAGS) $(CDEFS) -c debug.c

clean:
	rm *.o main
