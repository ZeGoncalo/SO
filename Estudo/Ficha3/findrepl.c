#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_word_boundary(char c) {
    return !isalnum(c);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s find-repl [...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t nread;

    while ((nread = getline(&line, &len, stdin)) != -1) {
        for (int i = 1; i < argc; i++) {
            char* arg = strdup(argv[i]);
            char* dash = strchr(arg, '-');
            if (!dash) {
                fprintf(stderr, "Invalid argument: %s\n", argv[i]);
                free(arg);
                free(line);
                exit(EXIT_FAILURE);
            }
            *dash = '\0';
            char* find = arg;
            char* repl = dash + 1;
            size_t find_len = strlen(find);
            size_t repl_len = strlen(repl);
            char* output = (char*)malloc(nread * 2);
            ssize_t out_idx = 0;
            for (ssize_t pos = 0; pos < nread; ) {
                if (pos + find_len <= nread && strncmp(&line[pos], find, find_len) == 0 && (pos == 0 || is_word_boundary(line[pos - 1])) && (pos + find_len == nread || is_word_boundary(line[pos + find_len]))) {
                    memcpy(&output[out_idx], repl, repl_len);
                    out_idx += repl_len;
                    pos += find_len;
                } 
                else {
                    output[out_idx++] = line[pos++];
                }
            }
            memcpy(line, output, out_idx);
            nread = out_idx;
            line[nread] = '\0';
            free(output);
            free(arg);
        }
        printf("%s", line);
    }
    free(line);
    return 0;
}

