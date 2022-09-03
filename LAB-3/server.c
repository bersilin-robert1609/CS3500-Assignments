#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 8192

typedef struct message {
    long int mtype;
    char mtext[BUFSIZE];
} message;

int main()
{
    int C2S = msgget((key_t) 69695, 0666 | IPC_CREAT);
    int S2C = msgget((key_t) 96965, 0666 | IPC_CREAT);
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

        if(strncmp(msg.mtext, "End", 3) == 0) break;

        char* resultBuf = (char*)malloc(BUFSIZE);
        FILE* fp = popen(msg.mtext, "r");

        if(fp == NULL)
        {
            perror("Error in popen");
            return 1;
        }
        char* resultMainBuffer = (char*)malloc(BUFSIZE);

        while(fgets(resultBuf, BUFSIZE, fp) != NULL)
            strncat(resultMainBuffer, resultBuf, BUFSIZE);

        message result;
        result.mtype = 1;
        strncpy(result.mtext, resultMainBuffer, BUFSIZE);

        if(msgsnd(S2C, (void*)&result, BUFSIZE, 0) == -1)
        {
            perror("Error in sending message to C2S queue");
            return 1;
        }

        pclose(fp);
    }
    msgctl(C2S, IPC_RMID, NULL);
    msgctl(S2C, IPC_RMID, NULL);
    return 0;
}