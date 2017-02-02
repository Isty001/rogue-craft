#ifndef ROGUECRAFT_LEVEL_H
#define ROGUECRAFT_LEVEL_H


#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "cell.h"
#include "point.h"
#include "../util.h"
#include "../color.h"


#define iterate_matrix(from, size, ...)             \
    for (int y = from; y < size.height; y++) {      \
        for (int x = from; x < size.width; x++) {   \
            __VA_ARGS__;                            \
        }                                           \
    }

#define level_add_bounds(level)                                                       \
    level->bounds.y = (Range) {.from = 0, .to = level->size.height - 1};       \
    level->bounds.x = (Range) {.from = 0, .to = level->size.width - 1};        \

#define level_rand_point(level) \
    point_new(rand_in_range(level->bounds.y), rand_in_range(level->bounds.x));

#define size_new(h, w) (Size) {.height = h, .width = w}


/** Avoiding circular references */
typedef struct Player Player;

typedef struct Camera Camera;


typedef struct {
    int height;
    int width;
} Size;

typedef enum {
    CAVE
} LevelType;

typedef struct {
    Size size;
    Cell ***cells;
    uint16_t item_count;
    struct {
        Range y;
        Range x;
    } bounds;
    struct {
        CellRegistry solid;
        CellRegistry hollow;
    } registry;
} Level;

typedef const struct {
    LevelType type;
    struct {
        CellConfig *solid;
        CellConfig *hollow;
    } cell;
} LevelConfig;


Level *level_new(Size size, LevelConfig cfg);

void level_display(Player *player);

void level_generate_cave(Level *level);

void level_interact(Player *player, Point click);

void level_set_hollow(Level *level, Point at);

bool level_in_bounds(Level *level, Point point);

void level_free(Level *level);


#endif
