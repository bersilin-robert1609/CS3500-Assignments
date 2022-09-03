CC=gcc
CFLAGS=-Wall -g

all: clean client.c server.c
	$(CC) $(CFLAGS) -o client.o client.c
	$(CC) $(CFLAGS) -o server.o server.c

clean:
	rm -f *.o