#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main() {
    list* l = list_new();

    printf("A adicionar elementos no início:\n");
    list_add_first(10, l);
    list_add_first(20, l);
    list_add_first(30, l);
    list_print(l);

    printf("\nA adicionar elementos no fim:\n");
    list_add_last(40, l);
    list_add_last(50, l);
    list_print(l);

    printf("\nPrimeiro elemento: %d\n", list_get_first(l));
    printf("Último elemento: %d\n", list_get_last(l));
    printf("Tamanho: %d\n", list_size(l));

    printf("\nA remover primeiro elemento...\n");
    list_remove_first(l);
    list_print(l);

    printf("\nA remover último elemento...\n");
    list_remove_last(l);
    list_print(l);

    printf("\nA criar lista aleatória com 5 elementos (0 a 99):\n");
    list* r = list_new_random(5, 100);
    list_print(r);

    return 0;
}
