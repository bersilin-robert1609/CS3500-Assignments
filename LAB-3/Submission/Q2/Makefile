CC=gcc
CFLAGS=-Wall -g

all: parent.c child.c clean data_1.txt data_2.txt data_3.txt data_4.txt data_5.txt
	$(CC) $(CFLAGS) parent.c -o parent
	$(CC) $(CFLAGS) child.c -o child

clean:
	rm -f parent child
	rm -f readfifo* writefifo*