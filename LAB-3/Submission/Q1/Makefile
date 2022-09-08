CC=gcc
CFLAGS=-Wall -g

all: clean client.c server.c
	$(CC) $(CFLAGS) -o client client.c
	$(CC) $(CFLAGS) -o server server.c

clean:
	rm -f client server
	rm -f *.txt