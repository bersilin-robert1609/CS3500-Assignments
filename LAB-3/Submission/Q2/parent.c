#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>

#define REQUEST (long)100
#define PIVOT (long)200
#define LARGE (long)300
#define SMALL (long)400
#define READY (long)500
#define EXIT (long)600
#define BUFSIZE 200

void callError(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int valid(pid_t *cpid)
{
    int zeros = 0;
    if (cpid[0] == 0) zeros++;
    if (cpid[1] == 0) zeros++;
    if (cpid[2] == 0) zeros++;
    if (cpid[3] == 0) zeros++;
    if (cpid[4] == 0) zeros++;

    if(zeros > 1) return 0;
    else return 1;
}

int main()
{
    char* readfifo[5] = {"readfifo1", "readfifo2", "readfifo3", "readfifo4", "readfifo5"};
    char* writefifo[5] = {"writefifo1", "writefifo2", "writefifo3", "writefifo4", "writefifo5"};

    for(int i=0; i<5; i++)
    {
        mkfifo(readfifo[i], 0666);
        mkfifo(writefifo[i], 0666);
    }

    pid_t *cpid;
    cpid = (pid_t*)malloc(5 * sizeof(pid_t));
    for(int i=0; i<5; i++) 
    {
        cpid[i] = fork();
        if(cpid[i] == -1) callError("fork");
    }

    if(!valid(cpid)) _exit(EXIT_SUCCESS);
    else
    {
        if(cpid[0] == 0)
        {
            // child 1
            printf("Child 1 created with pid %d\n", getpid());

            char* binaryPath = "./child";
            char* ch_args[] = {binaryPath, "1", NULL};
            execv(binaryPath, ch_args);
        }
        else if(cpid[1] == 0)
        {
            // child 2
            printf("Child 2 created with pid %d\n", getpid());

            char* binaryPath = "./child";
            char* ch_args[] = {binaryPath, "2", NULL};
            execv(binaryPath, ch_args);
        }
        else if(cpid[2] == 0)
        {
            // child 3
            printf("Child 3 created with pid %d\n", getpid());

            char* binaryPath = "./child";
            char* ch_args[] = {binaryPath, "3", NULL};
            execv(binaryPath, ch_args);
        }
        else if(cpid[3] == 0)
        {
            // child 4
            printf("Child 4 created with pid %d\n", getpid());

            char* binaryPath = "./child";
            char* ch_args[] = {binaryPath, "4", NULL};
            execv(binaryPath, ch_args);
        }
        else if(cpid[4] == 0)
        {
            // child 5
            printf("Child 5 created with pid %d\n", getpid());

            char* binaryPath = "./child";
            char* ch_args[] = {binaryPath, "5", NULL};
            execv(binaryPath, ch_args);
        }
        else
        {
            printf("Parent, pid: %d\n", getpid());
            for(int i=0; i<5; i++)
            {
                int writefd = open(writefifo[i], O_CREAT | O_WRONLY);
                long childId = i+1; //change here to send various types of child ids
                if(write(writefd, &childId, sizeof(long)) != sizeof(long)) callError("write");
                else printf("Child id %ld sent to child %d\n", childId, i);
            }

            for(int i=0; i<5; i++)
            {
                int readfd = open(readfifo[i], O_RDONLY);
                long ready;
                if(read(readfd, &ready, sizeof(long)) == -1) callError("read");
                else if(ready == READY) printf("Child %d ready\n", i);
                else callError("Child not ready");
            }

            long k = 25/2;
            int found = 0;
            long median;
            long pivot;

            srand(time(0));
            while(!found)
            {
                int randomChild = rand() % 5;
                printf("Random child chosen by parent: %d\n", randomChild);
                long command = REQUEST;
                int writefd = open(writefifo[randomChild], O_WRONLY);
                int readfd = open(readfifo[randomChild], O_RDONLY);

                if(write(writefd, &command, sizeof(long)) != sizeof(long)) callError("write");
                else printf("REQUEST sent to child %d\n", randomChild);

                if(read(readfd, &pivot, sizeof(long)) == -1) callError("read");
                else printf("Pivot %ld received from child %d\n", pivot, randomChild);

                if(pivot == -1) continue;

                printf("Pivot %ld selected\n", pivot);
                long m = 0;

                printf("Parent broadcasts PIVOT %ld to all children\n", pivot);
                long temp[5];
                for(int i=0; i<5; i++)
                {
                    int writefd = open(writefifo[i], O_WRONLY);
                    int readfd = open(readfifo[i], O_RDONLY);
                    long command = PIVOT;
                    if(write(writefd, &command, sizeof(long)) != sizeof(long)) callError("write");
                    if(write(writefd, &pivot, sizeof(long)) != sizeof(long)) callError("write");

                    if(read(readfd, &temp[i], sizeof(long)) == -1) callError("read");
                    else printf("Parent received %ld from child %d\n", temp[i], i);

                    m += temp[i];
                }
                printf("Parent: m=%ld+%ld+%ld+%ld+%ld=%ld\n", temp[0], temp[1], temp[2], temp[3], temp[4], m);
                printf("m: %ld, k: %ld\n", m, k);

                if(m == k)
                {
                    found = 1;
                    median = pivot;
                    printf("%ld == %ld, Median found: %ld\n", m, k, median);

                    printf("Parent broadcasts TERMINATE to all children\n");
                    for(int i=0; i<5; i++)
                    {
                        int writefd = open(writefifo[i], O_WRONLY);
                        long exit = EXIT;
                        if(write(writefd, &exit, sizeof(long)) != sizeof(long)) callError("write");
                    }
                }
                else if(m > k)
                {
                    printf("%ld > %ld, SMALL sent to all children\n", m, k);
                    for(int i=0; i<5; i++)
                    {
                        int writefd = open(writefifo[i], O_WRONLY);
                        long command = SMALL;
                        if(write(writefd, &command, sizeof(long)) != sizeof(long)) callError("write");
                    }
                }
                else if(m < k)
                {
                    printf("%ld < %ld, LARGE sent to all children\n", m, k);
                    for(int i=0; i<5; i++)
                    {
                        int writefd = open(writefifo[i], O_WRONLY);
                        long command = LARGE;
                        if(write(writefd, &command, sizeof(long)) != sizeof(long)) callError("write");
                    }
                    k -= m;
                }
            }

            wait(NULL);
            printf("Parent, all children terminated\n");
            exit(EXIT_SUCCESS);
        }
    }
}