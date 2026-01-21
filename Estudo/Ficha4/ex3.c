#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
int main(int argc, char* argv[]) {
    struct stat info;
    if (argc < 2) {
        fprintf(stderr, "usage: %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int by=0,bl=0;
    for (int i=1;i<argc;i++){
        int retv = stat(argv[i], &info);
        if (retv == -1) {
                fprintf(stderr, "fsize: Can’t stat %s\n", argv[1]);
                exit(EXIT_FAILURE);
        }
        by+=(int)info.st_size;
        bl+=(int)info.st_blocks;
        char* t = ctime(&info.st_mtime);
        t[strlen(t) - 1] = '\0';
        printf("%s size: %d bytes, disk_blocks: %d, last modified: %s, owner UID: %d\n", argv[1], (int)info.st_size, (int)info.st_blocks, t, (int)info.st_uid);    
    }
    printf("total size: %d bytes, disk_blocks: %d\n",by,bl); 
    exit(EXIT_SUCCESS);
}
