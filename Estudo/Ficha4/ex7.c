#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
void findfile (char* dirpath, const char* arg){
    struct dirent* d;
    DIR* q = opendir(dirpath);
    struct stat st;
    while((d=readdir(q))!=NULL){
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) continue;
        char fullpath[1024];
        snprintf(fullpath,sizeof(fullpath),"%s/%s",dirpath,d->d_name);
        if (stat(fullpath, &st) == -1) {
            perror(fullpath);
            continue;
        }
        if(S_ISDIR(st.st_mode)){findfile(fullpath,arg);}
        else{
            if(strcmp(d->d_name, arg)==0){printf("%s/%s\n",dirpath,d->d_name);}
        }
    }
}

int main(int argc, char* argv[]){
    findfile(argv[1],argv[2]);
    exit(EXIT_SUCCESS);
}