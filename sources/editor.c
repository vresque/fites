#include <fites/editor.h>
#include <stdio.h>
#include <stdlib.h>

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
}

void editor_open(char* path) {
    FILE* file = fopen(path, "r");
    if (!file) die("file does not exist");

    char* line = NULL;
    size_t cap = 0;
    ssize_t len;
    while ((len = getline(&line, &cap, file)) != -1) {
        while (len > 0 && (line[len - 1] == '\n' || line [len - 1] == '\r')) len--;
        editor_push_row(line, len);
    }
    free(line);
    fclose(file);
}
