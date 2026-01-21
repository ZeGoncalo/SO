#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void die_usage() {
    fprintf(stderr, "Uso: txt2epub book1.txt book2.txt ... bookn.txt\n");
    exit(EXIT_FAILURE);
}

static int ends_with(const char *s, const char *suffix) {
    size_t ls = strlen(s), lsu = strlen(suffix);
    return (ls >= lsu && strcmp(s + (ls - lsu), suffix) == 0);
}

static void epub_name(const char *txtfile, char *out, size_t outsz) {
    // troca ".txt" por ".epub"
    snprintf(out, outsz, "%s", txtfile);
    char *dot = strrchr(out, '.');
    if (dot) strcpy(dot, ".epub");
}

int main(int argc, char *argv[]) {
    if (argc < 2) die_usage();

    int nfiles = argc - 1;
    char **txtfiles = &argv[1];

    // validar .txt
    for (int i = 0; i < nfiles; i++) {
        if (!ends_with(txtfiles[i], ".txt")) {
            fprintf(stderr, "Erro: '%s' não é ficheiro .txt\n", txtfiles[i]);
            return EXIT_FAILURE;
        }
    }

    pid_t *pids = malloc(sizeof(pid_t) * nfiles);
    if (!pids) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    // 1) CONVERSÃO EM PARALELO: pandoc book.txt -o book.epub
    for (int i = 0; i < nfiles; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            free(pids);
            return EXIT_FAILURE;
        }

        if (pid == 0) {
            // filho
            printf("[pid:%d] converting %s ...\n", getpid(), txtfiles[i]);
            fflush(stdout);

            char epub[1024];
            epub_name(txtfiles[i], epub, sizeof(epub));

            char *args[] = {"pandoc", txtfiles[i], "-o", epub, NULL};
            execvp("pandoc", args);

            perror("execvp(pandoc)");
            exit(EXIT_FAILURE);
        }

        pids[i] = pid;
    }

    // 2) Esperar todos e verificar se falhou algum
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
        fprintf(stderr, "txt2epub: erro na conversão (não vou criar zip)\n");
        return EXIT_FAILURE;
    }

    // 3) Criar o ZIP: zip ebooks.zip book1.epub ... bookn.epub
    char **zip_args = malloc(sizeof(char*) * (nfiles + 4));
    if (!zip_args) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    zip_args[0] = "zip";
    zip_args[1] = "ebooks.zip";

    // criar lista de epub's
    char **epubs = malloc(sizeof(char*) * nfiles);
    if (!epubs) {
        perror("malloc");
        free(zip_args);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < nfiles; i++) {
        epubs[i] = malloc(1024);
        if (!epubs[i]) {
            perror("malloc");
            return EXIT_FAILURE;
        }
        epub_name(txtfiles[i], epubs[i], 1024);
        zip_args[i + 2] = epubs[i];
    }

    zip_args[nfiles + 2] = NULL;

    printf("[pid:%d] creating ebooks.zip ...\n", getpid());
    fflush(stdout);

    pid_t zpid = fork();
    if (zpid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (zpid == 0) {
        execvp("zip", zip_args);
        perror("execvp(zip)");
        exit(EXIT_FAILURE);
    }

    int zstatus;
    if (waitpid(zpid, &zstatus, 0) == -1) {
        perror("waitpid(zip)");
        return EXIT_FAILURE;
    }

    // limpar memória
    for (int i = 0; i < nfiles; i++) free(epubs[i]);
    free(epubs);
    free(zip_args);

    if (!WIFEXITED(zstatus) || WEXITSTATUS(zstatus) != 0) {
        fprintf(stderr, "txt2epub: erro ao criar zip\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
