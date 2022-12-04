#ifndef STATE_H
#define STATE_H
#include <termios.h>
#include "text.h"


struct state {
    struct termios terminal;
    int last_key;
    int rows;
    int cols;
    int cursor_x;
    int cursor_y;
    int text_row_count;
    struct text_row* text;
    int row_offset;
    int col_offset;
    int rendered_x;
};

void state_init();
struct state* state_w();
struct state state_r();

#endif
