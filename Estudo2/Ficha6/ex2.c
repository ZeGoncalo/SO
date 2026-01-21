#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main(int argc, char* argv[]){
    int pid=(int)fork();
    if(pid==-1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    int f;
    if(pid==0){
        if((f=open("out.txt", O_CREAT | O_WRONLY | O_TRUNC , 0644))==-1){
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(f, STDOUT_FILENO);
        close(f);
        execlp("ls","ls","-l",NULL);
        perror("execlp");
    exit(EXIT_FAILURE);
    }
    if(pid>0){
        wait(NULL);
        printf("feito\n");
    }
    exit(EXIT_SUCCESS);
}