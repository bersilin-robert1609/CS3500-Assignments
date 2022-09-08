#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef MAX_BUF
#define MAX_BUF 200
#endif

struct stat st = {0};

int EXIT = 0;

int main()
{
	char* path = (char*)malloc(MAX_BUF);
	getcwd(path, MAX_BUF);
	
	FILE* fptr;
	fptr = fopen("./bash_history.txt", "w");
	
	while(!EXIT)
	{
		printf("%s> ", path);
		
		char* command = (char*)malloc(MAX_BUF);
		fgets(command, MAX_BUF, stdin);
		
		int i=0;
		
		while(command[i] != '\n') i++;
		command[i] = 0;
		
		fprintf(fptr, "%s\n", command);
		
		if(!strcmp(command, "clear")) 
			system("clear");
			
		else if(!strcmp(command, "pwd")) 
			printf("%s\n", path);
			
		else if(!strcmp(command, "mkdir"))
		{
			if(stat("./dir", &st) == -1)
				mkdir("dir", 0700);
				
			else
				printf("mkdir: cannot create directory 'dir': File exists\n");
		}
		
		else if(!strcmp(command, "rmdir")) 
			rmdir("dir");
			
		else if(!strcmp(command, "ls")) 
			system("ls");
			
		else if(!strcmp(command, "ls -l")) 
			system("ls -l");
			
		else if(!strcmp(command, "history")) 
		{
			fclose(fptr);
			
			fptr = fopen("./bash_history.txt", "r");
			while(!fptr) fopen("./bash_history.txt", "r");
			
			char* content = (char*)malloc(MAX_BUF);
			int index = 1;
			
			while(fgets(content, MAX_BUF, fptr))
			{
				printf(" %d %s", index, content);
				index++;
			} 
			
			fclose(fptr);
			
			fptr = fopen("./bash_history.txt", "a");
		}
		
		else if(!strcmp(command, "exit"))
		{
			EXIT = 1;
			if(fptr) fclose(fptr);
		}
		
		else
			printf("%s, command not found\n", command);
	}
	return 0;
}
