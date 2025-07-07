#include "struct_data.h"
#include "lexer.h"
#include "run_cmd.h"
#include <stdio.h>

int main()
{
    int ch;
    Word_item *cmdlist = NULL;
    Word_list *word_list = NULL;

    while ((ch = getchar()) != EOF) {
        if (!word_list) {
            word_list = init_list();
            printf("> "); 
        }
        get_character_to_word(word_list, ch);
        if (ch == '\n') {
            if (quote_count(word_list)) {
                cmdlist = word_list->first_item;
                exec_cmd(cmdlist);
            } else {
                fprintf(stdout, "Error: unmatched quotes\n");
            }
            word_list = free_word_list(word_list);
        }
    }
    return 0;
}
