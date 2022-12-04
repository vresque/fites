#ifndef EDITOR_H
#define EDITOR_H
#include <fites/fites.h>
#include <sys/types.h>
#include "text.h"
#include <stdarg.h>

void editor_open(char* path);
void editor_push_row(char* content, size_t len);
void editor_set_status(const char* fmt, ...);

#endif

