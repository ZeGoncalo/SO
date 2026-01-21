#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
int main(int argc, char* argv[]){
	if(argc!=2){
		printf("usage: %s string\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	char* p=strdup(argv[1]);
	char* s = p; 
	char* q=(char*)malloc((strlen(argv[1])+1)*sizeof(char));
	char* r = q;
	while(*p!='\0'){
		*q=tolower(*p);
		p++;
		q++;
	}
	*q='\0';
	printf("%s\n", r);
	free(s);
	free(r);
	exit(EXIT_SUCCESS);
}
