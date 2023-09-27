#ifndef NUM_WORK

#define NUM_WORK 
struct num_list {
    long data;
    int count;
    struct num_list *next;
};

struct num_list *is_num_list(long n, struct num_list *ptr);
struct num_list *add_num_list(long n, struct num_list *last);
void show_num_list(struct num_list *ptr);
void clearup(struct num_list *ptr);

#endif
