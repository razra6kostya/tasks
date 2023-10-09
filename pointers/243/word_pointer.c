#include "word_pointer.h"

void print_vertical(struct listptr *list, int max_count_letter) 
{
    int i, j;
    int count_word = list->count_word;
    for (i=0; i<max_count_letter; i++) {
        struct wordptr *target_word = list->first;
        for(j=0; j<count_word; j++){
            if (target_word->first_char) {
                putchar(target_word->first_char->letter);
                target_word->first_char = target_word->first_char->next;
            } else {
                putchar(32);
            }
            target_word = target_word->next;
        }
        putchar(10);
    }
}
