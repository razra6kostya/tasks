#include <stdio.h>

struct num_list {
    int data;
    int count;
    struct num_list *next;
};

struct num_list *is_num_list(int n, struct num_list *ptr)
{
    return ptr; 
}

struct num_list *add_num_list(int n, struct num_list *ptr)
{
    return ptr;
}

void show_num_list(struct num_list *ptr)
{
}

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
    show_num_list(first);
    return 0;
}
