#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "word_pointer.h"

int main()
{
    int c;
    char letter, prev_letter = '\n';
    struct wordptr *first = NULL, *last = NULL, *temp_ptr;
    /* filling list text */ 
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
            int max_count = 0;
            temp_ptr = first;
            while (temp_ptr) {
                if (temp_ptr->count_letter > max_count) {
                    max_count = temp_ptr->count_letter;
                }
                temp_ptr = temp_ptr->next;
            }
            /* print */
            temp_ptr = first;
            while (temp_ptr) {
                while (temp_ptr->first_char) {
                    putchar(temp_ptr->first_char->letter);
                    temp_ptr->first_char = temp_ptr->first_char->next;
                }
                temp_ptr= temp_ptr->next;
            }
            /* cleanup */
            while (first) {
                while (first->first_char) {
                    struct charptr *tmp = first->first_char;
                    first->first_char = first->first_char->next;
                    free(tmp);
                }
                struct wordptr *tmp = first;
                first = first->next;
                free(tmp);
            }
            printf("%d\n", max_count);
        }
        prev_letter = letter;
    }
    return 0;
}
