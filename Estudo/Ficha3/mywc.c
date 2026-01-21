#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int main(int argc, char* argv[]){
    if (argc!=3){
        printf("usage: %s -option file\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE* file =fopen(argv[2],"r");
    if(file==NULL){
        printf("Couldn't open %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }
    if (strcmp(argv[1], "-c") == 0){
        int size=1, count=0;
        char buffer[1];
        int nchar=fread(buffer, sizeof(char), size, file);
        while(nchar>0){
            count++;
            nchar=fread(buffer, sizeof(char), size, file);
        }
        printf("%d\n", count);
        fclose(file);
        exit(EXIT_SUCCESS);
    }
    if (strcmp(argv[1], "-l") == 0){
        int count=0;
        char* line=NULL;
        size_t len=0;
        ssize_t nread;
        while ((nread = getline(&line, &len, file)) != -1) {
           count++;
        }
        free(line);
        fclose(file);
        printf("%d\n", count);
        exit(EXIT_SUCCESS);
    }
    if (strcmp(argv[1], "-w") == 0){
        int size=1, count=0, in_word=0;
        char buffer[1];
        int nchar=fread(buffer, sizeof(char), size, file);
        while(nchar>0){
            if (!isspace(buffer[0])) {
                if (!in_word) {
                    count++;
                    in_word = 1;
                }
            } 
            else {in_word = 0;}
            nchar=fread(buffer, sizeof(char), size, file);
        }
        printf("%d\n", count);
        fclose(file);
        exit(EXIT_SUCCESS);
    }
    printf("usage: %s -option file\n", argv[0]);
    exit(EXIT_FAILURE);
}