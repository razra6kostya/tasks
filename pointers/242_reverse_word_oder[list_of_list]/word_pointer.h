#ifndef WORD_POINTER
#define WORD_POINTER

struct listptr {
    struct wordptr *first;
};

struct charptr {
    char letter;
    struct charptr *next;
};

struct wordptr {
    struct charptr *first_char;
    struct charptr *last_char;
    struct wordptr *next;
};

#endif
