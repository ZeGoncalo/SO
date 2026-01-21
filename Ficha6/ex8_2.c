#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1
#define MAX_ARGS 64

static void die_usage(const char *prog) {
    fprintf(stderr, "Uso: %s \"cmd1 | cmd2\"\n", prog);
    exit(EXIT_FAILURE);
}

static void trim_spaces(char *s) {
    // remove espaços no início
    while (*s == ' ') memmove(s, s + 1, strlen(s));
    // remove espaços no fim
    int n = strlen(s);
    while (n > 0 && s[n - 1] == ' ') s[--n] = '\0';
}

static void split_args(char *cmd, char *argv_out[MAX_ARGS]) {
    int i = 0;
    char *tok = strtok(cmd, " ");
    while (tok && i < MAX_ARGS - 1) {
        argv_out[i++] = tok;
        tok = strtok(NULL, " ");
    }
    argv_out[i] = NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) die_usage(argv[0]);

    // copiar string porque vamos modificar com strtok
    char line[1024];
    strncpy(line, argv[1], sizeof(line));
    line[sizeof(line) - 1] = '\0';

    // separar pelo '|'
    char *left = strtok(line, "|");
    char *right = strtok(NULL, "|");
    if (!left || !right) {
        fprintf(stderr, "Erro: formato esperado \"cmd1 | cmd2\"\n");
        return EXIT_FAILURE;
    }

    trim_spaces(left);
    trim_spaces(right);

    // preparar argv de cada comando
    char left_copy[1024], right_copy[1024];
    strncpy(left_copy, left, sizeof(left_copy));
    strncpy(right_copy, right, sizeof(right_copy));
    left_copy[sizeof(left_copy) - 1] = '\0';
    right_copy[sizeof(right_copy) - 1] = '\0';

    char *cmd1_argv[MAX_ARGS];
    char *cmd2_argv[MAX_ARGS];
    split_args(left_copy, cmd1_argv);
    split_args(right_copy, cmd2_argv);

    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t p1 = fork();
    if (p1 == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (p1 == 0) {
        // filho 1: executa cmd1 e escreve na pipe
        close(fd[READ_END]);
        dup2(fd[WRITE_END], STDOUT_FILENO);
        close(fd[WRITE_END]);

        execvp(cmd1_argv[0], cmd1_argv);
        perror("execvp cmd1");
        exit(EXIT_FAILURE);
    }

    pid_t p2 = fork();
    if (p2 == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (p2 == 0) {
        // filho 2: executa cmd2 e lê da pipe
        close(fd[WRITE_END]);
        dup2(fd[READ_END], STDIN_FILENO);
        close(fd[READ_END]);

        execvp(cmd2_argv[0], cmd2_argv);
        perror("execvp cmd2");
        exit(EXIT_FAILURE);
    }

    // pai: fecha os dois lados e espera
    close(fd[READ_END]);
    close(fd[WRITE_END]);

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    return 0;
}
