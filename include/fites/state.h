#ifndef STATE_H
#define STATE_H
#include <termios.h>
#include "text.h"
#include <time.h>

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
    char* filename;
    char status[80];
    time_t status_time;
};

void state_init();
struct state* state_w();
struct state state_r();

#endif
