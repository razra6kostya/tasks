#ifndef MEM_UTILS_H
#define MEM_UTILS_H

#include <stddef.h>

void *safe_malloc(size_t size);
void *safe_realloc(void *ptr, size_t size);
char *safe_strdup(const char *s);
void safe_free(void *ptr);

#endif
