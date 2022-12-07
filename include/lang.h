#ifndef LANG_H
#define LANG_H

#include "fites/highlighter.h"
#define HL_HIGHLIGHT_NUMBERS (1 << 0)
#define HL_HIGHLIGHT_UNDERSCORED_NUMBERS (1 << 1)


#include "../languages.h"
#define language_entries (sizeof(languages) / sizeof(languages[0]))
#endif
