#include <stdio.h>
#include <stdlib.h>
#ifndef STACK_MANAGEMENT
#define STACK_MANAGEMENT

typedef struct stack *Stack;

Stack stackdbl_init();
void stackdbl_destroy(Stack);
int stackdbl_push(Stack, double);
double stackdbl_pop(Stack);

#endif
