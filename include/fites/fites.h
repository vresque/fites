#ifndef FITE_H
#define FITE_H
#include "../config.h"


#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include "state.h"
#include "buffer.h"
#include "editor.h"
#include "finder.h"

#define VERSION "1.0"

#define CONTROL(key) ((key) & 0x1f)
#define ALT(key) ((key) + 128)


#define UNUSED(var) ((void)(var));


void die(const char* msg);
#endif
