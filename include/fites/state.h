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

static struct state state;


#endif
