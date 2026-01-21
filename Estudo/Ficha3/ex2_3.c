#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char* argv[]){
    if(argc!=3){
        printf("usage: %s string string2", argv[0]);
        exit(EXIT_FAILURE);
    }
    char* p=(char*) malloc((strlen(argv[1])+1)*sizeof(char));
    char* q=(char*) malloc((strlen(argv[2])+1)*sizeof(char));
    for(int i=0;i<strlen(argv[1]);i++){
        p[i]=argv[1][i];
    }
    p[strlen(argv[1])]='\0';
    for(int i=0;i<strlen(argv[2]);i++){
        q[i]=argv[2][i];
    }
    q[strlen(argv[2])]='\0';
    int count=0;
    char* r=q;
    while((r=strstr(r,p))!=NULL){
        count++;
        r++;
    }
    printf("%d\n",count);
    free(p);
    free(q);
}