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
