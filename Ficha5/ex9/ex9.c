#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void die_usage() {
    fprintf(stderr, "Uso: parcc -o prog prog1.c prog2.c ... progn.c\n");
    exit(EXIT_FAILURE);
}

static int ends_with(const char *s, const char *suffix) {
    size_t ls = strlen(s), lsu = strlen(suffix);
    return (ls >= lsu && strcmp(s + (ls - lsu), suffix) == 0);
}

static void obj_name(const char *cfile, char *out, size_t outsz) {
    // troca ".c" por ".o"
    snprintf(out, outsz, "%s", cfile);
    char *dot = strrchr(out, '.');
    if (dot) strcpy(dot, ".o");
}

int main(int argc, char *argv[]) {
    if (argc < 4) die_usage();
    if (strcmp(argv[1], "-o") != 0) die_usage();

    char *outprog = argv[2];
    int nfiles = argc - 3;
    char **cfiles = &argv[3];

    // validar ficheiros .c
    for (int i = 0; i < nfiles; i++) {
        if (!ends_with(cfiles[i], ".c")) {
            fprintf(stderr, "Erro: '%s' não é ficheiro .c\n", cfiles[i]);
            return EXIT_FAILURE;
        }
    }

    pid_t *pids = malloc(sizeof(pid_t) * nfiles);
    if (!pids) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    // 1) COMPILAÇÃO EM PARALELO: gcc -Wall -c file.c
    for (int i = 0; i < nfiles; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            free(pids);
            return EXIT_FAILURE;
        }

        if (pid == 0) {
            // filho
            printf("[pid:%d] compiling %s ...\n", getpid(), cfiles[i]);
            fflush(stdout);

            char *args[] = {"gcc", "-Wall", "-c", cfiles[i], NULL};
            execvp("gcc", args);
            perror("execvp(gcc)");
            exit(EXIT_FAILURE);
        }

        // pai guarda PID
        pids[i] = pid;
    }

    // 2) Esperar todos e ver se algum falhou
    int ok = 1;
    for (int i = 0; i < nfiles; i++) {
        int status;
        pid_t w = waitpid(pids[i], &status, 0);
        if (w == -1) {
            perror("waitpid");
            ok = 0;
            continue;
        }

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            ok = 0;
        }
    }

    free(pids);

    if (!ok) {
        fprintf(stderr, "parcc: erro na compilação (não vou fazer link)\n");
        return EXIT_FAILURE;
    }

    // 3) LINK FINAL: gcc prog1.o ... progn.o -o prog
    // construir argv dinamicamente: ["gcc", obj1, obj2, ..., "-o", outprog, NULL]
    char **link_args = malloc(sizeof(char*) * (nfiles + 4));
    if (!link_args) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    link_args[0] = "gcc";

    // nomes dos .o (guardar em buffers)
    char **objs = malloc(sizeof(char*) * nfiles);
    if (!objs) {
        perror("malloc");
        free(link_args);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < nfiles; i++) {
        objs[i] = malloc(1024);
        if (!objs[i]) {
            perror("malloc");
            return EXIT_FAILURE;
        }
        obj_name(cfiles[i], objs[i], 1024);
        link_args[i + 1] = objs[i];
    }

    link_args[nfiles + 1] = "-o";
    link_args[nfiles + 2] = outprog;
    link_args[nfiles + 3] = NULL;

    printf("[pid:%d] linking -> %s ...\n", getpid(), outprog);
    fflush(stdout);

    // executa gcc (link) no processo principal
    pid_t lpid = fork();
    if (lpid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (lpid == 0) {
        execvp("gcc", link_args);
        perror("execvp(gcc link)");
        exit(EXIT_FAILURE);
    }

    int lstatus;
    if (waitpid(lpid, &lstatus, 0) == -1) {
        perror("waitpid(link)");
        return EXIT_FAILURE;
    }

    // limpar buffers
    for (int i = 0; i < nfiles; i++) free(objs[i]);
    free(objs);
    free(link_args);

    if (!WIFEXITED(lstatus) || WEXITSTATUS(lstatus) != 0) {
        fprintf(stderr, "parcc: erro no linking\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
