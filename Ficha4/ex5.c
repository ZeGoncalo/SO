#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (access(argv[1], F_OK) == 0) {
        struct timeval times[2];
        times[0].tv_sec = times[1].tv_sec = time(NULL);
        times[0].tv_usec = times[1].tv_usec = 0;
        if (utimes(argv[1], times) == -1) {
            perror("utimes");
            exit(EXIT_FAILURE);
        }
    } 
    else {
        mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        int fd = open(argv[1], O_CREAT | O_WRONLY, perms);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        close(fd);
    }
    exit(EXIT_SUCCESS);
}
