#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

FILE* preProcessChild(int childNo, int readfd) 
{
    long id;
    if(read(readfd, &id, sizeof(long)) == -1) 
        callError("Error in reading id");

    char* fileName = (char*)malloc(BUFSIZE*sizeof(char));
    sprintf(fileName, "data_%ld.txt", id);

    FILE *fp = fopen(fileName, "r");
    if(fp == NULL) callError("Error in opening file");
    return fp;
}

int* readFile(FILE *fp)
{
    int *arr = (int*)malloc(5 * sizeof(int));
    for(int i = 0; i < 5; i++) {
        if(fscanf(fp, "%d", &arr[i]) == EOF) callError("File Not Proper");
    }
    return arr;
}

void processChild(int* arr, int childNo, int readfd, int writefd)
{
    long ready = READY;
    if(write(writefd, &ready, sizeof(long)) == -1) 
        callError("Error in writing ready");
    else printf("Child %d sent READY\n", childNo);

    long size = 5;
    long pivotC;

    srand(time(NULL));
    while(1)
    {
        long command;
        if(read(readfd, &command, sizeof(long)) == -1) 
            callError("Error in reading command");

        if(command == EXIT) break;
        else if(command == REQUEST)
        {
            if(size == 0) 
            {
                long minusOne = -1;
                if(write(writefd, &minusOne, sizeof(long)) == -1) 
                    callError("Error in writing -1");
            }
            else
            {
                int randomIndex = rand() % size;
                long randomElement = arr[randomIndex];
                if(write(writefd, &randomElement, sizeof(long)) == -1) 
                    callError("Error in writing random element");
                else printf("Child %d sent random element %ld\n", childNo, randomElement);
            }
        }
        else if(command == PIVOT)
        {
            if(read(readfd, &pivotC, sizeof(long)) == -1) 
                callError("Error in reading pivot");

            long greaterThanPivot = 0;
            for(int i = 0; i < size; i++) {
                if(arr[i] > pivotC) greaterThanPivot++;
            }

            if(write(writefd, &greaterThanPivot, sizeof(long)) == -1) 
                callError("Error in writing greater than pivot");
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
            printf("Child %d: New array size is %d, Array is ", childNo, greaterThanPivotSize);
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
            printf("Child %d: New array size is %d, Array is ", childNo, smallerThanPivotSize);
            for(int i = 0; i < size; i++) printf("%d ", arr[i]);
            printf("\n");
        }
    }
    return;
}

int main(int argc, char* argv[])
{
    if(argc != 2) callError("Invalid number of arguments");
    int childNo = atoi(argv[1]);
    char* readfifo = (char*)malloc(BUFSIZE);
    sprintf(readfifo, "writefifo%d", childNo);
    char* writefifo = (char*)malloc(BUFSIZE);
    sprintf(writefifo, "readfifo%d", childNo);

    int readfd = open(readfifo, O_RDONLY);
    if(readfd == -1) callError("Error in opening read fifo");
    int writefd = open(writefifo, O_WRONLY);
    if(writefd == -1) callError("Error in opening write fifo");

    FILE *fp = preProcessChild(childNo, readfd);
    int* arr = readFile(fp);
    processChild(arr, childNo, readfd, writefd);
    printf("Child %d terminates\n", childNo);
    _exit(EXIT_SUCCESS);
}