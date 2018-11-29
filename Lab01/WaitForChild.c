#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
	int pid,status;
	pid = fork();
	wait(&status);
	if (pid < 0){
		perror("fork");
		exit(1);
	}
	if (pid == 0) puts("This is the child process");
	else puts("This is the parent process");
	

	return 0;
}
