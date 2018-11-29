#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
	int pid;
	
	for (int i=0; i<3; i++){
	
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			exit(1);
		}
		if (pid == 0)
			puts("This is the child process");
		else
			puts("This is the parent process");
	}
	return 0;
}
