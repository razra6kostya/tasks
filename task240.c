#include <stdio.h>
#include "mod.h"

struct num_list {
    int data;
    int count;
    struct num_list *next;
};

int main()
{
    struct num_list *first, *ptr;
    first = NULL;
    ptr = NULL;
    int i;
    while((i = getchar()) != EOF) {
        if ((ptr = is_num_list(i, first))) {
            ptr->count++;
        } else {
            add_num_list(i, first);
        }
    }
    show_num_list(first);
    return 0;
}
