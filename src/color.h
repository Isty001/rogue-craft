#ifndef ROGUECRAFT_COLOR_H
#define ROGUECRAFT_COLOR_H


#include <stddef.h>
#include <ncurses.h>
#include "util.h"
#include "level/material.h"


#define COLOR_PAIR_NONE         1
#define COLOR_PAIR_RED_F        2
#define COLOR_PAIR_GREEN_F      3
#define COLOR_PAIR_BLUE_F       4
#define COLOR_PAIR_YELLOW_F     5
#define COLOR_PAIR_GRAY_F       6

#define COLOR_HEALTH COLOR_PAIR_RED_F
#define COLOR_STAMINA COLOR_PAIR_GREEN_F
#define COLOR_FOOD COLOR_PAIR_YELLOW_F
#define COLOR_WATER COLOR_PAIR_BLUE_F


typedef short Color;

typedef unsigned long Style;


void color_init(void);


#endif
