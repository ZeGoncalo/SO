#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s string file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *pattern = argv[1];
    char *filename = argv[2];

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "error: could not open %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int lineno = 0;
    int first = 1;

    printf("[");

    while ((read = getline(&line, &len, file)) != -1) {
        lineno++;
        char *pos = line;

        while ((pos = strstr(pos, pattern)) != NULL) {
            if (!first)
                printf(", ");
            printf("%d:%ld", lineno, (long)(pos - line + 1));
            first = 0;

            pos++;
        }
    }

    printf("]\n");

    free(line);
    fclose(file);
    return EXIT_SUCCESS;
}
