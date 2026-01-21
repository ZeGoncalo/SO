#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 4096

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <ficheiro.txt>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1) { perror("pipe"); exit(EXIT_FAILURE); }
    if ((pid = fork()) == -1) { perror("fork"); exit(EXIT_FAILURE); }

    if (pid > 0) {
        /* PARENT: lê ficheiro e escreve na pipe */
        close(fd[0]); // fecha leitura

        int f = open(argv[1], O_RDONLY);
        if (f == -1) {
            perror("open");
            close(fd[1]);
            exit(EXIT_FAILURE);
        }

        char buf[BUFSIZE];
        ssize_t n;

        while ((n = read(f, buf, BUFSIZE)) > 0) {
            ssize_t total = 0;
            while (total < n) {
                ssize_t w = write(fd[1], buf + total, n - total);
                if (w == -1) {
                    perror("write(pipe)");
                    close(f);
                    close(fd[1]);
                    exit(EXIT_FAILURE);
                }
                total += w;
            }
        }

        if (n == -1) {
            perror("read(file)");
            close(f);
            close(fd[1]);
            exit(EXIT_FAILURE);
        }

        close(f);
        close(fd[1]); // importante: sinaliza EOF ao filho

        if (wait(NULL) == -1) { perror("wait"); exit(EXIT_FAILURE); }
        exit(EXIT_SUCCESS);

    } else {
        /* CHILD: lê da pipe e escreve no stdout */
        close(fd[1]); // fecha escrita

        char buf[BUFSIZE];
        ssize_t n;

        while ((n = read(fd[0], buf, BUFSIZE)) > 0) {
            ssize_t total = 0;
            while (total < n) {
                ssize_t w = write(STDOUT_FILENO, buf + total, n - total);
                if (w == -1) {
                    perror("write(stdout)");
                    close(fd[0]);
                    exit(EXIT_FAILURE);
                }
                total += w;
            }
        }

        if (n == -1) {
            perror("read(pipe)");
            close(fd[0]);
            exit(EXIT_FAILURE);
        }

        close(fd[0]);
        exit(EXIT_SUCCESS);
    }
}
