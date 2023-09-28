#include <stdio.h>
#include <stdlib.h>
#include "word_pointer.h"

int main()
{
    struct wordptr *first = NULL, *last = NULL, *tmpw;
    struct charptr *tmp, *pcur;
    int prev_let = '\n', let;
    while ((let = getchar()) != EOF) {
        switch(situation(prev_let, let)) {
        case begin_string:
            first = malloc(sizeof(struct wordptr));
            first->next = NULL;
            first->word = malloc(sizeof(struct charptr));
            first->word->c = let;
            first->word->next = NULL;
            last = first;
            pcur = last->word;
            break;
        case continues:
            tmp = malloc(sizeof(struct charptr));
            tmp->c = let;
            tmp->next = NULL;
            pcur->next = tmp;
            pcur = pcur->next;
            break;
        case begin_word:
            tmpw = malloc(sizeof(struct wordptr));
            tmpw->word = malloc(sizeof(struct charptr));
            tmpw->word->c = let;
            tmpw->word->next = NULL;
            tmpw->next = first;
            first = tmpw;
            pcur = first->word;
            break;
        case end_word:
            tmp = malloc(sizeof(struct charptr));
            tmp->c = ' ';
            tmp->next = NULL;
            pcur->next = tmp;
            pcur = pcur->next;
            break;
        case end_string:
            tmp = malloc(sizeof(struct charptr));
            tmp->c = '\n';
            tmp->next = NULL;
            pcur->next = tmp;
            pcur = pcur->next;
            /*print_reverse(first);
            cleanup(first);*/
            break;
        default:
            ;
        }

        prev_let = let;
    }
    print_reverse(first);
    return 0;
}
