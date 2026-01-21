#include "list.h"
#include <stdlib.h> 
void list_remove_last(list *l) {
    if (l->size == 0) return;
    if (l->size == 1) {
        free(l->first);
        l->first = NULL;
    } else {
        node* p = l->first;
        while (p->next->next != NULL)
            p = p->next;
        free(p->next);
        p->next = NULL;
    }
    l->size--;
}
