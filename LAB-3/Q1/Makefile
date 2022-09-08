CC=gcc
CFLAGS=-Wall -g

all: clean client.c server.c
	$(CC) $(CFLAGS) -o client.out client.c
	$(CC) $(CFLAGS) -o server.out server.c

clean:
	rm -f *.out
	rm -f *.txt