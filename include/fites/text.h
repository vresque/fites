#ifndef TEXT_H
#define TEXT_H
#include <stddef.h>
#include <fites/bool.h>


struct text_row {
    int size;
    char* buffer;
    int rendered_size;
    char* rendered;
    unsigned char* highlight;
    int index;
    bool hl_open_comment;
};

void text_row_insert_char(struct text_row* row, int loc, int chr);
char* text_rows_to_string(int* len);
void text_row_delete_char(struct text_row* row, int loc);
void text_row_drop(struct text_row* row);
void text_row_append_string(struct text_row* row, char* string, size_t length);
int text_rendered_x_to_cursor_x(struct text_row* row, int ren_x);

#endif
