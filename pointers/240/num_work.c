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

struct num_list *add_num_list(long n, struct num_list *last)
{
    if (!last) {
        last = malloc(sizeof(struct num_list)); 
    } else {
        last->next = malloc(sizeof(struct num_list)); 
        last = last->next;
    }
    last->data = n;
    last->count = 1;
    last->next = NULL;
    return last;
}

void show_num_list(struct num_list *ptr)
{
    int print_check = 0;
    while (ptr) {
        if(ptr->count == 3) {
            printf("[%ld]%s", ptr->data, " ");         
            print_check++;
        }
        ptr = ptr->next;
    }
    if(print_check) { 
        printf("\n");         
    }
}

void clearup(struct num_list *ptr)
{
    struct num_list *tmp;
    while(ptr) {
        tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }
}
