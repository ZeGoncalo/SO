#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ARGS 64
#define FIFO_PATH "/tmp/mypipefifo"

static void die_usage(const char *prog) {
    fprintf(stderr, "Uso: %s \"cmd1 | cmd2\"\n", prog);
    exit(EXIT_FAILURE);
}

static void trim_spaces(char *s) {
    while (*s == ' ') memmove(s, s + 1, strlen(s));
    int n = (int)strlen(s);
    while (n > 0 && s[n - 1] == ' ') s[--n] = '\0';
}

static void split_args(char *cmd, char *out_argv[MAX_ARGS]) {
    int i = 0;
    char *tok = strtok(cmd, " ");
    while (tok && i < MAX_ARGS - 1) {
        out_argv[i++] = tok;
        tok = strtok(NULL, " ");
    }
    out_argv[i] = NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) die_usage(argv[0]);

    char line[1024];
    strncpy(line, argv[1], sizeof(line));
    line[sizeof(line) - 1] = '\0';

    // separar "cmd1 | cmd2"
    char *left = strtok(line, "|");
    char *right = strtok(NULL, "|");

    if (!left || !right) {
        fprintf(stderr, "Erro: formato esperado \"cmd1 | cmd2\"\n");
        return EXIT_FAILURE;
    }

    trim_spaces(left);
    trim_spaces(right);

    // preparar argv para execvp
    char left_copy[1024], right_copy[1024];
    strncpy(left_copy, left, sizeof(left_copy));
    strncpy(right_copy, right, sizeof(right_copy));
    left_copy[sizeof(left_copy) - 1] = '\0';
    right_copy[sizeof(right_copy) - 1] = '\0';

    char *cmd1_argv[MAX_ARGS];
    char *cmd2_argv[MAX_ARGS];
    split_args(left_copy, cmd1_argv);
    split_args(right_copy, cmd2_argv);

    // criar FIFO (se já existir ignorar)
    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return EXIT_FAILURE;
    }

    // Filho 1: cmd1 -> escreve para FIFO (stdout redirecionado)
    pid_t p1 = fork();
    if (p1 == -1) {
        perror("fork");
        unlink(FIFO_PATH);
        return EXIT_FAILURE;
    }

    if (p1 == 0) {
        int fdw = open(FIFO_PATH, O_WRONLY); // bloqueia até haver leitor
        if (fdw == -1) {
            perror("open fifo write");
            exit(EXIT_FAILURE);
        }

        dup2(fdw, STDOUT_FILENO);
        close(fdw);

        execvp(cmd1_argv[0], cmd1_argv);
        perror("execvp cmd1");
        exit(EXIT_FAILURE);
    }

    // Filho 2: cmd2 -> lê da FIFO (stdin redirecionado)
    pid_t p2 = fork();
    if (p2 == -1) {
        perror("fork");
        unlink(FIFO_PATH);
        return EXIT_FAILURE;
    }

    if (p2 == 0) {
        int fdr = open(FIFO_PATH, O_RDONLY); // bloqueia até haver writer
        if (fdr == -1) {
            perror("open fifo read");
            exit(EXIT_FAILURE);
        }

        dup2(fdr, STDIN_FILENO);
        close(fdr);

        execvp(cmd2_argv[0], cmd2_argv);
        perror("execvp cmd2");
        exit(EXIT_FAILURE);
    }

    // pai espera ambos
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    // apagar FIFO
    unlink(FIFO_PATH);

    return 0;
}
