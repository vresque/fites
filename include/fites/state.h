#ifndef STATE_H
#define STATE_H
#include <termios.h>


struct state {
    struct termios terminal;
    char last_key;
    int rows;
    int cols;
    int cursor_x;
    int cursor_y;
};

struct state* state_w();
struct state state_r();

#endif
