
//
// Created by gihanchanaka on 11/14/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <zconf.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

#define BUFFER_LENGTH 100
#define READ_END 0
#define WRITE_END 1


void parentProcess(int* readPipe, int* writePipe){
    char* readBuffer=(char*)malloc(sizeof(char)*(BUFFER_LENGTH+1));
    printf("PARENT: Enter text to be sent to child processes: ");
    fgets(readBuffer,BUFFER_LENGTH,stdin);
    /*int readLen=*/strlen(readBuffer);

    printf("PARENT: Read from user: %s\n",readBuffer);

    close(writePipe[READ_END]);
    write(writePipe[WRITE_END],readBuffer,strlen(readBuffer));
    close(readPipe[WRITE_END]);
    read(readPipe[READ_END],readBuffer,BUFFER_LENGTH);

    printf("PARENT: Read from child: %s\n",readBuffer);
    free(readBuffer);
}


void childProcess(int* readPipe,int* writePipe){
    close(readPipe[WRITE_END]);
    char* readBuffer=(char*)malloc(sizeof(char)*(BUFFER_LENGTH+1));
    int count = read(readPipe[READ_END], readBuffer, BUFFER_LENGTH);
    readBuffer[count] = '\0';
    printf("CHILD: Recieved from parent: %s\n",readBuffer);
    int i;
    for(i=0;i<count;i++)readBuffer[i]=(char)toupper(readBuffer[i]);
    close(writePipe[READ_END]);
    write(writePipe[WRITE_END],readBuffer,count);

    free(readBuffer);
}


int main(int argCount,char** argVector){
    int parentToChildPipe[2];
    int childToParentPipe[2];

    int pipeCreationFail=pipe(parentToChildPipe);
    if(pipeCreationFail){
        fprintf(stderr,"Error in making pipes\n");
        return -1;
    }
    pipeCreationFail=pipe(childToParentPipe);
    if(pipeCreationFail){
        fprintf(stderr,"Error in making pipes\n");
        return -1;
    }


    pid_t pid=fork();

    if(pid<0){
        fprintf(stderr,"Error in creating child process\n");
        return -1;
    }
    else if(pid==0){
        childProcess(parentToChildPipe,childToParentPipe);
    }
    else{
        parentProcess(childToParentPipe,parentToChildPipe);
    }


    printf("PARENT/CHILD: Finish running\n");

    return 0;
}


