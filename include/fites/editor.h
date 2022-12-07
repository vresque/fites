#ifndef EDITOR_H
#define EDITOR_H
#include <fites/fites.h>
#include <sys/types.h>
#include "text.h"
#include <stdarg.h>
#include <stddef.h>

typedef void (*editor_prompt_callback_t)(char*, int);


void editor_open(char* path);
void editor_push_row(int loc, char* content, size_t len);
void editor_set_status(const char* fmt, ...);
void editor_update_row(struct text_row* row);
void editor_insert_char(int chr);
void editor_save();
void editor_delete_char();
void editor_delete_row(int loc);
void editor_insert_new_line();
char* editor_prompt(char* prompt);
char* editor_prompt_with_callback(char* prompt, editor_prompt_callback_t callback);


#endif

