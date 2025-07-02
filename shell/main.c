#include "word_list.h"
#include "wlist_manager.h"
#include "cmd_manager.h"
#include <stdio.h>

int main()
{
    int ch;
    Word_item *word_item_list;
    Word_list *word_list = init_list();

    while ((ch = getchar()) != EOF) {
        get_character_to_word(word_list, ch);
        if (ch == '\n') {
            if (word_list->anal_mode->quote_count % 2 == 0) { 
                word_item_list = word_list->first_item;
                exec_cmd(word_item_list);
            } else {
                fprintf(stdout, "Error: unmatched quotes\n");
            }
            free_word_list(word_list);
            word_list = init_list();
        }
    }
    free_word_list(word_list);
    return 0;
}
