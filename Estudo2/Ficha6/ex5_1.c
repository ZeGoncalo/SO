#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
int main(int argc, char* argv[]){
    if(argc!=1){
        printf("Usage: %s\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    if(mkfifo("/tmp/fifo1",0666)==-1 && errno!=EEXIST){
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    int f=open("/tmp/fifo1", O_WRONLY);
    if(f==-1){
        perror("open");
        exit(EXIT_FAILURE);
    }
    write(f,"Ola FIFO\n",10);
    close(f);
    exit(EXIT_SUCCESS);
}