#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ARGS 64

int main() {
    for (;;) {
        /* give prompt, read command and null terminate it */
        fprintf(stdout, "$ ");
        char buf[1024];
        char* command = fgets(buf, sizeof(buf), stdin);
        if(command == NULL || !strcmp(command,"exit\n"))
        break;
        command[strlen(buf) - 1] = '\0';

        // ---- separar em argumentos ----
        char *args[MAX_ARGS];
        int argc = 0;

        char *token = strtok(buf, " ");
        while (token != NULL && argc < MAX_ARGS - 1) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL; // obrigatório para execvp

        if (argc == 0) continue; // linha vazia (ex: só Enter)

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // child
            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }

        if (waitpid(pid, NULL, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
