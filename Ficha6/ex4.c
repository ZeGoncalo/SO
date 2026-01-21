#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void on_sigusr1(int sig) {
    (void)sig;
    printf("caught SIGUSR1\n");
    fflush(stdout);
}

static void on_sigusr2(int sig) {
    (void)sig;
    printf("caught SIGUSR2\n");
    fflush(stdout);
}

static void on_sigtstp(int sig) {
    (void)sig;
    printf("caught SIGTSTP (CTRL-Z)\n");
    fflush(stdout);
}

static void on_sigint(int sig) {
    (void)sig;
    printf("caught SIGINT (CTRL-C)\n");
    fflush(stdout);
}

static void on_sighup(int sig) {
    (void)sig;
    printf("caught SIGHUP (logout / terminal closed)\n");
    fflush(stdout);
}

static void reg_handler(int signum, void (*handler)(int), const char *name) {
    if (signal(signum, handler) == SIG_ERR) {
        fprintf(stderr, "Can't catch %s: %s\n", name, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    printf("my PID is %d\n", getpid());
    fflush(stdout);

    reg_handler(SIGUSR1, on_sigusr1, "SIGUSR1");
    reg_handler(SIGUSR2, on_sigusr2, "SIGUSR2");
    reg_handler(SIGTSTP, on_sigtstp, "SIGTSTP");
    reg_handler(SIGINT,  on_sigint,  "SIGINT");
    reg_handler(SIGHUP,  on_sighup,  "SIGHUP");

    for (;;) pause();
}
