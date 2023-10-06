#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "word_pointer.h"

int main()
{
    int c;
    char letter, prev_letter = '\n';
    struct wordptr *first = NULL, *last = NULL, *temp_ptr;
    
    while ((c = getchar()) != EOF) {
        letter = (char)c;
        if (isspace(prev_letter) && isgraph(letter)) {
            if (prev_letter == '\n' && isgraph(letter)) {
                first = NULL;
            }
            if (!first) {
                first = malloc(sizeof(struct wordptr));
                last = first;
            } else {
                last->next = malloc(sizeof(struct wordptr));
                last = last->next;
            }
            last->count_letter = 1;
            last->first_char = malloc(sizeof(struct charptr));
            last->first_char->letter = letter;
            last->first_char->next= NULL;
            last->last_char = last->first_char;
            last->next = NULL;
        }
        if (isgraph(prev_letter) && isgraph(letter)) {
            last->count_letter++;
            last->last_char->next = malloc(sizeof(struct charptr));
            last->last_char = last->last_char->next;
            last->last_char->letter = letter;
            last->last_char->next = NULL;
        }
        if (isgraph(prev_letter) && letter == '\n') {
            /* search long word */
            temp_ptr = first;
            while (temp_ptr) {
                temp_ptr = temp_ptr->next;
            }
            while (first) {
                while (first->first_char) {
                    putchar(first->first_char->letter);
                    struct charptr *tmp = first->first_char;
                    first->first_char = first->first_char->next;
                    free(tmp);
                }
                printf("%d", first->count_letter);
                struct wordptr *tmp = first;
                first = first->next;
                free(tmp);
            }
        }
        prev_letter = letter;
    }
    return 0;
}
