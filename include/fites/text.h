#ifndef TEXT_H
#define TEXT_H

struct text_row {
    int size;
    char* buffer;
    int rendered_size;
    char* rendered;
};

void text_row_insert_char(struct text_row* row, int loc, int chr);
char* text_rows_to_string(int* len);

#endif
