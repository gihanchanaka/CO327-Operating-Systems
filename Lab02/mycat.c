//
// Created by gihanchanaka on 11/13/18.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFER_LENGTH 100

int main(int argCount, char** argVec) {
    if(argCount!=2){
        printf("USAGE: %s /fileDir/fileName.txt\n",argVec[0]);
        return 0;
    }

    char* fileName=argVec[1];

    int file=open(fileName,O_RDONLY);//TO READ: What is the use of modes?
    if(file==-1){
        printf("ERROR: Couldn't open file\n");
        return 0;
    } else{
        //printf("SUCCESS: Opened file\n");
    }

    char* buffer=(char*)malloc(sizeof(char)*(BUFFER_LENGTH+1));
    int readLength=read(file,buffer,BUFFER_LENGTH);


    if(readLength==-1){
        printf("ERROR: Error while reading file\n");
        return 0;
    }
    while(readLength>0) {
        buffer[readLength] = '\0';
        printf("%s", buffer);
        readLength=read(file,buffer,BUFFER_LENGTH);
    }
    printf("\n");

    close(file);
    free(buffer);

    //printf("SUCCESS: Program complete\n");
    return 0;
}