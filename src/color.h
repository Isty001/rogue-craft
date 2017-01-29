#ifndef ROGUECRAFT_COLOR_H
#define ROGUECRAFT_COLOR_H


#include <stddef.h>
#include <ncurses.h>
#include "util.h"

#define COLOR_SCHEMA_MAX_PAIRS  10

#define COLOR_PAIR_NONE         1
#define COLOR_PAIR_RED_F        2
#define COLOR_PAIR_GREEN_F      3
#define COLOR_PAIR_BLUE_F       4
#define COLOR_PAIR_YELLOW_F     5

#define COLOR_HEALTH COLOR_PAIR_RED_F
#define COLOR_STAMINA COLOR_PAIR_GREEN_F
#define COLOR_FOOD COLOR_PAIR_YELLOW_F
#define COLOR_WATER COLOR_PAIR_BLUE_F


typedef short Color;

typedef unsigned long Style;

typedef short Character;


typedef struct {
    Color fore;
    Color back;
} ColorPair;

typedef const struct {
    size_t size;
    uint16_t pairs_from;
    ColorPair pairs[COLOR_SCHEMA_MAX_PAIRS];
} ColorSchema;


void color_init(void);


#endif
