#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char* argv[]){
    if(argc!=2){
        printf("Usage: %s filename\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    int fd[2];
    if(pipe(fd)==-1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    int pid=fork();
    if (pid==-1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(pid>0){
        close(fd[0]);
        int f=open(argv[1],O_RDONLY);
        if(f==-1){
            perror("open");
            exit(EXIT_FAILURE);
        }
        char buf[1024];
        int n;
        while((n=(int)read(f,buf,sizeof(buf)))>0){
            write(fd[1], buf,n);
        }
        close(fd[1]);
        close(f);
        wait(NULL);
    }
    else{
        close(fd[1]);
        char buf[1024];
        int n;
        while((n=(int)read(fd[0],buf,sizeof(buf)))>0){
            write(STDOUT_FILENO, buf,n);
        }
        close(fd[0]);
    }
    exit(EXIT_SUCCESS);
}