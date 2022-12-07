#include <fites/highlighter.h>
#include <ctype.h>

int highlighter_to_color(enum highlighter highlighter) {
	switch (highlighter) {
		case HL_NUMBER: return COLOR_NUMBER;
		case HL_NORMAL: return COLOR_NORMAL;
		default: return COLOR_NORMAL;
	}
	return -1;
}

void highlighter_update_syntax(struct text_row* row) {
	row->highlight = realloc(row->highlight, row->rendered_size);
	memset(row->highlight, HL_NORMAL, row->rendered_size);

	int i;
	for (i = 0; i < row->rendered_size; i++) {
		char this = row->rendered[i];
		if (isdigit(this)) {
			row->highlight[i] = HL_NUMBER;
		}
	}
}