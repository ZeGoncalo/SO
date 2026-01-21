#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

/* complete ... */
int main(int argc, char* argv[]) {
for(int i = 1 ; i < argc ; i++) {
pid_t pid = fork();
if( pid == 0 ) {
printf("%d: %s\n", getpid(), argv[i]);
exit(EXIT_SUCCESS);
}
}
for(int i = 1 ; i < argc ; i++)
wait(NULL);
exit(EXIT_SUCCESS);
}