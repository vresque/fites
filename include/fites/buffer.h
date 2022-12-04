#ifndef BUFFER_H
#define BUFFER_H

#include "fites.h"
#include <string.h>

struct buffer {
    char* buf;
    int count;
};

#define BUFFER_INIT {NULL, 0}

void buffer_append(struct buffer* buf, const char* append, int count);
void buffer_drop(struct buffer* buf);
#endif
