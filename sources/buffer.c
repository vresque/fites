#include <fites/buffer.h>

void buffer_append(struct buffer* buf, const char* append, int count) {
    char* new = realloc(buf->buf, buf->count + count);

    if (new == NULL) return;
    memcpy(&new[buf->count], append, count);
    buf->buf = new;
    buf->count += count;
}

void buffer_drop(struct buffer* buf) {
    free(buf->buf);
}
