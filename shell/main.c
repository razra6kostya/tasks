#include "wlist_manager.h"
#include <stdio.h>

int main()
{
    int ch;
    Word_list *word_list = init_list();

    while ((ch = getchar()) != EOF) {
        get_character_to_word(word_list, ch);
        if (ch == '\n') {
            free_word_list(word_list);
            word_list = init_list();
        }
    }
    free_word_list(word_list);
    return 0;
}
