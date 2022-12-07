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

#define VERSION "1.0"

#define UNUSED(var) ((void)(var));


void die(const char* msg);
#endif
