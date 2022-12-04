#ifndef INPUT_H
#define INPUT_H

#define CONTROL(key) ((key) & 0x1f)
#define ALT(key) ((key) + 128)

#include <fites/fites.h>
#include <ctype.h>
#include <stdio.h>


void input_loop();
int input_read_key();

#endif
