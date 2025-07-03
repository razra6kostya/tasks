#include "struct_data.h"
#include <stdlib.h>
#include <stdio.h>

static Analysis_mode *init_analysis_mode()
{
    Analysis_mode *a_mode = malloc(sizeof(Analysis_mode));
    *a_mode = (Analysis_mode) {
        .mode = NORMAL,
        .quote_count = 0
    };
    return a_mode;
}

static Word_item *init_item()
{
    Word_item *item = NULL;
    return item;
}

static Buf_word_for_list *init_word_for_list()
{
    Buf_word_for_list *wfl = malloc(sizeof(Buf_word_for_list));
    *wfl = (Buf_word_for_list){
        .prev_ch = ' ',
        .cap_len_word = DEF_WORD_BUF_SIZE,
        .lenght_word = 0,
        .b_word = malloc(sizeof(char) * DEF_WORD_BUF_SIZE)
    };
    return wfl;
}

Word_list *init_list()
{
    Word_list *wlst = malloc(sizeof(Word_list));
    *wlst = (Word_list) {
        .first_item = init_item(),
        .last_item = init_item(),
        .buf_word = init_word_for_list(),
        .anal_mode = init_analysis_mode()
    };
    return wlst;
}

static void free_item(Word_item *first_item)
{
    while (first_item) {
        Word_item *tmp = first_item;
        first_item = first_item->next;
        if (tmp->word) {
            free(tmp->word);
            tmp->word = NULL;
        }
        free(tmp);
    }
    first_item = NULL;
}

Word_list *free_word_list(Word_list *wlst)
{
    printf("FREE\n");
    free(wlst->buf_word->b_word);
    wlst->buf_word->b_word = NULL;
    free(wlst->buf_word);
    wlst->buf_word = NULL;
    free_item(wlst->first_item);
    free(wlst->anal_mode);
    wlst->anal_mode = NULL;
    free(wlst);
    wlst = NULL;
    return wlst;
}
