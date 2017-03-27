#ifndef ROGUECRAFT_COLOR_H
#define ROGUECRAFT_COLOR_H


#include <stddef.h>
#include <ncurses.h>
#include "storage/cache.h"
#include "util.h"
#include "../config/paths.h"


#define COLOR_PAIR_NONE           1
#define COLOR_PAIR_RED_F          2
#define COLOR_PAIR_GREEN_F        3
#define COLOR_PAIR_BLUE_F         4
#define COLOR_PAIR_YELLOW_F       5
#define COLOR_PAIR_GRAY_F         6
#define COLOR_PAIR_BROWN_F        7
#define COLOR_PAIR_CLARET_F       8
#define COLOR_PAIR_DARK_GREEN_F   9
#define COLOR_PAIR_DARK_GRAY_F   10
#define COLOR_PAIR_GREEN_B       11
#define COLOR_PAIR_DARK_GREEN_B  12
#define COLOR_PAIR_BROWN_B       13
#define COLOR_PAIR_YELLOW_B      14
#define COLOR_PAIR_ORANGE_B      15

#define cache_open_colors(cache) \
    cache_open(cache, CACHE_COLORS, sizeof(CachedColor))


typedef short Color;

typedef unsigned long ColorPair;

typedef unsigned long Style;

typedef struct {
    Color fore;
    Color back;
    uint16_t id;
} CachedColor;


void color_init(void);

/**
 * @see make palette
 */
ColorPair color_add(Color fore, Color back);

uint16_t color_last(void);

void color_cleanup(void);


#endif
