#include "word_pointer.h"

int is_letter(int let)
{
    return (let >= 'A' && let <= 'Z') || (let >= 'a' && let <= 'z');
}

enum begin_end situation(int pl, int l)
{
    if (is_letter(pl) && is_letter(l)) {
        return continues;
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
    return continues;
}

void print_reverse(struct wordptr *first)
{
    while (first) {
        while (first->word) {
            putchar(first->word->c);
            first->word = first->word->next;
        }
        first = first->next;
    }
}

void cleanup(struct wordptr *first)
{
    while (first) {
        while (first->word) {
            struct charptr *tmp = first->word;
            first->word = first->word->next;
            free(tmp);
        }
        struct wordptr *tmpw = first;
        first = first->next;
        free(tmpw);
    }
}

