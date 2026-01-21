#include <sys/wait.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>

#define BUFSIZE 4096

static void die_usage(const char *prog) {
    fprintf(stderr, "Uso: %s <ficheiro.txt>\n", prog);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc != 2) die_usage(argv[0]);

    int sockets[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == -1) {
        perror("socketpair");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        /* ---------------- PARENT ---------------- */
        close(sockets[1]); // pai usa sockets[0]

        int f = open(argv[1], O_RDONLY);
        if (f == -1) {
            perror("open");
            close(sockets[0]);
            exit(EXIT_FAILURE);
        }

        // 1) enviar ficheiro ao filho
        char buf[BUFSIZE];
        ssize_t n;

        while ((n = read(f, buf, BUFSIZE)) > 0) {
            ssize_t total = 0;
            while (total < n) {
                ssize_t w = write(sockets[0], buf + total, n - total);
                if (w == -1) {
                    perror("write(to child)");
                    close(f);
                    close(sockets[0]);
                    exit(EXIT_FAILURE);
                }
                total += w;
            }
        }

        if (n == -1) {
            perror("read(file)");
            close(f);
            close(sockets[0]);
            exit(EXIT_FAILURE);
        }

        close(f);

        // IMPORTANTE: dizer ao filho "acabou o envio"
        shutdown(sockets[0], SHUT_WR);

        // 2) receber do filho e imprimir no stdout
        while ((n = read(sockets[0], buf, BUFSIZE)) > 0) {
            ssize_t total = 0;
            while (total < n) {
                ssize_t w = write(STDOUT_FILENO, buf + total, n - total);
                if (w == -1) {
                    perror("write(stdout)");
                    close(sockets[0]);
                    exit(EXIT_FAILURE);
                }
                total += w;
            }
        }

        if (n == -1) {
            perror("read(from child)");
            close(sockets[0]);
            exit(EXIT_FAILURE);
        }

        close(sockets[0]);

        if (wait(NULL) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);

    } else {
        /* ---------------- CHILD ---------------- */
        close(sockets[0]); // filho usa sockets[1]

        char buf[BUFSIZE];
        ssize_t n;

        // 1) ler do pai, converter para maiúsculas e devolver
        while ((n = read(sockets[1], buf, BUFSIZE)) > 0) {
            for (ssize_t i = 0; i < n; i++) {
                buf[i] = (char) toupper((unsigned char) buf[i]);
            }

            ssize_t total = 0;
            while (total < n) {
                ssize_t w = write(sockets[1], buf + total, n - total);
                if (w == -1) {
                    perror("write(to parent)");
                    close(sockets[1]);
                    exit(EXIT_FAILURE);
                }
                total += w;
            }
        }

        if (n == -1) {
            perror("read(from parent)");
            close(sockets[1]);
            exit(EXIT_FAILURE);
        }

        close(sockets[1]);
        exit(EXIT_SUCCESS);
    }
}
