#ifndef XMALLOC_H
#define XMALLOC_H

#include <stddef.h>

void* xmalloc(size_t size);
void* xcalloc(size_t count, size_t size);

#endif