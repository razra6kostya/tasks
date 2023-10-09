#include <stdio.h>
#include <stdlib.h>
#ifndef WORD_POINTER
#define WORD_POINTER

struct listptr {
    struct wordptr *first;
    struct wordptr *last;
    int count_word;
};

struct charptr {
    char letter;
    struct charptr *next;
};

struct wordptr {
    struct charptr *first_char;
    struct charptr *last_char;
    int count_letter;
    struct wordptr *next;
};

#endif
