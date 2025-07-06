#include "parser.h"
#include "struct_data.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define UNMATH_QUOTE 2;

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

int quote_count(Word_list *wlst)
{
    return wlst->anal_mode->quote_count % 2 == 0;
}
