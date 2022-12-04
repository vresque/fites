#include <fites/text.h>
#include <fites/editor.h>

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