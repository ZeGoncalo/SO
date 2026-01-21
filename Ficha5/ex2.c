#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    printf("Processo inicial: %d\n",getpid());
    for (int i = 0; i < 4; i++){
        while (wait(NULL) > 0);
        fork();
        printf("Fork %d:%d\n",i+1,getpid());
    }
    exit(EXIT_SUCCESS);
}
