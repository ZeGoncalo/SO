#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
static int flag = 1;
static void on_sigint(int sig){
    flag=0;
}
int main(int argc, char* argv[]) {
printf("my PID is %d\n", getpid());
if (signal(SIGINT, on_sigint) == SIG_ERR) {
    fprintf(stderr, "Can't catch SIGINT: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}
/* enter potentially infinite loop ... */
while(flag)
pause();
printf("Exiting gracefully\n");
exit(EXIT_SUCCESS);
}