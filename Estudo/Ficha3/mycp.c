#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024
int main(int argc, char* argv[]){
    if(argc!=3){
        printf("usage: %s file1 file2\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE* f1=fopen(argv[1],"r");
    FILE* f2=fopen(argv[2],"w");
    if(f1==NULL || f2==NULL){
        printf("Couldn't open files\n");
        exit(EXIT_FAILURE);
    }
    char buffer[BUFFER_SIZE];
    int nchar=fread(buffer, sizeof(char), BUFFER_SIZE, f1);
    while(nchar>0){
        fwrite(buffer, sizeof(char), nchar, f2);
        nchar=fread(buffer, sizeof(char), BUFFER_SIZE, f1); 
    }
    fclose(f1);
    fclose(f2);
    exit(EXIT_SUCCESS);
}