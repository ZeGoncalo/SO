#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
int main(int argc, char* argv[]){
    if(argc!=3){
        printf("usage: %s string1 string2\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char* q=strdup(argv[1]);
    char* w=q;
    char* u= (char*)malloc((strlen(argv[1])+1)*sizeof(char));
    char* r=u;
    char* s=strdup(argv[2]);
    char* x=s;
    char* v= (char*)malloc((strlen(argv[2])+1)*sizeof(char));
    char* t=v;
    while(*q!='\0'){
        *u=tolower(*q);
        q++;
        u++;
    }
    *u='\0';
    while(*s!='\0'){
        *v=tolower(*s);
        s++;
        v++;
    }
    *v='\0';
    char* y= strstr(t,r);
    if (y==NULL){printf("%s não está em %s\n", argv[1],argv[2]);}
    else{printf("%s está em %s\n", argv[1],argv[2]);}
    free(w);
    free(r);
    free(x);
    free(t);
    exit(EXIT_SUCCESS);
}