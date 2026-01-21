#include <stdlib.h>
#include <stdio.h>

void f1(void);
void f2(void);

int main(int argc, char* argv[]){
    printf("Hello\n");
    f1();
    f2();
    exit(EXIT_SUCCESS);
}