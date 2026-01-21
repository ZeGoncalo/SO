#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char* argv[]){
    if(argc<2){
        printf("Usage: %s file1 file2 ... filen\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    int fd;
    for(int i=1;i<argc;i++){
        if((fd=open(argv[i], O_RDONLY))==-1){
            perror("open");
            exit(EXIT_FAILURE);
        }
        int n;
        char buf[1024];
        while((n=read(fd,buf,sizeof(buf)))>0){
            write(STDOUT_FILENO,buf,n);
        }
        close(fd);
    }
    exit(EXIT_SUCCESS);
}