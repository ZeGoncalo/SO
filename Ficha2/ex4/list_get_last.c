#include "list.h"
#include <stdlib.h> 
int list_get_last(list *l) {
    node* p = l->first;
    while (p->next != NULL)
        p = p->next;
    return p->val;
}

