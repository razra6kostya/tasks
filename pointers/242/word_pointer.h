#include <stdio.h>
#include <stdlib.h>
#ifndef WORD_POINTER
#define WORD_POINTER

enum begin_end { 
    continues, begin_string, end_string, begin_word, end_word 
};

struct charptr {
    int c;
    struct charptr *next;
};

struct wordptr {
    struct charptr *word;
    struct wordptr *next;
};

int is_letter(int let);
enum begin_end situation(int pl, int l);
void print_reverse(struct wordptr *first);
void cleanup(struct wordptr *first);

#endif
