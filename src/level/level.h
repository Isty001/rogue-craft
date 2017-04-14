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
#include "../event.h"
#include "../ncurses/ncurses.h"
#include "../randomization.h"


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

#define level_registry_rand(level, type) \
    probability_pick(&level->cfg->cells.type)

/** Avoiding circular references */
typedef struct Player Player;

typedef struct Camera Camera;


typedef enum {
    CELLULAR
} LevelType;

typedef struct {
    Probability hollow;
    Probability solid;
} LevelCells;

typedef struct {
    LevelType type;
    LevelCells cells;
} LevelConfig;

typedef struct Level {
    Size size;
    Cell ***cells;
    LevelConfig *cfg;
    List *lightings;

    struct {
        Range y;
        Range x;
    } bounds;
} Level;

typedef struct {
    Player *player;
    Cell *cell;
    Point point;
    uint16_t player_distance;
} InteractionEvent;


void level_load(void);

CacheError level_cache_load(void);

void level_cache_save(void);

void level_unload(void);

Level *level_new(Size size);

Cell ***level_allocate_cells(Size size);

void level_display(Player *player);

void level_generate_cellular(Level *level);

EventError level_interact(ClickEvent *click);

void level_set_hollow(Level *level, Point at);

Cell *level_replace_cell_with_new(Level *level, Point at);

bool level_in_bounds(Level *level, Point point);

Point level_rand_hollow(Level *level);

void level_free(Level *level);


#endif
