#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
void handler1(){
    printf("Recebi SIGUSR1\n");
}
void handler2(){
    printf("A terminar...\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){
    printf("PID = %d\n",getpid());
    fflush(stdout);
    if(signal(SIGUSR1,handler1)==SIG_ERR){
        fprintf(stderr,"Can't catch SIGUSR1: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if(signal(SIGINT,handler2)==SIG_ERR){
        fprintf(stderr,"Can't catch SIGINT: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    for(;;){
        pause();
    }
}