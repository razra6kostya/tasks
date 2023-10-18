#ifndef STACK_MANAGEMENT
#define STACK_MANAGEMENT

struct stack {
    double *data;
    int size;
    int top;
};

int stackdbl_init(struct stack st);
int stackdbl_destroy(struct stack st );
int stackdbl_push(struct stack st);
int stackdbl_pop(struct stack st);
int stackdbl_empty(struct stack st);

#endif
