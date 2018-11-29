#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define TRUE 1

void handle_client(int newsockfd, struct sockaddr_in cli_addr, int clilen){

	// Echo server function
	char recieveMessage[1000];

	char * hi = "I am the server process handling you\n";

	sendto(newsockfd, hi, strlen(hi), 0, (struct sockaddr*) &cli_addr, clilen);

	int recieveLength = recvfrom(newsockfd, recieveMessage, 1000, 0, (struct sockaddr*)&cli_addr, &clilen);

	recieveMessage[recieveLength] = '\0';
	printf("Recieved: %s\n", recieveMessage);
	char * thanks = "Thank you for the message:\n";

	sendto(newsockfd, thanks, strlen(thanks), 0, (struct sockaddr*) &cli_addr, clilen);
	sendto(newsockfd, recieveMessage, recieveLength, 0, (struct sockaddr*) &cli_addr, clilen);

}


int main(int argc,char **argv){
	int sockfd,n,newsockfd,pid,clilen;
	struct sockaddr_in serverAddress;
	struct sockaddr_in cli_addr;
	socklen_t length;
	char message[1000];
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddress.sin_port=htons(12345);
	bind(sockfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress));	
	listen(sockfd,5);
	

	while (TRUE){
		/* New socket descriptor is returned each time a client connects*/
		clilen = sizeof(cli_addr); 
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

		if (newsockfd < 0){
			perror("ERROR on accept");
			exit(1);
		}
		pid = fork();
		
		if (pid < 0){
			perror("ERROR on fork");
			exit(1);
		}
		if (pid == 0){
		/* In child process which the handles client connection */
			close(sockfd);
			handle_client(newsockfd,cli_addr,clilen);
			exit(0);
		}
		else{
			/* In parent process which continues to listen for new clients */
			close(newsockfd);
			wait(NULL);
		}
	}
} 