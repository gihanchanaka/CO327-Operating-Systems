//
// Created by gihanchanaka on 11/13/18.
//

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <zconf.h>
#include <string.h>

#define BUFFER_LENGTH 100

int main(int argCount, char** argVec){
    if(argCount!=3){
        printf("USAGE: %s /sourceFileDir/sourceFileName.txt /destFileDir/destFileName.txt\n",argVec[0]);
        return -1;
    }
    char* sourceFileName=argVec[1];
    char* destFileName=argVec[2];

    int sourceFile=open(sourceFileName,O_RDONLY);

    if(sourceFile==-1){
        printf("ERROR: Source file %s cannot be opened\n",sourceFileName);
        return -1;
    }

    int destFile=open(destFileName,O_WRONLY  | O_CREAT , S_IRUSR | S_IWUSR );

    if(destFile==-1){
        printf("ERROR: Destination file %s cannot be opened\n",destFileName);
        return -1;
    }

    char* buffer=(char*)malloc(sizeof(char)*(BUFFER_LENGTH+1));

    int readLength=read(sourceFile,buffer,BUFFER_LENGTH);
    int writeLength;

    if(readLength==-1){
        printf("ERROR: Error while reading file\n");
        return -1;
    }

    while(readLength > 0){
        buffer[readLength] = '\0';
        writeLength=write(destFile,buffer,readLength);
        if(writeLength!=readLength){
            printf("ERROR: Could not write to file properly\n");
            return -1;
        }

        //printf("DEBUG: Wrote %d =%d characters to file.",strlen(buffer),readLength);
        readLength=read(sourceFile,buffer,BUFFER_LENGTH);
    }

    close(sourceFile);
    close(destFile);



    free(buffer);
    //printf("SUCCESS: Program completed\n");
    return 0;
}

