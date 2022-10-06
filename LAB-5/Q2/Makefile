CC=gcc
CFLAGS=-Wall -g -lpthread -lrt

all: clean multi_pc.c
	$(CC) $(CFLAGS) -o multi_pc multi_pc.c

clean:
	rm -f multi_pc