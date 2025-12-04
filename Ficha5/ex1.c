#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid = fork();
    if (pid > 0) wait(NULL);  
    printf("Depois do 1º fork PID=%d\n", getpid());
    pid = fork();
    if (pid > 0) wait(NULL);
    printf("Depois do 2º fork PID=%d\n", getpid());
    pid = fork();
    if (pid > 0) wait(NULL);
    printf("Depois do 3º fork PID=%d\n", getpid());
    return 0;
}
