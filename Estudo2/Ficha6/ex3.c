#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
int main(int argc, char* argv[]){
    if(argc!=3){
        printf("Usage: %s cmd1 cmd2\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    int fd[2];
    if(pipe(fd)==-1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    int pid=(int)fork();
    if(pid==-1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    char* args1[64];
    int i=0;
    char* cmd1=strdup(argv[1]);
    args1[i]=strtok(cmd1," ");
    while(args1[i]!=NULL){
        i++;
        args1[i]=strtok(NULL," ");
    }
    if(pid==0){
        dup2(fd[1],STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(args1[0], args1);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    pid=(int)fork();
    if(pid==-1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    char* args2[64];
    i=0;
    char* cmd2=strdup(argv[2]);
    args2[i]=strtok(cmd2," ");
    while(args2[i]!=NULL){
        i++;
        args2[i]=strtok(NULL," ");
    }
    if(pid==0){
        dup2(fd[0],STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(args2[0], args2);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
}