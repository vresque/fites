#include <fites/highlighter.h>
#include <ctype.h>
#include <lang.h>

int highlighter_to_color(enum highlighter highlighter) {
	switch (highlighter) {
		case HL_NUMBER: return COLOR_NUMBER;
		case HL_NORMAL: return COLOR_NORMAL;
		case HL_MATCH: return COLOR_MATCH;
		default: return COLOR_NORMAL;
	}
	return -1;
}

int is_separator(int c) {
	return isspace(c) || c == '\0' || strchr(",.()#-/*=~%<>[];", c) != NULL;
}

void highlighter_update_syntax(struct text_row* row) {
	row->highlight = realloc(row->highlight, row->rendered_size);
	memset(row->highlight, HL_NORMAL, row->rendered_size);

	int last_separator = 1;
	int i = 0;
	while (i < row->rendered_size) {
		char this = row->rendered[i];
		unsigned char last_highlight = (i > 0) ? row->highlight[i - 1] : HL_NORMAL;


		if (
			isdigit(this) && 
			((
				last_separator ||
				last_highlight == HL_NUMBER
			) ||
			(
				(this == '.' || this == '_')&&
				last_highlight == HL_NUMBER
			))) {

			row->highlight[i] = HL_NUMBER;
			i++;
			last_separator = 0;
			continue;
		}
		last_separator = is_separator(this);
		i++;
	}
}