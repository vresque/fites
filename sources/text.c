#include <fites/text.h>
#include <fites/editor.h>
#include <fites/fites.h>

int text_rendered_x_to_cursor_x(struct text_row* row, int ren_x) {
	int cur_rx = 0;
	int cursor_x;
	for (cursor_x = 0; cursor_x < row->size; cursor_x++) {
		if (row->buffer[cursor_x] == '\t')
			cur_rx += (TAB_SIZE - 1) - (cur_rx % TAB_SIZE);
		cur_rx++;

		if (cur_rx > ren_x) return cursor_x;
	}
	return cursor_x;
}

void text_row_append_string(struct text_row* row, char* string, size_t length) {
	row->buffer = realloc(row->buffer, row->size + length + 1);
	memcpy(&row->buffer[row->size], string, length);
	row->size += length;
	row->buffer[row->size] = '\0';
	editor_update_row(row);
	state_w()->buffer_is_dirty++;
}

void text_row_delete_char(struct text_row* row, int loc) {
    if (loc < 0 || loc >= row->size) return;
    memmove(&row->buffer[loc], &row->buffer[loc + 1], row->size - loc);
    row->size--;
    editor_update_row(row);
    state_w()->buffer_is_dirty++;
}

void text_row_insert_char(struct text_row* row, int loc, int chr) {
	if (loc < 0 || loc > row->size) loc = row->size;
	row->buffer = realloc(row->buffer, row->size + 2);
	memmove(&row->buffer[loc + 1], &row->buffer[loc], row->size - loc + 1);
	row->size++;
	row->buffer[loc] = chr;
	editor_update_row(row);
}

char* text_rows_to_string(int* len) {
	int total = 0;
	int j = 0;
	for (j = 0; j < state_r().text_row_count; j++) {
		total += state_r().text[j].size + 1;
	}
	*len = total;

	char* buf = malloc(total);
	char* p = buf;
	for (j = 0; j < state_r().text_row_count; j++) {
		memcpy(p, state_w()->text[j].buffer, state_w()->text[j].size);
		p += state_r().text[j].size;
		*p = '\n';
		p++;
	}

	return buf;
}


void text_row_drop(struct text_row* row) {
	free(row->rendered);
	free(row->buffer);
	free(row->highlight);
}