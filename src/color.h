#ifndef ROGUECRAFT_COLOR_H
#define ROGUECRAFT_COLOR_H


#include <stddef.h>
#include <ncurses.h>
#include "util.h"


#define COLOR_PAIR_NONE         1
#define COLOR_PAIR_RED_F        2
#define COLOR_PAIR_GREEN_F      3
#define COLOR_PAIR_BLUE_F       4
#define COLOR_PAIR_YELLOW_F     5
#define COLOR_PAIR_GRAY_F       6
#define COLOR_PAIR_BROWN_F      7
#define COLOR_PAIR_CLARET_F     8
#define COLOR_PAIR_DARK_GREEN_F 9

typedef short Color;

typedef unsigned long Style;


void color_init(void);


#endif
