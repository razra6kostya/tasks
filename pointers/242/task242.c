#include <stdio.h>
#include "word_pointer.h"

int main()
{
    struct wordptr *first = NULL;
    int prev_let = '\n', let;
    while ((let = getchar()) != EOF) {
        ;
        switch(situation(prev_let, let)) {
        case begin_string:
            first = new_string(first);
        case begin_word:
            break;
        case end_word:
            break;
        case end_string:
            break;
        default:
            ;
        }

        prev_let = let;
    }
    return 0;
}
