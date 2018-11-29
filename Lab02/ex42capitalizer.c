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
    int fd,fdd;
    char * fifoSimple = "/tmp/fifoSimple";
    char * fifoCapital = "/tmp/fifoCapital";

    mkfifo(fifoSimple,0666);
    mkfifo(fifoCapital,0666);

    fd = open(fifoSimple, O_RDONLY);
    dup2(fd,fileno(stdin));
    fdd=open(fifoCapital,O_WRONLY);
    dup2(fdd,fileno(stdout));

    char *tr_args[4];
    tr_args[0]="tr";
    tr_args[1]="[:lower:]";
    tr_args[2]="[:upper:]";
    tr_args[3]=NULL;


    execvp(tr_args[0],tr_args);

    return 0;
}

