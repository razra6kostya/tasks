#include "word_pointer.h"

int max_letter(struct wordptr *tmp_word) 
{
    int max = 0;
    while (tmp_word) {
        if (tmp_word->count_letter > max) {
            max = tmp_word->count_letter;
        }
        tmp_word= tmp_word->next;
    }
    return max;
}

void print_vertical(struct listptr *list, int max_count_letter) 
{
    int i, j;
    int count_word = list->count_word;
    for (i=0; i<max_count_letter; i++) {
        struct wordptr *target_word = list->first;
        for(j=0; j<count_word; j++){
            if (target_word->first_char) {
                struct charptr **pp = &target_word->first_char;
                struct charptr *del_ch = target_word->first_char;
                putchar(target_word->first_char->letter);
                *pp = (*pp)->next;
                free(del_ch);
            } else {
                putchar(32);
            }
            target_word = target_word->next;
        }
        putchar(10);
    }
}
