#ifndef STATE_H
#define STATE_H
#include <termios.h>
#include "text.h"
#include <time.h>
#include "highlighter.h"
#include "bool.h"

struct state {
    struct termios terminal;
    int last_key;
    int rows;
    int cols;
    unsigned int cursor_x;
    unsigned int cursor_y;
    unsigned int text_row_count;
    struct text_row* text;
    unsigned int row_offset;
    unsigned int col_offset;
    unsigned int rendered_x;
    struct syntax* current_syntax;
    char* filename;
    char status[80];
    time_t status_time;
    int buffer_is_dirty;
};

void state_init();
struct state* state_w();
struct state state_r();

#endif
