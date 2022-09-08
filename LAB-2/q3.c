#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUF 200

int DEBUG = 0;

int main(int argc, char* argv[])
{
	int pipefd[2];
	pid_t cpid;
	char buf;
	
	if(argc != 2) 
	{
		fprintf(stderr, "Usage: %s <String>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	if(pipe(pipefd) == -1) 
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	
	cpid = fork();
	if(cpid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	
	if(cpid == 0) 
	{
		close(pipefd[0]);
		write(pipefd[1], argv[1], strlen(argv[1]));
		if(DEBUG) printf("Sent from Child Process\n");
		close(pipefd[1]);
		_exit(EXIT_SUCCESS);					
	}
	else
	{
		close(pipefd[1]);
		
		while(read(pipefd[0], &buf, 1) > 0)
		{
			if(DEBUG) printf("Printing From Parent Process\n");
			write(STDOUT_FILENO, &buf, 1);
			if(DEBUG) printf("\n");
		}
		
		write(STDOUT_FILENO, "\n", 1);
		close(pipefd[0]);
		wait(NULL);
		exit(EXIT_SUCCESS);
	}
	
}
