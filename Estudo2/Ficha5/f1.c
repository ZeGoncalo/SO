#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
void f2(void);
int main(int argc, char* argv[]){
    printf("Hello\n");
    f2();
    fflush(stdout);
    return 0;
}