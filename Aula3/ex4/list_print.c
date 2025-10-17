#include "list.h"
#include <stdio.h>

void list_print(list* l) {
    node* p = l->first;
    printf("[");
    while (p != NULL) {
        printf("%d", p->val);
        if (p->next != NULL)
            printf(", ");
        p = p->next;
    }
    printf("]\n");
}

