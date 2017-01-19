#ifndef ROGUECRAFT_COLOR_H
#define ROGUECRAFT_COLOR_H


#include <stddef.h>
#include <ncurses.h>
#include "util.h"

#define COLOR_SCHEMA_MAX 10


typedef short Color;

typedef struct {
    Color fore;
    Color back;
} ColorPair;

typedef const struct {
    size_t size;
    unsigned pairs_from;
    ColorPair pairs[COLOR_SCHEMA_MAX];
} ColorSchema;


void color_init(void);


#endif
