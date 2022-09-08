#define REQUEST 100
#define PIVOT 200
#define LARGE 300
#define SMALL 400
#define READY 500

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]) {
    char *myreadfifo = "Child_W";
    char *mywritefifo = "Child_R";

    // You don't have to create fifos here as this program 
    // can use the same fifos created by other program

    // char writefifo[10] = "Child1W";
    // sprintf(writefifo, "Child%sW", argv[1]);
    // printf("In Child process %s: %s\n", argv[1], writefifo);
    // char readfifo[10] = "Child1R";
    // sprintf(writefifo, "Child%sR", argv[1]);

    // Read the parent program's message from Child_W
    int fdr = open(myreadfifo, O_RDONLY);
    char readbuf[80];
    int read_bytes = read(fdr, readbuf, sizeof(readbuf));
    close(fdr);
    readbuf[read_bytes] = '\0';
    printf("In Child Process: %s\n", readbuf);

    // Send a message to parent program via Child_R 
    char writebuf[80] = "END From Child process";
    int fdw = open(mywritefifo, O_WRONLY);
    write(fdw, writebuf, sizeof(writebuf));
    close(fdw);

    // printf("In Child process %s Before: %s\n", argv[1], writebuf);
    // printf("In Child process %s After: %s\n", argv[1], writebuf);
    // char buff[10] = "Hello";
    // write(fdw, buff, sizeof(buff));
    return 0;
}