#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <fites/fites.h>
#include <fites/text.h>

enum highlighter {
	HL_NORMAL = 0,
	HL_NUMBER,
	HL_MATCH,
	HL_STRING,
	HL_COMMENT,
};

struct syntax {
	char* filetype;
	char** filematch;
	int flags;
	char* line_comment;
};

void highlighter_update_syntax(struct text_row* row);
int highlighter_to_color(enum highlighter highlighter);
void highlighter_select_syntax_highlight();
#endif