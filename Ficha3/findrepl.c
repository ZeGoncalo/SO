#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PAIRS 100
#define MAX_WORD 256

typedef struct {
    char find[MAX_WORD];
    char repl[MAX_WORD];
} Pair;


int is_delim(int c) {
    return isspace(c) || ispunct(c);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s find1-repl1 [find2-repl2 ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Pair pairs[MAX_PAIRS];
    int npairs = 0;

    for (int i = 1; i < argc && npairs < MAX_PAIRS; i++) {
        char *sep = strchr(argv[i], '-');
        if (sep == NULL) {
            fprintf(stderr, "error: invalid pair format '%s' (expected find-repl)\n", argv[i]);
            exit(EXIT_FAILURE);
        }

        *sep = '\0'; // divide a string no '-'
        strncpy(pairs[npairs].find, argv[i], MAX_WORD - 1);
        strncpy(pairs[npairs].repl, sep + 1, MAX_WORD - 1);
        npairs++;
    }

    int c;
    char word[MAX_WORD];
    int i = 0;

    while ((c = getchar()) != EOF) {
        if (is_delim(c)) {
            if (i > 0) {
                word[i] = '\0';
                int replaced = 0;

                // procura se a palavra tem substituição
                for (int p = 0; p < npairs; p++) {
                    if (strcmp(word, pairs[p].find) == 0) {
                        fputs(pairs[p].repl, stdout);
                        replaced = 1;
                        break;
                    }
                }

                if (!replaced)
                    fputs(word, stdout);

                i = 0;
            }

            // escreve o delimitador tal como está
            putchar(c);
        } else {
            if (i < MAX_WORD - 1)
                word[i++] = c;
        }
    }

    // se o texto não terminar com delimitador
    if (i > 0) {
        word[i] = '\0';
        int replaced = 0;
        for (int p = 0; p < npairs; p++) {
            if (strcmp(word, pairs[p].find) == 0) {
                fputs(pairs[p].repl, stdout);
                replaced = 1;
                break;
            }
        }
        if (!replaced)
            fputs(word, stdout);
    }

    return EXIT_SUCCESS;
}
