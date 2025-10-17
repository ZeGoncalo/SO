#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <time.h>

int main() {
    srand(time(NULL));  // para list_new_random

    list* l1 = list_new();
    list_add_first(10, l1);
    list_add_last(20, l1);
    list_add_first(5, l1);

    list_print(l1);
    printf("Primeiro: %d\n", list_get_first(l1));
    printf("Último: %d\n", list_get_last(l1));

    list_remove_first(l1);
    list_remove_last(l1);
    list_print(l1);
    printf("Tamanho: %d\n", list_size(l1));

    list* l2 = list_new_random(5, 100);
    list_print(l2);

    return 0;
}
