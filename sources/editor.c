#include <fites/editor.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <fites/term.h>
#include <fites/input.h>

void editor_delete_row(int loc) {
    if (loc < 0 || loc >= state_r().text_row_count) return;
    text_row_drop(&state_w()->text[loc]);
    memmove(&state_w()->text[loc], &state_r().text[loc + 1], sizeof(struct text_row) * (state_r().rows - loc - 1));
    state_w()->rows--;
    state_w()->rows++;
}


void editor_save() {
    if (state_r().filename == NULL) {
        state_w()->filename = editor_prompt("Enter filename for new file: %s (ESC to cancel)");
        if (state_r().filename == NULL) {
            editor_set_status("Save aborted!");
            return;
        }
    }

    int len;
    char* buf = text_rows_to_string(&len);

    int fd = open(state_r().filename, O_RDWR | O_CREAT, 0644);

    if (fd != -1) {
        if (ftruncate(fd, len) != -1) {
            if (write(fd, buf, len) != -1) {
                close(fd);
                free(buf);
                editor_set_status("Saved %s; %d bytes written", state_r().filename, len);
                state_w()->buffer_is_dirty = 0;
                return;
            }
        }
        close(fd);
    }
    free(buf);
    editor_set_status("Unable to save %s! I/O error: %s", state_r().filename, strerror(errno));
}
void editor_insert_char(int chr) {
    if (!(chr >= 0x20 && chr <= 0x7e)) return;


    if (state_r().cursor_y == state_r().text_row_count) {
        editor_push_row(state_r().text_row_count, "", 0);
    }
    text_row_insert_char(&state_w()->text[state_r().cursor_y], state_r().cursor_x, chr);
    state_w()->cursor_x++;
    state_w()->buffer_is_dirty++;

}

void editor_delete_char() {
    if (state_r().cursor_y == state_r().rows) return;
    if (state_r().cursor_x == 0 && state_r().cursor_y == 0) return;

    struct text_row* row = &state_w()->text[state_r().cursor_y];
    if (state_r().cursor_x > 0) {
        text_row_delete_char(row, state_r().cursor_x - 1);
        state_w()->cursor_x--;
    } else {
        state_w()->cursor_x = state_r().text[state_r().cursor_y - 1].size;
        text_row_append_string(&state_w()->text[state_r().cursor_y - 1], row->buffer, row->size);
        editor_delete_row(state_r().cursor_y);
        state_w()->cursor_y--;
    }
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

void editor_push_row(int loc, char* content, size_t len) {
    if (loc < 0 || loc > state_r().text_row_count) return;

    state_w()->text = realloc(state_w()->text, sizeof(struct text_row) * (state_r().text_row_count + 1));
    memmove(&state_w()->text[loc + 1], &state_w()->text[loc], sizeof(struct text_row) * (state_r().text_row_count - loc));

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

void editor_insert_new_line() {
    if (state_r().cursor_x == 0) {
        editor_push_row(state_r().cursor_y, "", 0);
    } else {
        struct text_row* row = &state_w()->text[state_r().cursor_y];
        editor_push_row(state_r().cursor_y + 1, &row->buffer[state_r().cursor_x], row->size - state_r().cursor_x);
        row = &state_w()->text[state_r().cursor_y];
        row->size = state_r().cursor_x;
        row->buffer[row->size] = '\0';
        editor_update_row(row);
    }
    state_w()->cursor_y++;
    state_w()->cursor_x = 0;
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
        editor_push_row(state_r().text_row_count, line, len);
    }
    free(line);
    fclose(file);

    state_w()->buffer_is_dirty = 0;
}

char* editor_prompt(char* prompt) {
    size_t bufsize = 128;
    char* buf = malloc(bufsize);

    size_t buflen = 0;
    buf[0] = '\0';

    while (1) {
        editor_set_status(prompt, buf);
        term_loop();

        int c = input_read_key();
        
        if (c == KEY_DELETE || c == 127) {
            if (buflen != 0) buf[--buflen] = '\0';
        } else if (c == '\x1b') {
            editor_set_status("");
            free(buf);
            return NULL;
        } else if (c == '\r') {
            if (buflen != 0) {
                editor_set_status("");
                return buf;
            }
        } else if (!iscntrl(c) && c < 128) {
            if (buflen == bufsize - 1) {
                bufsize *= 2;
                buf = realloc(buf, bufsize);
            }
            buf[buflen++] = c;
            buf[buflen] = '\0';
        }
    }
}

char* editor_prompt_with_callback(char* prompt, editor_prompt_callback_t callback) {
    size_t bufsize = 128;
    char* buf = malloc(bufsize);

    size_t buflen = 0;
    buf[0] = '\0';

    while (1) {
        editor_set_status(prompt, buf);
        term_loop();

        int c = input_read_key();
        
        if (c == KEY_DELETE || c == 127) {
            if (buflen != 0) buf[--buflen] = '\0';
        } else if (c == '\x1b') {
            editor_set_status("");
            if (callback) callback(buf, c);
            free(buf);
            return NULL;
        } else if (c == '\r') {
            if (buflen != 0) {
                editor_set_status("");
                if (callback) callback(buf, c);
                return buf;
            }
        } else if (!iscntrl(c) && c < 128) {
            if (buflen == bufsize - 1) {
                bufsize *= 2;
                buf = realloc(buf, bufsize);
            }
            buf[buflen++] = c;
            buf[buflen] = '\0';
        }

        if (callback) callback(buf, c);
    }
}