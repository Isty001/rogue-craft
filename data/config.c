#include "config.h"

/**
 * LEVEL
 */
LevelConfig LEVEL_STONE_CAVE = {
    .type = CAVE,
    .cell.solid = &CELL_WALL_STONE,
    .cell.hollow = &CELL_HOLLOW_SIMPLE
};


/**
 * CELL
 */
CellConfig CELL_WALL_STONE = {
    .type = SOLID,
    .chr = '#',
    .has_color_schema = true,
    .color = {
        .schema = &COLOR_SCHEMA_STONE_WALL
    }
};

CellConfig CELL_HOLLOW_SIMPLE = {
    .type = HOLLOW,
    .chr = ' ',
    .has_color_schema = false,
    .color = {
        .single = COLOR_PAIR_NONE
    }
};



/**
 * COLOR
 */
ColorSchema COLOR_SCHEMA_STONE_WALL = {
    .pairs_from = 110,
    .size = 4,
    .pairs = {
        {.fore = 22, .back = -1},
        {.fore = 237, .back = -1},
        {.fore = 238, .back = -1},
        {.fore = 243, .back = -1}
    }
};
