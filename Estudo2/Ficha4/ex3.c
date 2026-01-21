#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
int main(int argc, char* argv[]){
    if (argc < 2) {
        printf("Usage: %s file1 file2 ...\n", argv[0]);
        return 1;
    }
    int total=0;
    for(int i=1;i<argc;i++){
        struct stat st;
        if(stat(argv[i],&st)==-1){
            perror("stat");
            continue;
        }
        total+=st.st_size;
        printf("%s :\n\tsize:%d\n\tblocks:%d\n",argv[i],(int)st.st_size,(int)st.st_blocks);
    }
    printf("Total: %d\n",total);
    exit(EXIT_SUCCESS);
}