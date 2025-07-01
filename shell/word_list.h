#ifndef WORD_LIST 
#define WORD_LIST

typedef enum mode {
    NORMAL,
    INSIDE
} Mode;

enum { DEF_WORD_BUF_SIZE = 10 };

typedef struct word_item {
    char *word;
    struct word_item *next;
} Word_item;

typedef struct analysis_mode {
    Mode mode;
    int quote_count;
} Analysis_mode;

typedef struct buf_word_for_list {
    char* b_word;
    int lenght_word;
    int cap_len_word;
    int prev_ch;      /* to track a serias spaces */
} Buf_word_for_list;

typedef struct word_list  {
    Word_item *first_item;
    Word_item *last_item;
    Buf_word_for_list *buf_word;
    Analysis_mode *anal_mode;
} Word_list;

#endif
