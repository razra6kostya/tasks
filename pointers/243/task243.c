#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "word_pointer.h"

int main()
{
    int c;
    char letter, prev_letter = '\n';
    struct listptr *list = NULL;
    /* filling list text */ 
    while ((c = getchar()) != EOF) {
        letter = (char)c;
        if (isspace(prev_letter) && isgraph(letter)) {
            if (prev_letter == '\n' && isgraph(letter)) {
                list = malloc(sizeof(struct listptr));
                list->first = NULL;
            }
            if (!list->first) {
                list->first = malloc(sizeof(struct wordptr));
                list->count_word = 0;
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
            int max_count_letter = max_letter(list->first);
            /* print */
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
            /* cleanup */
            while (list->first) {
                struct wordptr *tmp = list->first;
                list->first = list->first->next;
                free(tmp);
            }
            free(list);
            list = NULL;
        }
        prev_letter = letter;
    }
    return 0;
}
