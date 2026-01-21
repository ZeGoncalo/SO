#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
void findfile(char* dirpath, const char* file){
    struct stat st;
    struct dirent* d;
    DIR* q;
    q=opendir(dirpath);
    if (q == NULL) {
        perror(dirpath);
        return;
    }
    while((d=readdir(q))!=NULL){
        if(!strcmp(d->d_name,".") || (!strcmp(d->d_name,".."))){
            continue;
        }
        char fullpath[1024];
        snprintf(fullpath,sizeof(fullpath),"%s/%s",dirpath,d->d_name);
        if (stat(fullpath,&st)==-1) {
            perror(fullpath);
            continue;
        }
        if(!strcmp(d->d_name,file)){
            printf("%s/%s\n",dirpath,d->d_name);
        }
        if(S_ISDIR(st.st_mode)){
            findfile(fullpath,file);
        }
    }
    closedir(q);
}
int main(int argc, char* argv[]){
    if(argc!=3){
        printf("Usage: %s dir file\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    findfile(argv[1],argv[2]);
    exit(EXIT_SUCCESS);
}