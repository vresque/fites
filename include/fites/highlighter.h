#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <fites/fites.h>
#include <fites/text.h>

enum highlighter {
	HL_NORMAL = 0,
	HL_NUMBER,
	HL_MATCH,
};

struct syntax {
	char* filetype;
	char** filematch;
	int flags;
};

void highlighter_update_syntax(struct text_row* row);
int highlighter_to_color(enum highlighter highlighter);

#endif