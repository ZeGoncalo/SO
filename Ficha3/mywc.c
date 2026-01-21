#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    int opt;
    int count_chars = 0, count_words = 0, count_lines = 0;

    if (argc < 3) {
        fprintf(stderr, "usage: %s [-c | -w | -l] file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    while ((opt = getopt(argc, argv, "cwl")) != -1) {
        switch (opt) {
            case 'c': count_chars = 1; break;
            case 'w': count_words = 1; break;
            case 'l': count_lines = 1; break;
            default:
                fprintf(stderr, "usage: %s [-c | -w | -l] file\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    char *filename = argv[optind];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "error: could not open %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int c;
    long n_chars = 0, n_words = 0, n_lines = 0;
    int in_word = 0;

    while ((c = fgetc(file)) != EOF) {
        n_chars++;

        if (c == '\n')
            n_lines++;

        if (isspace(c))
            in_word = 0;
        else if (!in_word) {
            in_word = 1;
            n_words++;
        }
    }

    fclose(file);

    if (count_chars)
        printf("%ld\n", n_chars);
    else if (count_words)
        printf("%ld\n", n_words);
    else if (count_lines)
        printf("%ld\n", n_lines);

    return EXIT_SUCCESS;
}
