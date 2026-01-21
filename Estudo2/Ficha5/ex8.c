#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char* argv[]){
    if(argc<2){
        printf("%s int1 int2 ... int n\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    for(int i=1;i<argc;i++){
        int pid=(int)fork();
        if(pid==-1){
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if(pid==0){
            if(atoi(argv[i])%2==0){
                printf("PID=%d : %d é par\n",(int)getpid(),atoi(argv[i]));
                exit(EXIT_SUCCESS);
            }
            else{
                printf("PID=%d : %d é ímpar\n",(int)getpid(),atoi(argv[i]));
                exit(EXIT_SUCCESS);
            }
        }
    }
    for (int i = 1; i < argc; i++) {
        wait(NULL);
    }
    exit(EXIT_SUCCESS);
}