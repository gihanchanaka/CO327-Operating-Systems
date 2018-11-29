#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LIMIT 1000
#define TRUE 1
#define CHILD_PID 0
#define FORK_ERROR -1

int main(char argc, char **argv)
{
	char command[MAX_LIMIT];
	int childStatus;
	
	while(TRUE){
		scanf("%[^\n]%*c",command);

//		printf("The input is: %s\n",command);
		int noArguments=0;

		char* temp=(char*)malloc(sizeof(char)*(strlen(command)+1));
		strcpy(temp,command);
		char* arg=strtok(temp," ");
		while(arg){
			noArguments++;
			arg=strtok(NULL," ");

		}
//		printf("The input is: %s\n",command);


		
		char** args=(char**)malloc((noArguments+1)*sizeof(char *));
		
		strcpy(temp,command);
		arg=strtok(temp," ");
		int i=0;
		while(arg){
			//printf("Incomming arg= %s -- %s.\n",arg,command);
			args[i]=(char*)malloc(sizeof(char)*(strlen(arg)+1));
			//printf("arg counting: %s\n",arg );
			strcpy(args[i],arg);
			//printf("arg counting: %s %s\n",args[i],arg );
			i++;
			arg=strtok(NULL," ");
		}
		args[noArguments]=NULL;

		// for(i=0;i<noArguments;i++){
		// 	printf("arg[%d] = %s\n",i,args[i]);
		// }



		
		int pid=fork();

		
		if(pid==FORK_ERROR){
			printf("Error!\n");
			break;
		}
		else if(pid==CHILD_PID){
			execvp(args[0],args);
			printf("Child finished running command: %s\n",command);
			for(i=0;i<noArguments;i++)free(args[i]);
			free(args);
			break;
		}
		else{//pid==PARENT
			printf("Parent waiting\n");
			wait(&childStatus);
			printf("Parent finished waiting for child to finish command: %s\n and child exit state: %d.\n",command,childStatus);
		}



		


	}
}