#ifndef INPUT_H
#define INPUT_H

#define CONTROL(key) ((key) & 0x1f)

#include <fites/fites.h>
#include <ctype.h>
#include <stdio.h>


void input_loop();
char input_read_key();

#endif
