#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
void getperms(mode_t m){
    printf(S_ISDIR(m) ? "d":"-");
    printf(m & S_IRUSR ? "r":"-");
    printf(m & S_IWUSR ? "w":"-");
    printf(m & S_IXUSR ? "x":"-");
    printf(m & S_IRGRP ? "r":"-");
    printf(m & S_IWGRP ? "w":"-");
    printf(m & S_IXGRP ? "x":"-");
    printf(m & S_IROTH ? "r":"-");
    printf(m & S_IWOTH ? "w":"-");
    printf(m & S_IXOTH ? "x":"-");
}
int main(int argc, char** argv) {
    int len;
    if (argc != 2) {
        fprintf (stderr, "usage: %s dirname/filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    struct stat st;
    if (lstat(argv[1], &st) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }
    if(S_ISDIR(st.st_mode)){
        DIR *q = opendir(argv[1]);
        if (q == NULL) {
            fprintf (stderr, "cannot open directory: %s\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        printf ("%s/\n", argv[1]);
        struct dirent *p = readdir(q);
        while (p != NULL) {
            if(strcmp(p->d_name,".")!=0 && strcmp(p->d_name,"..")!=0){
                char fullpath[1024];
                snprintf(fullpath,sizeof(fullpath),"%s/%s",argv[1],p->d_name);
                stat(fullpath,&st);
                getperms(st.st_mode);
                printf("\t%s\n", p->d_name);    
            }
            p = readdir(q);
        }
        closedir(q);
    }
    else{
        struct stat st;
        stat(argv[1],&st);
        getperms(st.st_mode);
        printf(" ./%s\n",argv[1]);
    }
    
    exit(EXIT_SUCCESS);
}


