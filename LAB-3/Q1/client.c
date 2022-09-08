#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

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
        printf("Type a command: ");
        fgets(msg.mtext, BUFSIZE, stdin);
        msg.mtype = 1;
        if(msgsnd(C2S, (void*)&msg, BUFSIZE, 0) == -1)
        {
            perror("Error in sending message to C2S queue");
            return 1;
        }

        result rslt;
        if(msgrcv(S2C, (void*)&rslt, BUFSIZE, 1, 0) == -1)
        {
            perror("Error in receiving message from S2C queue");
            return 1;
        }
        else printf("%ld\n", rslt.mtext);

        if(strncmp(msg.mtext, "End", 3) == 0) break;
    }

    msgctl(C2S, IPC_RMID, NULL);
    msgctl(S2C, IPC_RMID, NULL);
    return 0;
}