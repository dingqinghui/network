#include <stdlib.h>
#include <stdio.h>
#include "../include/zmemory.h"

#define PREFIX_SIZE sizeof(size_t)


static size_t used_memory = 0;

static void zmalloc_oom(size_t size) {
    fprintf(stderr, "zmalloc: Out of memory trying to allocate %zu bytes\n",
        size);
    fflush(stderr);
    abort();
}



void *zmalloc(size_t size) {
    void *ptr = malloc(size+PREFIX_SIZE);

    if (!ptr) zmalloc_oom(size);

    *((size_t*)ptr) = size;
    used_memory += size+PREFIX_SIZE;
    return (char*)ptr+PREFIX_SIZE;
}

void *zrealloc(void *ptr, size_t size) {

    void *realptr;

    size_t oldsize;
    void *newptr;

    if (ptr == NULL) return zmalloc(size);

    realptr = (char*)ptr-PREFIX_SIZE;
    oldsize = *((size_t*)realptr);
    newptr = realloc(realptr,size+PREFIX_SIZE);
    if (!newptr) zmalloc_oom(size);

    *((size_t*)newptr) = size;
    used_memory -= oldsize;
    used_memory += size;
    return (char*)newptr+PREFIX_SIZE;

}

void zfree(void *ptr) {

    void *realptr;
    size_t oldsize;

    if (ptr == NULL) return;

    realptr = (char*)ptr-PREFIX_SIZE;
    oldsize = *((size_t*)realptr);
    used_memory -= oldsize+PREFIX_SIZE;
    free(realptr);
}

char *zstrdup(const char *s) {
    size_t l = strlen(s)+1;
    char *p = zmalloc(l);

    memcpy(p,s,l);
    return p;
}

size_t zmalloc_used_memory(void) {
    return used_memory;
}