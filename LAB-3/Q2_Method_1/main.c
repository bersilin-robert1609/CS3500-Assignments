#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>

#define REQUEST (long long)100
#define PIVOT (long long)200
#define LARGE (long long)300
#define SMALL (long long)400
#define READY (long long)500
#define EXIT (long long)600
#define BUFSIZE 200

void callError(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

FILE* preProcessChild(int childNo, int pipefd[5][2][2]) 
{
    close(pipefd[childNo][0][1]);
    close(pipefd[childNo][1][0]);
    long long id;
    if(read(pipefd[childNo][0][0], &id, sizeof(long long)) == -1) 
        callError("Error in reading id");
    else /*printf("Child %d received id %lld\n", childNo, id)*/;

    //printf("Child %d: Preprocessing data...\n", childNo);
    char* fileName = (char*)malloc(BUFSIZE*sizeof(char));
    sprintf(fileName, "data_%lld.txt", id);

    FILE *fp = fopen(fileName, "r");
    if(fp == NULL) callError("Error in opening file");
    //printf("Child %d: Preprocessing done, File opened %s\n", childNo, fileName);
    return fp;
}

int* readFile(FILE *fp)
{
    int *arr = (int*)malloc(5 * sizeof(int));
    for(int i = 0; i < 5; i++) {
        if(fscanf(fp, "%d", &arr[i]) == EOF) callError("File Not Proper");
    }
    //for(int i=0; i<5; i++) printf("%d ", arr[i]);
    //printf("\n");
    return arr;
}

void processChild(int* arr, int childNo, int pipefd[5][2][2])
{
    long long ready = READY;
    if(write(pipefd[childNo][1][1], &ready, sizeof(long long)) == -1) 
        callError("Error in writing ready");
    else /*printf("Child %d sent ready\n", childNo)*/;

    long long size = 5;
    long long pivotC;

    srand(time(NULL));
    while(1)
    {
        long long command;
        if(read(pipefd[childNo][0][0], &command, sizeof(long long)) == -1) 
            callError("Error in reading command");
        else /*printf("Child %d received command %lld\n", childNo, command)*/;

        if(command == EXIT) break;
        else if(command == REQUEST)
        {
            if(size == 0) 
            {
                long long minusOne = -1;
                if(write(pipefd[childNo][1][1], &minusOne, sizeof(long long)) == -1) 
                    callError("Error in writing -1");
            }
            else
            {
                int randomIndex = rand() % size;
                long long randomElement = arr[randomIndex];
                if(write(pipefd[childNo][1][1], &randomElement, sizeof(long long)) == -1) 
                    callError("Error in writing random element");
                else printf("Child %d sent random element %lld\n", childNo, randomElement);
            }
        }
        else if(command == PIVOT)
        {
            if(read(pipefd[childNo][0][0], &pivotC, sizeof(long long)) == -1) 
                callError("Error in reading pivot");
            else printf("Child %d received pivot %lld\n", childNo, pivotC);

            long long greaterThanPivot = 0;
            for(int i = 0; i < size; i++) {
                if(arr[i] > pivotC) greaterThanPivot++;
            }

            if(write(pipefd[childNo][1][1], &greaterThanPivot, sizeof(long long)) == -1) 
                callError("Error in writing greater than pivot");
            else printf("Child %d sent greater than pivot number: %lld\n", childNo, greaterThanPivot);
        }
        else if(command == SMALL)
        {
            int* greaterThanPivot = (int*)malloc(size * sizeof(int));
            int greaterThanPivotSize = 0;
            for(int i = 0; i < size; i++) {
                if(arr[i] >= pivotC) greaterThanPivot[greaterThanPivotSize++] = arr[i];
            }
            arr = greaterThanPivot;
            size = greaterThanPivotSize;
            printf("Child %d: New array size %d is ", childNo, greaterThanPivotSize);
            for(int i = 0; i < size; i++) printf("%d ", arr[i]);
            printf("\n");
        }
        else if(command == LARGE)
        {
            int* smallerThanPivot = (int*)malloc(size * sizeof(int));
            int smallerThanPivotSize = 0;
            for(int i = 0; i < size; i++) {
                if(arr[i] <= pivotC) smallerThanPivot[smallerThanPivotSize++] = arr[i];
            }
            arr = smallerThanPivot;
            size = smallerThanPivotSize;
            printf("Child %d: New array size %d is ", childNo, smallerThanPivotSize);
            for(int i = 0; i < size; i++) printf("%d ", arr[i]);
            printf("\n");
        }
    }
    return;
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
    int pipefd[5][2][2];

    // pipes are made from parent to child and from child to parent 
    // the 0 index in a pipefd is for reading and 1 is for writing
    // pipefd[i][0] is the pipe from parent to child i+1 and data direction is parent to child
    // pipefd[i][1] is the pipe from child i+1 to parent and data direction is child to parent

    for(int i=0; i<5; i++)
    {
        if(pipe(pipefd[i][0]) == -1) callError("pipe");
        if(pipe(pipefd[i][1]) == -1) callError("pipe");
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

            FILE *fp = preProcessChild(0, pipefd);
            int* arr = readFile(fp);

            processChild(arr, 0, pipefd);

            _exit(EXIT_SUCCESS);
        }
        else if(cpid[1] == 0)
        {
            // child 2
            printf("Child 2 created with pid %d\n", getpid());

            FILE *fp = preProcessChild(1, pipefd);
            int* arr = readFile(fp);

            processChild(arr, 1, pipefd);

            _exit(EXIT_SUCCESS);
        }
        else if(cpid[2] == 0)
        {
            // child 3
            printf("Child 3 created with pid %d\n", getpid());

            FILE *fp = preProcessChild(2, pipefd);
            int* arr = readFile(fp);

            processChild(arr, 2, pipefd);

            _exit(EXIT_SUCCESS);
        }
        else if(cpid[3] == 0)
        {
            // child 4
            printf("Child 4 created with pid %d\n", getpid());

            FILE *fp = preProcessChild(3, pipefd);
            int* arr = readFile(fp);

            processChild(arr, 3, pipefd);

            _exit(EXIT_SUCCESS);
        }
        else if(cpid[4] == 0)
        {
            // child 5
            printf("Child 5 created with pid %d\n", getpid());

            FILE *fp = preProcessChild(4, pipefd);
            int* arr = readFile(fp);

            processChild(arr, 4, pipefd);

            _exit(EXIT_SUCCESS);
        }
        else
        {
            printf("Parent, pid: %d\n", getpid());
            for(int i=0; i<5; i++)
            {
                close(pipefd[i][0][0]);
                long long childId = i+1; //change here to send various types of child ids
                if(write(pipefd[i][0][1], &childId, sizeof(long long)) != sizeof(long long)) callError("write");
                else printf("Child id %lld sent to child %d\n", childId, i);
            }

            for(int i=0; i<5; i++)
            {
                close(pipefd[i][1][1]);
                long long ready;
                if(read(pipefd[i][1][0], &ready, sizeof(long long)) == -1) callError("read");
                else if(ready == READY) printf("Child %d ready\n", i);
                else callError("Child not ready");
            }

            long long k = 25/2;
            int found = 0;
            long long median;
            long long pivot = -1;

            srand(time(0));
            while(!found)
            {
                int randomChild = rand() % 5;
                printf("Random child %d\n", randomChild);
                long long command = REQUEST;

                if(write(pipefd[randomChild][0][1], &command, sizeof(long long)) != sizeof(long long)) callError("write");
                else printf("Command %lld sent to child %d\n", command, randomChild);

                if(read(pipefd[randomChild][1][0], &pivot, sizeof(long long)) == -1) callError("read");
                else printf("Pivot %lld received from child %d\n", pivot, randomChild);

                if(pivot == -1) continue;

                printf("Pivot %lld selected\n", pivot);
                long long m = 0;

                for(int i=0; i<5; i++)
                {
                    long long command = PIVOT;
                    if(write(pipefd[i][0][1], &command, sizeof(long long)) != sizeof(long long)) callError("write");
                    else printf("Command %lld sent to child %d\n", command, i);

                    if(write(pipefd[i][0][1], &pivot, sizeof(long long)) != sizeof(long long)) callError("write");
                    else /*printf("Pivot %lld sent to child %d\n", pivot, i)*/;

                    long long temp;
                    if(read(pipefd[i][1][0], &temp, sizeof(long long)) == -1) callError("read");
                    else printf("temp %lld received from child %d\n", temp, i);

                    m += temp;
                }
                printf("m: %lld, k: %lld\n", m, k);

                if(m == k)
                {
                    found = 1;
                    median = pivot;
                    printf("Median: %lld\n", median);

                    for(int i=0; i<5; i++)
                    {
                        long long exit = EXIT;
                        if(write(pipefd[i][0][1], &exit, sizeof(long long)) != sizeof(long long)) callError("write");
                        else printf("Exit sent to child %d\n", i);
                    }
                }
                else if(m > k)
                {
                    for(int i=0; i<5; i++)
                    {
                        long long command = SMALL;
                        if(write(pipefd[i][0][1], &command, sizeof(long long)) != sizeof(long long)) callError("write");
                        else printf("Command %lld sent to child %d\n", command, i);
                    }
                }
                else if(m < k)
                {
                    for(int i=0; i<5; i++)
                    {
                        long long command = LARGE;
                        if(write(pipefd[i][0][1], &command, sizeof(long long)) != sizeof(long long)) callError("write");
                        else printf("Command %lld sent to child %d\n", command, i);
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