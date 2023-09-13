#include <stdio.h>
#include <stdlib.h>
#include "num_work.h"

struct num_list *is_num_list(long n, struct num_list *ptr)
{
    while (ptr) {
        if (ptr->data == n) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

struct num_list *add_num_list(long n, struct num_list *ptr)
{
    struct num_list *first = NULL, *tmp;
    tmp = malloc(sizeof(struct num_list)); 
    tmp->data = n;
    tmp->count = 1;
    tmp->next = ptr;
    first = tmp;
    return first;
}

void show_num_list(struct num_list *ptr)
{
    while (ptr) {
        printf("c:%ld\n", ptr->data);         
        ptr = ptr->next;
    }
}
