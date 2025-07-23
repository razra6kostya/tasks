#include "mem_utils.h"

#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Fatal error: memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void *safe_realloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        fprintf(stderr, "Fatal error: memory reallocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

char *safe_strdup(const char *s) {
    if (s == NULL) {
        fprintf(stderr, "Fatal error: safe_strdup received NULL input.\n");
        exit(EXIT_FAILURE);
    }
    size_t len = strlen(s);
    char *new_s = (char *)safe_malloc(len + 1);
    strcpy(new_s, s);
    return new_s;
}

void safe_free(void *ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}
