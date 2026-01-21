#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char* argv[]){
    if(argc!=1){
        printf("Usage: %s\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    for(; ;){
        fprintf(stdout,"shell> ");
        char buf[1024];
        char* command=fgets(buf,sizeof(buf),stdin);
        if(command==NULL || !strcmp(command,"exit\n")){
            break;
        }
        if (!strcmp(command, "\n")) {
            continue;
        }
        command[strlen(command)-1]='\0';
        char* args[24];
        int i=0;
        args[i]=strtok(command," ");
        while(args[i]!=NULL){
            i++;
            args[i]=strtok(NULL," ");
        }
        pid_t pid=fork();
        if(pid==-1){
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if(pid==0){
            execvp(args[0],args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else{
            waitpid(pid,NULL,0);
        }
    }
    exit(EXIT_SUCCESS);
}