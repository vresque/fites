#ifndef EDITOR_H
#define EDITOR_H
#include <fites/fites.h>
#include <sys/types.h>
#include "text.h"
#include <stdarg.h>

void editor_open(char* path);
void editor_push_row(char* content, size_t len);
void editor_set_status(const char* fmt, ...);
void editor_update_row(struct text_row* row);
void editor_insert_char(int chr);
void editor_save();
#endif

