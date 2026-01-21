#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char* argv[]){
    if(argc<4 || strcmp(argv[1], "-o")!=0){
        printf("Usage: %s -o <name> <files.c>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char* fargs[64];
    fargs[0]="gcc";
    fargs[1]="-Wall";
    int d=2;
    for(int i=3;i<argc;i++){
        char* args[5];
        args[0]="gcc";
        args[1]="-Wall";
        args[2]="-c";
        args[4]=NULL;
        char* s=strdup(argv[i]);
        s[strlen(s)-1]='o';
        fargs[d]=s;
        d++;
        int pid=(int)fork();
        if(pid==-1){
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if(pid==0){
            args[3]=argv[i];
            execvp(args[0],args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    for(int i=3;i<argc;i++){
        int status;
        wait(&status);
        if (!WIFEXITED(status) || WEXITSTATUS(status)!=0){
            printf("Error in child\n");
            exit(EXIT_FAILURE);
        }
    }
    fargs[d++]="-o";
    fargs[d++]=argv[2];
    fargs[d]=NULL;
    execvp(fargs[0],fargs);
    perror("execvp");
    exit(EXIT_FAILURE);
}