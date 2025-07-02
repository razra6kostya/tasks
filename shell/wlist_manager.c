#include "wlist_manager.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define UNMATH_QUOTE 2;

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

static void print_prompt()
{
    printf("> ");
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
    print_prompt();
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

void free_word_list(Word_list *wlst)
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

}

static void print_list(Word_item *first_item)
{
    while (first_item) {
        printf("[%s]\n", first_item->word);
        first_item = first_item->next;
    }
}

static Mode change_anal_mode(Mode mode)
{
    return mode == NORMAL ? INSIDE : NORMAL;
}

static int switching_mode(Analysis_mode *a_mode, const int ch)
{
    if (ch == '"') {
        a_mode->mode = 
            change_anal_mode(a_mode->mode);
        a_mode->quote_count++;
        return 1;
    } 
    return 0;
}

static int buf_is_full(Buf_word_for_list *wfl)
{
    return wfl->cap_len_word - 1 == 
        wfl->lenght_word;
}

static void increase_word_buffer(Buf_word_for_list *wfl)
{
    if(buf_is_full(wfl)) {
        wfl->cap_len_word *= 2;
        char *new_word = 
            realloc(wfl->b_word, wfl->cap_len_word);
        if (!new_word) {
            free(wfl->b_word);
            return;
        }
        wfl->b_word =  new_word;
    }
}

static void add_word_in_list(Word_list *wlst)
{
    if (wlst->first_item == NULL) {
        wlst->first_item = malloc(sizeof(Word_item));
        wlst->last_item = wlst->first_item;
    } else {
        wlst->last_item->next = malloc(sizeof(Word_item));
        wlst->last_item = wlst->last_item->next;
    }
    wlst->last_item->word = strdup(wlst->buf_word->b_word);
    wlst->last_item->next = NULL; 
}

static void finalize_word(Word_list *wlst)
{
    wlst->buf_word->b_word[wlst->buf_word->lenght_word] = '\0';
    add_word_in_list(wlst);
    wlst->buf_word->lenght_word = 0;
}

static int progress_space(const int prev_ch, const int ch)
{
    return isspace(prev_ch) && isspace(ch);
}
    
static int handle_end_word(Word_list *wlst, const int ch) 
{
    if (isspace(ch)) {
        switch (wlst->anal_mode->mode) {
        case NORMAL:
            if (progress_space(wlst->buf_word->prev_ch, ch)) {
                /* this word after INSIDE mode */
                if (wlst->buf_word->lenght_word > 0) {
                    if (wlst->buf_word->b_word[wlst->buf_word->lenght_word - 1] == ' ') {
                        ;
                    }
                } else {
                    return 1;
                }
            }
            finalize_word(wlst);
            return 1; 
        case INSIDE:
            if (ch == '\n') {
                finalize_word(wlst);
                return 1;
            }
            wlst->buf_word->b_word[wlst->buf_word->lenght_word] = ch;
            break;
        }
    }
    return 0;
}

void get_character_to_word(Word_list *wlst, const int ch)
{
    /* ch == '"'*/
    if (switching_mode(wlst->anal_mode, ch)) 
        return;

    increase_word_buffer(wlst->buf_word);

    if (handle_end_word(wlst, ch)) {
        wlst->buf_word->prev_ch = ch;
        return;
    }

    /* normal character */
    wlst->buf_word->b_word[wlst->buf_word->lenght_word] = ch;
    wlst->buf_word->lenght_word++;
    wlst->buf_word->prev_ch = ch;
}
