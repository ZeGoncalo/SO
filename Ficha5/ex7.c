#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ARGS 64
#define HISTORY_FILE "myhistory.txt"

static void append_history(const char *line) {
    FILE *f = fopen(HISTORY_FILE, "a");
    if (f == NULL) {
        perror("fopen(history)");
        return;
    }
    fprintf(f, "%s\n", line);
    fclose(f);
}

static void run_tail_history(int n) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        // child: exec tail -n n myhistory.txt
        char nbuf[32];
        snprintf(nbuf, sizeof(nbuf), "%d", n);

        char *args[] = {"tail", "-n", nbuf, HISTORY_FILE, NULL};
        execvp("tail", args);
        perror("execvp(tail)");
        exit(EXIT_FAILURE);
    }

    // parent waits
    if (waitpid(pid, NULL, 0) == -1) {
        perror("waitpid");
    }
}

int main() {
    for (;;) {
        fprintf(stdout, "$ ");
        char buf[1024];

        char *line = fgets(buf, sizeof(buf), stdin);
        if (line == NULL) break;

        buf[strcspn(buf, "\n")] = '\0'; // remove '\n'

        // linha vazia
        if (buf[0] == '\0') continue;

        // comando interno: exit
        if (strcmp(buf, "exit") == 0) {
            break;
        }

        // Guardar cópia do comando original (antes do strtok estragar a string)
        char original[1024];
        strncpy(original, buf, sizeof(original));
        original[sizeof(original) - 1] = '\0';

        // separar em tokens
        char *args[MAX_ARGS];
        int argc = 0;

        char *token = strtok(buf, " ");
        while (token != NULL && argc < MAX_ARGS - 1) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;

        if (argc == 0) continue;

        // comando interno: myhistory n
        if (strcmp(args[0], "myhistory") == 0) {
            if (argc < 2) {
                fprintf(stderr, "Uso: myhistory n\n");
            } else {
                int n = atoi(args[1]);
                if (n <= 0) {
                    fprintf(stderr, "myhistory: n tem de ser inteiro > 0\n");
                } else {
                    run_tail_history(n);
                }
            }
            continue;
        }

        append_history(original);

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
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
