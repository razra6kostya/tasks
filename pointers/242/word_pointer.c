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

void print_reverse_and_cleanup(struct wordptr *cur)
{
    while (cur) {
        while (cur->word) {
            putchar(cur->word->c);
            struct charptr *tmp = cur->word;
            cur->word = cur->word->next;
            free(tmp);
        }
        struct wordptr *tmpw = cur;
        cur = cur->next;
        free(tmpw);
    }
}

