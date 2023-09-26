#include "word_pointer.h"

int is_letter(int let)
{
    return (let >= 'A' && let <= 'Z') || (let >= 'a' && let <= 'z');
}

enum begin_end situation(int pl, int l)
{
    if(is_letter(pl) && is_letter(l)) {
        return word;
    }            
    if (pl == '\n' && is_letter(l)) {
        return begin_string;
    }
    if (is_letter(pl) && l == '\n') {
        return end_string;
    }
    if (pl == ' ' && is_letter(l)) {
        return begin_word;
    }
    if (is_letter(pl) && l == ' ') {
        return end_word;
    }
}

void action(enum begin_end x)
{
    switch(x) {
        case word:
            break;    
        case begin_string:
            break; 
        case end_string:
            print_reverse();
            clearup();
            break;
        default:
            ;
    }
}
