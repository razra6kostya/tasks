#include <stdio.h>
#include "num_work.h"

int main()
{
    struct num_list *first, *ptr;
    first = NULL;
    ptr = NULL;
    int i, res = 0;

    while((i = getchar()) != EOF) {
        if(i > '0' && i < '9') {
            res = res * 10 + i - '0';
        } else {
            break;
        }
/*        if ((ptr = is_num_list(i, first))) {
            ptr->count++;
        } else {
        */
/*        }*/
    }
    first = add_num_list(res, first);
    show_num_list(first);
    return 0;
}
