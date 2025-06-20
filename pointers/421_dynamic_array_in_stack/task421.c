#include <stdio.h>
#include <math.h>
#include "stack_management.h"

int main()
{
    int i;
    double n;
    Stack stack = stackdbl_init();
    for (i = 1; i < 30; i++) {
        stackdbl_push(stack, sin(i));
    }
    while ((n = stackdbl_pop(stack))) {
        printf("%g\n", n);
    }
    stackdbl_destroy(stack);
    return 0;
}
