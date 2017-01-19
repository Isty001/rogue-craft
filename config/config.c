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
        .schema = &COLOR_STONE_WALL
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

CellConfig CELL_WATER = {
    .type = HOLLOW,
    .has_color_schema = true,
    .chr = '~',
    .color = {
        .schema = &COLOR_WATER
    }
};


/**
 * COLOR
 */
ColorSchema COLOR_STONE_WALL = {
    .pairs_from = 110,
    .size = 4,
    .pairs = {
        {.fore = 22, .back = -1},
        {.fore = 237, .back = -1},
        {.fore = 238, .back = -1},
        {.fore = 243, .back = -1}
    }
};

ColorSchema COLOR_WATER = {
    .pairs_from = 120,
    .size = 0,
    .pairs = {
    }
};
