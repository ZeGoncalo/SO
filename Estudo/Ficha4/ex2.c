#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            printf("error: cannot open %s\n", argv[i]);
            exit(EXIT_FAILURE);
        }
        off_t filesize = lseek(fd, 0, SEEK_END);
        if (filesize == -1) {
            printf("error: %s has no content\n", argv[i]);
            close(fd);
            exit(EXIT_FAILURE);
        }
        for (off_t pos = filesize - 1; pos >= 0; pos--) {
            char c;
            lseek(fd, pos, SEEK_SET); 
            if (read(fd, &c, 1) != 1) {
                close(fd);
                exit(EXIT_FAILURE);
            }
            write(STDOUT_FILENO, &c, 1); 
        }
        close(fd);
    }
    write(STDOUT_FILENO, "\n", 1); 
    exit(EXIT_SUCCESS);
}
