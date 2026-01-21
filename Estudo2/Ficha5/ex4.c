#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char* argv[]){
    if(argc!=2){
        printf("Usage: %s command\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    pid_t pid=fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(pid==0){
        if(execlp(argv[1], argv[1],NULL)==-1){
            perror("execlp");
            exit(EXIT_FAILURE);
        }
    }
    else{
        int status;
        waitpid(pid, &status, 0);
        printf("Pai: filho terminou.\n");
    }
    exit(EXIT_SUCCESS);
}