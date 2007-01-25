CDEFS=-DMDNS_HOST_TEST
CFLAGS=-Wall

main: main.o mdns.o
	gcc main.o mdns.o -o main

test: test.o mdns.o
	gcc test.o mdns.o -o test

main.o: main.c
	gcc $(CFLAGS) $(CDEFS) -c main.c

test.o: test.c
	gcc $(CFLAGS) $(CDEFS) -c test.c

mdns.o: mdns.c
	gcc $(CFLAGS) $(CDEFS) -c mdns.c

clean:
	rm *.o main
