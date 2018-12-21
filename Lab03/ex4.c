#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#define TRUE 1
#define PORT 32000
void* handle_client(void*);

int main()
{
	int listenfd;
	int* connfd;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t clilen;

	listenfd=socket(AF_INET,SOCK_STREAM,0);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(PORT);

	bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

	listen(listenfd,5);

	clilen = sizeof(cliaddr);
	while (TRUE)
	{
		connfd = malloc(sizeof(int));
		*connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);

		pthread_t clienthandler;
		if (pthread_create(&clienthandler, NULL, handle_client, connfd)) {
			printf("ERROR: Failed to create thread.");
		}
		
	}
}

void* handle_client(void* connfd)
{
	int socket = *((int *)connfd);	
	char buffer[1000];
   	read(socket , buffer, 1000); 
   	printf("CLIENT:%s\n",buffer);
	strcpy(buffer,"Hello from the server!!!\n");
    send(socket , buffer , strlen(buffer) , 0 ); 
    close(socket);
	free(connfd);
	return NULL;
}
