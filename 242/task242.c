#include <stdio.h>
#include "word_pointer.h"

int main()
{
    struct wordptr *line_text= NULL;
    struct charptr *letter_word = NULL;
    int prev_let = '\n', let;
    while ((let = getchar()) != EOF) {
        action(situation(prev_let, let));
    } 
    return 0;
}
