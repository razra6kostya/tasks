#include <stdio.h>
#include <stdlib.h>
#ifndef STACK_MANAGEMENT
#define STACK_MANAGEMENT
#define STACK_SIZE 6
#define MUL_SIZE 2

struct stack {
    double *data;      // array numbers
    int size;          // size array 
    int top;           // pointer to top of stack
};

struct stack *stackdbl_init();
void stackdbl_destroy(struct stack *st);
int stackdbl_empty(struct stack *st);
struct stack *stackdbl_resize(struct stack *st);
int stackdbl_push(struct stack *st, double n);
double stackdbl_pop(struct stack *st);

#endif
