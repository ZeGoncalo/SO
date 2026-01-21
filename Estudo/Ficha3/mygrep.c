#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char* argv[]){
    if(argc!=3){
        printf("usage: %s string file\n", argv[0]);
        exit(EXIT_SUCCESS);
    }
    FILE* file=fopen(argv[2],"r");
    if (file==NULL){
        printf("Couldn't open %s\n", argv[2]);
        exit(EXIT_SUCCESS);
    }
    char* s=argv[1];
    size_t size=strlen(s);
    size_t sline=0;
    int nline=0, first=1;
    char* line = NULL;
    ssize_t nread;
    printf("[");
    while((nread=getline(&line,&sline,file)!=-1)){
        for(int col=0;col<sline-size;col++){
            if(strncmp(s,&line[col],size)==0){
                if(!first){printf(",");}
                printf("%d:%d",nline+1,col+1);
                first=0;
            }
        }
        nline++;
    }
    fclose(file);
    printf("]\n");
    exit(EXIT_SUCCESS);
}