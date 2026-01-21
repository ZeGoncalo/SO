#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char* argv[]){
    if(argc!=1){
        printf("Usage: %s\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    int f=open("/tmp/fifo1", O_RDONLY);
    if(f==-1){
        perror("open");
        exit(EXIT_FAILURE);
    }
    char buf[1024];
    int n;
    while((n=read(f,buf,sizeof(buf)))>0) {
        write(STDOUT_FILENO,buf,n);
    }
    close(f);
    unlink("/tmp/fifo1");
    exit(EXIT_SUCCESS);
}