#include <stdio.h>
#include "num_work.h"

int main()
{
    struct num_list *first, *ptr, *last;
    first = last = ptr = NULL;
    long res = 0;
    int i;
    while((i = getchar()) != EOF) {
        if(i > '0' && i < '9') {
            res = res * 10 + i - '0';
        } else {
            if ((ptr = is_num_list(res, first))) {
                ptr->count++;
            } else {
                first = add_num_list(res, first);
                res = 0;
            }
        }
    }
    show_num_list(first);
    return 0;
}
