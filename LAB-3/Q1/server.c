#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFSIZE 8192

typedef struct message {
    long int mtype;
    char mtext[BUFSIZE];
} message;

typedef struct result {
    long int mtype;
    ssize_t mtext;
} result;

int main()
{
    int C2S = msgget((key_t) 69696, 0666 | IPC_CREAT);
    int S2C = msgget((key_t) 96966, 0666 | IPC_CREAT);
    if(C2S == -1)
    {
        perror("Error in C2S creation");
        return 1;
    }
    if(S2C == -1)
    {
        perror("Error in S2C creation");
        return 1;
    }

    while(1)
    {
        message msg;
        if(msgrcv(C2S, (void*)&msg, BUFSIZE, 1, 0) == -1)
        {
            perror("Error in receiving message from S2C queue");
            return 1;
        }
        else printf("Message Received: %s", msg.mtext);

        ssize_t result_to_send;

        if(strncmp(msg.mtext, "End", 3) == 0) 
        {
            
            break;
        }
        else if(strncmp(msg.mtext, "open", 4) == 0)
        {
            printf("Open command received\n");
            char* filename = (char*)malloc(BUFSIZE);
            strcpy(filename, "created_file.txt");
            int fileDescriptor = open(filename, O_CREAT | O_EXCL | O_RDWR, 0666);
            char* buffer = (char*)malloc(BUFSIZE);
            strcpy(buffer, "This is a test file");
            ssize_t len = write(fileDescriptor, buffer, strlen(buffer));
            printf("File created and number of bytes written: %ld\n", len);
            result_to_send = fileDescriptor;
            close(fileDescriptor);
        }
        else if(strncmp(msg.mtext, "read", 4) == 0)
        {
            printf("Read command received\n");
            char* filename = (char*)malloc(BUFSIZE);
            strcpy(filename, "created_file.txt");
            int fileDescriptor = open(filename, O_RDONLY);
            char* buffer = (char*)malloc(BUFSIZE);
            result_to_send = read(fileDescriptor, buffer, BUFSIZE);
            close(fileDescriptor);
        }
        else if(strncmp(msg.mtext, "access", 6) == 0)
        {
            printf("Access command received\n");
            char* filename = (char*)malloc(BUFSIZE);
            strcpy(filename, "created_file.txt");
            result_to_send = access(filename, F_OK);
        }
        else if(strncmp(msg.mtext, "mkdir", 5) == 0)
        {
            printf("Mkdir command received\n");
            char* dirname = (char*)malloc(BUFSIZE);
            strcpy(dirname, "created_dir");
            result_to_send = mkdir(dirname, 0777);
        }
        else if(strncmp(msg.mtext, "rmdir", 5) == 0)
        {
            printf("Rmdir command received\n");
            char* dirname = (char*)malloc(BUFSIZE);
            strcpy(dirname, "created_dir");
            result_to_send = rmdir(dirname);
        }

        result rslt;
        rslt.mtype = 1;
        rslt.mtext = result_to_send;
        printf("result_to_send: %ld\n", result_to_send);

        if(msgsnd(S2C, (void*)&rslt, BUFSIZE, 0) == -1)
        {
            perror("Error in sending message to C2S queue");
            return 1;
        }
    }
    msgctl(C2S, IPC_RMID, NULL);
    msgctl(S2C, IPC_RMID, NULL);
    return 0;
}