#include <stdio.h>
#include <stdlib.h>
#include "word_pointer.h"

int main()
{
    struct wordptr *first = NULL, *tmpw;
    struct charptr *curc = NULL;
    int prev_let = '\n', let, spc;
    while ((let = getchar()) != EOF) {

        switch(situation(prev_let, let)) {
        case begin_string:
            spc = '\n';
            /* first word -> last word ' ' -> '\n' */
        case begin_word:
            tmpw = malloc(sizeof(struct wordptr));
            tmpw->word = NULL;
            curc = tmpw->word;
            tmpw->next = first;
            first = tmpw;
        case continues:
            if(!curc) {
                curc = malloc(sizeof(struct charptr));
                first->word = curc;
            } else {
                curc->next = malloc(sizeof(struct charptr));
                curc = curc->next;
            }
            curc->c = let;
            curc->next = NULL;
            break;
        case end_word:
            curc->next = malloc(sizeof(struct charptr));
            curc->next->c = spc;
            curc->next->next = NULL;
            spc = ' ';
            break;
        case end_string:
            curc->next = malloc(sizeof(struct charptr));
            curc->next->c = spc;
            curc->next->next = NULL;
            print_reverse(first);
            cleanup(first);
            first = NULL; 
            break;
        default:
            ;
        }

        prev_let = let;
    }
    /*if (let == EOF) {
        print_reverse(first);
        cleanup(first);
    }*/
    return 0;
}
