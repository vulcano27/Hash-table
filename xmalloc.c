#include <stdio.h>
#include <stdlib.h>

#include "xmalloc.h"

static void die_oom(void) {
    fprintf(stderr, "fatal: out of memory\n");
    exit(EXIT_FAILURE);
}

void* xmalloc(size_t size) {
    void* p = malloc(size);
    if (p == NULL) die_oom();
    return p;
}

void* xcalloc(size_t count, size_t size) {
    void* p = calloc(count, size);
    if (p == NULL) die_oom();
    return p;
}