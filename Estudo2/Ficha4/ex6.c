#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char* argv[]){
    struct dirent* d;
    if(argc>2){
        printf("Usage: %s dir",argv[0]);
        exit(EXIT_FAILURE);
    }
    DIR* q;
    if(argc==1){
        q=opendir(".");
    }
    else{
        q=opendir(argv[1]);
    }
    if (q == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    while((d=readdir(q))!=NULL){
        if(!strcmp(d->d_name,".") || !strcmp(d->d_name,"..")){
            continue;
        }
        printf("%s\n",d->d_name);
    }
    closedir(q);
    exit(EXIT_SUCCESS);
}