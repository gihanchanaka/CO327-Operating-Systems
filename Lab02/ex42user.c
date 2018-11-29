//
// Created by gihanchanaka on 11/21/18.
//

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main()
{
    char buffer[100];
    printf("Enter the text:");
    fgets(buffer,100,stdin);

    int fd,fdd;
    char * fifoSimple = "/tmp/fifoSimple";
    char * fifoCapital = "/tmp/fifoCapital";

    mkfifo(fifoSimple,0666);
    mkfifo(fifoCapital,0666);

    fd = open(fifoSimple, O_WRONLY);
    write(fd, buffer,strlen(buffer));
    close(fd);



    fdd = open(fifoCapital, O_RDONLY);
    read(fdd, buffer, 100);
    printf("Recieved: %s\n", buffer);
    close(fd);
    unlink(fifoSimple);
    unlink(fifoCapital);

    return 0;
}

