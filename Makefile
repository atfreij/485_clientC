#Andy White
#Senior Design
#Client

CC=gcc
CFLAGS=-std=c11 -g -Wall -pedantic

arrival: client_main.c client_network.c serial.c
	$(CC) $(CFLAGS) -D ARRIVAL -o$@ $^ 

departure: client_main.c client_network.c serial.c
	$(CC) $(CFLAGS) -o$@ $^

clean:
	rm -f client
