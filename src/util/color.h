#ifndef ROGUECRAFT_COLOR_H
#define ROGUECRAFT_COLOR_H


#include <stddef.h>
#include <ncurses.h>
#include "storage/cache.h"
#include "util/util.h"


#define COLOR_NONE           1
#define COLOR_RED_F          2
#define COLOR_GREEN_F        3
#define COLOR_BLUE_F         4
#define COLOR_YELLOW_F       5
#define COLOR_GRAY_F         6
#define COLOR_BROWN_F        7
#define COLOR_CLARET_F       8
#define COLOR_DARK_GREEN_F   9
#define COLOR_DARK_GRAY_F   10
#define COLOR_GREEN_B       11
#define COLOR_DARK_GREEN_B  12
#define COLOR_BROWN_B       13
#define COLOR_YELLOW_B      14
#define COLOR_ORANGE_B      15
#define COLOR_RED_F_B       16
#define COLOR_WHITE_B       17
#define COLOR_GRAY_B        18
#define COLOR_DARK_GRAY_B   19
#define COLOR_DARK_BROWN_B  20
#define COLOR_DARK_BROWN_F  21
#define COLOR_ORANGE_F      22

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
