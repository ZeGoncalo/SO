#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 128

int main(int argc, char* argv[]) {
    char* myfifo = "/tmp/myfifo";

    // cria a FIFO (se já existir, ignoramos o erro EEXIST)
    int rv = mkfifo(myfifo, 0666);
    if (rv == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    // abre para escrita (pode bloquear até existir um leitor)
    int fd = open(myfifo, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (1) {
        char text[BUF_SIZE];

        // lê do stdin (teclado)
        if (fgets(text, BUF_SIZE, stdin) == NULL) {
            break; // EOF
        }

        // envia para a FIFO
        if (write(fd, text, strlen(text)) == -1) {
            perror("write");
            break;
        }
    }

    close(fd);
    return 0;
}
