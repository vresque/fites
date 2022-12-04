#ifndef TEXT_H
#define TEXT_H


struct text_row {
    int size;
    char* buffer;
    int rendered_size;
    char* rendered;
};

#endif
