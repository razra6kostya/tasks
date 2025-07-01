#include "word_list.h"
#ifndef WLIST_MANAGER 
#define WLIST_MANAGER


Word_list *init_list();
void free_word_list(Word_list *wlst);
void get_character_to_word(Word_list *wlst, const int ch);

#endif
