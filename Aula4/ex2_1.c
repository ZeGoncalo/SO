#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char* argv[]) {

    if (argc == 2) {
        char *original = argv[1];
        char *copy = strdup(original);

        for (int i = 0; copy[i] != '\0'; i++)
            copy[i] = tolower(copy[i]);

        printf("Original : %s\n", original);
        printf("Minúscula: %s\n", copy);

        free(copy);
        exit(EXIT_SUCCESS);
    }

    else if (argc == 3) {
        char *sub = argv[1];
        char *str = argv[2];

        char *found = strstr(str, sub);
        if (found != NULL)
            printf("'%s' ocorre em '%s'\n", sub, str);
        else
            printf("'%s' NÃO ocorre em '%s'\n", sub, str);

        /* contar número de ocorrências */
        int count = 0;
        char *pos = str;
        while ((pos = strstr(pos, sub)) != NULL) {
            count++;
            pos += strlen(sub); 
        }

        printf("'%s' ocorre %d vez(es) em '%s'\n", sub, count, str);
        exit(EXIT_SUCCESS);
    }

    else {
        printf("usage:\n");
        printf("  %s string             → converte para minúsculas\n", argv[0]);
        printf("  %s substring string   → verifica e conta ocorrências\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}
