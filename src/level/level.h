#ifndef ROGUECRAFT_LEVEL_H
#define ROGUECRAFT_LEVEL_H


#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "cell.h"
#include "point.h"
#include "../util.h"
#include "../color.h"
#include "liquid.h"


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


/** Avoiding circular references */
typedef struct Player Player;

typedef struct Camera Camera;


typedef enum {
    CAVE
} LevelType;

typedef struct Level {
    Size size;
    Cell ***cells;
    struct {
        Range y;
        Range x;
    } bounds;
    struct {
        CellRegistry solid;
        CellRegistry hollow;
        CellRegistry liquids[MATERIAL_LIQUID_NUM];
    } registry;
} Level;

typedef const struct {
    LevelType type;
    struct {
        CellRegistryConfig *solid;
        CellRegistryConfig *hollow;
        CellRegistryConfig liquids[MATERIAL_LIQUID_NUM];
    } cell;
} LevelConfig;


Level *level_new(Size size, LevelConfig *cfg);

void level_display(Player *player);

void level_generate_cave(Level *level);

void level_interact(Player *player, Point click);

void level_set_hollow(Level *level, Point at);

Cell *level_replace_cell_with_new(Level *level, Point at);

bool level_in_bounds(Level *level, Point point);

Point level_rand_hollow(Level *level);

void level_free(Level *level);


#endif
