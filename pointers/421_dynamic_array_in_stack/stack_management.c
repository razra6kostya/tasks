#include "stack_management.h"

#define STACK_SIZE 6
#define MUL_SIZE 2

struct stack {
    double *data;      // array numbers
    int size;          // size array 
    int top;           // pointer to top of stack
};

Stack stackdbl_init()
{
    Stack st = NULL;
    st = malloc(sizeof(struct stack));
    st->size = STACK_SIZE;
    st->top = 0;
    st->data = malloc(st->size * sizeof(double));
    return st;
}

void stackdbl_destroy(Stack st)
{
    free(st->data);
    free(st);
    st = NULL;
}

static int stackdbl_empty(Stack st)
{
    return st->top == 0;
}

static Stack stackdbl_resize(Stack st)
{
    int i;
    double *tmp = malloc(st->size * sizeof(double));
    for (i = 0; i <= st->top; i++) {
        tmp[i] = st->data[i];
    }
    free(st->data);
    st->size *= MUL_SIZE;
    st->data = malloc(st->size * sizeof(double));
    for (i = 0; i <= st->top; i++) {
        st->data[i] = tmp[i];
    }
    free(tmp);
#ifdef DEBAG_PRINT
    printf("resize: %d\n", st->size);
#endif
    return st;
}

static Stack stackdbl_reducesize(Stack st)
{
    int i;
    st->size /= MUL_SIZE;
    double *tmp = malloc(st->size * sizeof(double));
    for (i = 0; i <= st->top; i++) {
        tmp[i] = st->data[i];
    }
    free(st->data);
    st->data = malloc(st->size * sizeof(double));
    for (i = 0; i <= st->top; i++) {
        st->data[i] = tmp[i];
    }
    free(tmp);
#ifdef DEBAG_PRINT
    printf("reduce size: %d\n", st->size);
#endif
    return st;
}

int stackdbl_push(Stack st, double n)
{
    if(st->size - 1 == st->top) {
        st = stackdbl_resize(st);
    }
    st->data[st->top++] = n;
    return 0;
}

double stackdbl_pop(Stack st)
{
    if(!stackdbl_empty(st)) {
        if(st->size >= (st->top + 1) * MUL_SIZE) {
#ifdef DEBAG_PRINT
    printf("check size: %d\n", st->size);
#endif
            st = stackdbl_reducesize(st);
        }
        return st->data[--st->top];
    }
    return 0;
}
