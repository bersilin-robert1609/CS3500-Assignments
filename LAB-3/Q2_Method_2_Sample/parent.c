#define REQUEST 100
#define PIVOT 200
#define LARGE 300
#define SMALL 400
#define READY 500

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main() {
    char *writefifo = "Child_W";    // to send some message to the child 
    char *readfifo = "Child_R";     // to receive some message from the child

    // We can use either mknod or mkfifo.
    // mknod("Child_W", 0666, 0);
    // mknod("Child_R", 0666, 0);

    // As these fifos are named fifos, we can use them in the child program as well.
    mkfifo(writefifo, 0666);
    mkfifo(readfifo, 0666);
    
    // fork and exec of the child process
    pid_t childpid = fork(); 
    if(childpid == 0) {
        char *binaryPath = "./child";
        char *ch_args[] = {binaryPath, NULL};
        execv(binaryPath, ch_args);
    }
    
    // Send the message to child - write into writefifo
    int fdw = open(writefifo, O_CREAT|O_WRONLY);
    char writebuf[80];
    sprintf(writebuf, "Hithere!%d From Parent Process", 1);
    // printf("In parent process: %s\n", writebuf);
    write(fdw, writebuf, sizeof(writebuf));
    close(fdw);

    // Receive the message from child - To read from readfifo
    int fd = open(readfifo, O_RDONLY);
    char readbuf[80];
    int read_bytes = read(fd, readbuf, sizeof(readbuf));
    close(fd);
    readbuf[read_bytes] = '\0';
    printf("In Parent Process: %d, %s\n", read_bytes, readbuf);
    
    // if(strcmp(readbuf, "END") == 0) printf("In Parent Process: Hi\n");
}