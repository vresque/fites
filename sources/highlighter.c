#include <fites/highlighter.h>
#include <ctype.h>
#include <lang.h>

void highlighter_select_syntax_highlight() {
	state_w()->current_syntax = NULL;
	if (state_r().filename == NULL) return;

	char* ext = strrchr(state_r().filename, '.');

	for (unsigned int j = 0; j < LANGUAGE_ENTRIES; j++) {
		struct syntax* s = &languages[j];
		unsigned int i = 0;
		while (s->filematch[i]) {
			int is_ext = (s->filematch[i][0] == '.');
			if ((is_ext && ext && !strcmp(ext, s->filematch[i])) ||
				(!is_ext && strstr(state_r().filename, s->filematch[i]))) {
					state_w()->current_syntax = s;
					return;
				}
			i++;
		}
	}
}

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
	return isspace(c) || c == '\0' || strchr(",.()#-/*=~%<>[];_", c) != NULL;
}

void highlighter_update_syntax(struct text_row* row) {
	row->highlight = realloc(row->highlight, row->rendered_size);
	memset(row->highlight, HL_NORMAL, row->rendered_size);

	if (state_r().current_syntax == NULL) return;

	int last_separator = 1;
	int i = 0;
	while (i < row->rendered_size) {
		char this = row->rendered[i];
		unsigned char last_highlight = (i > 0) ? row->highlight[i - 1] : HL_NORMAL;
		int flags = state_r().current_syntax->flags;

		if (
			flags & HL_HIGHLIGHT_NUMBERS &&
			isdigit(this) && 
			((
				last_separator ||
				last_highlight == HL_NUMBER
			) ||
			(
				(this == '.' || (this == '_' && HL_HIGHLIGHT_UNDERSCORED_NUMBERS)) &&
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