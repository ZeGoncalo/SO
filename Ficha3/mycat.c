#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s file1 [file2 ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];

    for (int i = 1; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");

        if (file == NULL) {
            fprintf(stderr, "error: could not open %s\n", argv[i]);
            continue;
        }

        int nchars = fread(buffer, sizeof(char), BUFFER_SIZE, file);
        while (nchars > 0) {
            fwrite(buffer, sizeof(char), nchars, stdout);
            nchars = fread(buffer, sizeof(char), BUFFER_SIZE, file);
        }

        fclose(file);
    }

    return EXIT_SUCCESS;
}
