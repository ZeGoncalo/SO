#include <stdlib.h>
#include <stdio.h>
#define BUFFER_SIZE 1024
int main(int argc, char* argv[]){
    if(argc==1){
        printf("usage: %s file1 file2 ...", argv[0]);
        exit(EXIT_FAILURE);
    }
    for(int i=1;i<argc;i++){
        FILE* file=fopen(argv[i], "r");
        if ( file == NULL ) {
            printf("error: could not open %s\n", argv[i]);
            exit(EXIT_FAILURE);
        }
        char buffer[BUFFER_SIZE];
        int nchar= fread(buffer,sizeof(char),BUFFER_SIZE, file);
        while(nchar>0){
            fwrite(buffer, sizeof(char),nchar, stdout);
            nchar= fread(buffer,sizeof(char),BUFFER_SIZE, file);
        }
        fclose(file);
    }
    exit(EXIT_SUCCESS);
}