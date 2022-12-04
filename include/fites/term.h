#ifndef TERM_H
#define TERM_H

#include <fites/fites.h>
#include <termios.h>

void enable_raw_mode();
void disable_raw_mode();
void term_loop();
void term_reset();
void get_window_size();
#endif
