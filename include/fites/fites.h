#ifndef FITE_H
#define FITE_H

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "state.h"
#include "buffer.h"
#include "../config.h"

#define VERSION "1.0"

#define UNUSED(var) (void)(var);

typedef _Bool bool;
#define true 1
#define false 0


void die(const char* msg);
#endif
