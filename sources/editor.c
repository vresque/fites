#include <fites/editor.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>

void editor_save() {
    if (state_r().filename == NULL) return;

    int len;
    char* buf = text_rows_to_string(&len);

    int fd = open(state_r().filename, O_RDWR | O_CREAT, 0644);

    if (fd != -1) {
        if (ftruncate(fd, len) != -1) {
            if (write(fd, buf, len) != -1) {
                close(fd);
                free(buf);
                editor_set_status("Saved %s; %d bytes written", state_r().filename, len);
                return;
            }
        }
        close(fd);
    }
    free(buf);
    editor_set_status("Unable to save %s! I/O error: %s", state_r().filename, strerror(errno));
    state_w()->buffer_is_dirty = 0;
}
void editor_insert_char(int chr) {
    if (!(chr >= 0x20 && chr <= 0x7e)) return;


    if (state_r().cursor_y == state_r().text_row_count) {
        editor_push_row("", 0);
    }
    text_row_insert_char(&state_w()->text[state_r().cursor_y], state_r().cursor_x, chr);
    state_w()->cursor_x++;
    state_w()->buffer_is_dirty++;

}


void editor_set_status(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(state_w()->status, sizeof(state_r().status), fmt, ap);
    va_end(ap);
    state_w()->status_time = time(NULL);
}

void editor_update_row(struct text_row* row) {
    int j = 0;
    int index = 0;
    int tabs_rendered = 0;

    for (j = 0; j < row->size; j++) {
        if (row->buffer[j] == '\t') tabs_rendered++;
    }

    free(row->rendered);
    row->rendered = malloc(row->size + (tabs_rendered * (TAB_SIZE - 1)) + 1);

    for (j = 0; j < row->size; j++) {
        if (row->buffer[j] == '\t') {
            row->rendered[index++] = ' ';
            while (index % TAB_SIZE != 0) row->rendered[index++] = ' ';
        } else {
            row->rendered[index++] = row->buffer[j];
        }
    }
    row->rendered[index] = '\0';
    row->rendered_size = index;
}

void editor_push_row(char* content, size_t len) {
    state_w()->text = realloc(state_w()->text, sizeof(struct text_row) * (state_r().text_row_count + 1));

    int loc = state_r().text_row_count;
    state_w()->text[loc].size = len;
    state_w()->text[loc].buffer = malloc(len + 1);
    memcpy(state_w()->text[loc].buffer, content, len);
    state_w()->text[loc].buffer[len] = '\0';

    state_w()->text[loc].rendered_size = 0;
    state_w()->text[loc].rendered = NULL;
    editor_update_row(&state_w()->text[loc]);


    state_w()->text_row_count++;
    state_w()->buffer_is_dirty++;
}

void editor_open(char* path) {
    free(state_w()->filename);
    state_w()->filename = strdup(path);



    FILE* file = fopen(path, "r");
    if (!file) {
        file = fopen(path, "r");
        editor_set_status("Created '%s' as it did not previously exist.", path);
        if (!file) die("not able to open file");
    }

    char* line = NULL;
    size_t cap = 0;
    ssize_t len;
    while ((len = getline(&line, &cap, file)) != -1) {
        while (len > 0 && (line[len - 1] == '\n' || line [len - 1] == '\r')) len--;
        editor_push_row(line, len);
    }
    free(line);
    fclose(file);

    state_w()->buffer_is_dirty = 0;
}
