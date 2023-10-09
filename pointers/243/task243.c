#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "word_pointer.h"

int main()
{
    int c;
    char letter, prev_letter = '\n';
    struct listptr *list;
    list = malloc(sizeof(struct listptr));
    /* filling list text */ 
    while ((c = getchar()) != EOF) {
        letter = (char)c;
        if (isspace(prev_letter) && isgraph(letter)) {
            if (prev_letter == '\n' && isgraph(letter)) {
                list->first = NULL;
            }
            if (!list->first) {
                list->first = malloc(sizeof(struct wordptr));
                list->last = list->first;
            } else {
                list->last->next = malloc(sizeof(struct wordptr));
                list->last = list->last->next;
            }
            list->count_word++;
            list->last->count_letter = 1;
            list->last->first_char = malloc(sizeof(struct charptr));
            list->last->first_char->letter = letter;
            list->last->first_char->next= NULL;
            list->last->last_char = list->last->first_char;
            list->last->next = NULL;
        }
        if (isgraph(prev_letter) && isgraph(letter)) {
            list->last->count_letter++;
            list->last->last_char->next = malloc(sizeof(struct charptr));
            list->last->last_char = list->last->last_char->next;
            list->last->last_char->letter = letter;
            list->last->last_char->next = NULL;
        }
        if (isgraph(prev_letter) && letter == '\n') {
            /* search long word */
            int max_count_letter = 0;
            struct wordptr *tmp_word = list->first;
            while (tmp_word) {
                if (tmp_word->count_letter > max_count_letter) {
                    max_count_letter = tmp_word->count_letter;
                }
                tmp_word= tmp_word->next;
            }
            /* print */
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
            /* cleanup */
            while (list->first) {
                while (list->first->first_char) {
                    struct charptr *tmp = list->first->first_char;
                    list->first->first_char = list->first->first_char->next;
                    free(tmp);
                }
                struct wordptr *tmp = list->first;
                list->first = list->first->next;
                free(tmp);
            }
        }
        prev_letter = letter;
    }
    free(list);
    return 0;
}
