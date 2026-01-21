#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 128

int main() {
    char* myfifo = "/tmp/myfifo";

    int fd = open(myfifo, O_RDONLY);
    if (fd == -1) { perror("open"); exit(EXIT_FAILURE); }

    while (1) {
        char text[BUF_SIZE];
        ssize_t n = read(fd, text, BUF_SIZE);

        if (n == -1) { perror("read"); break; }
        if (n == 0) break; // EOF

        write(STDOUT_FILENO, text, n);
    }

    close(fd);
    return 0;
}
