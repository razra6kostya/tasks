#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct listptr {
    struct wordptr *first;
};

struct charptr {
    char letter;
    struct charptr *next;
};

struct wordptr {
    struct charptr *first_char;
    struct charptr *last_char;
    struct wordptr *next;
};

int main()
{
    int c;
    char letter, prev_letter = '\n', spc = '\n';
    struct listptr *list = NULL;
    /* filling list text */ 
    while ((c = getchar()) != EOF) {
        letter = (char)c;
        /* begin word*/
        if (isspace(prev_letter) && isgraph(letter)) {
            /* begin string */
            if (prev_letter == '\n') {
                list = malloc(sizeof(struct listptr));
                list->first = NULL;
                spc = '\n';
            }
            struct wordptr *tmp = malloc(sizeof(struct wordptr));
            tmp->first_char = tmp->last_char = NULL;
            tmp->next = list->first;
            list->first = tmp;
                
            list->first->first_char = malloc(sizeof(struct charptr));
            list->first->first_char->letter = letter;
            list->first->first_char->next= NULL;
            list->first->last_char = list->first->first_char;
        }
        if (isgraph(prev_letter) && isgraph(letter)) {
            list->first->last_char->next = malloc(sizeof(struct charptr));
            list->first->last_char = list->first->last_char->next;
            list->first->last_char->letter = letter;
            list->first->last_char->next = NULL;
        }
        if (isgraph(prev_letter) && isspace(letter)) {
            list->first->last_char->next = malloc(sizeof(struct charptr));
            list->first->last_char = list->first->last_char->next;
            list->first->last_char->letter = spc;
            list->first->last_char->next = NULL;
            spc = ' ';
            if(letter == '\n') {
                while (list->first) {
                    while (list->first->first_char) {
                        putchar(list->first->first_char->letter);
                        struct charptr *tmp_ch = list->first->first_char;
                        list->first->first_char = list->first->first_char->next;
                        free(tmp_ch);
                    }
                    struct wordptr *tmp_w = list->first;
                    list->first = list->first->next;
                    free(tmp_w);
                }
                free(list);
                } 
        }
        prev_letter = letter;
        }
    return 0;
}
