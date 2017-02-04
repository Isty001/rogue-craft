#include "config.h"

/**
 * MATERIALS
 */
Material MATERIALS[] = {
    [VOID] = {.strength = 0},
    [STONE] = {.strength = 100},
    [DIRT] = {.strength = 30}
};


/**
 * LEVEL
 */
LevelConfig LEVEL_CAVE = {
    .type = CAVE,
    .cell.solid = &CELL_REGISTRY_CAVE,
    .cell.hollow = &CELL_REGISTRY_VOID
};


/**
 * CELL
 */
CellRegistryConfig CELL_REGISTRY_CAVE = {
    .type = SOLID,
    .chr = '#',
    .count = 4,
    .cells = {
        {.color = {.id = 110, .fore = 2, .back = -1}, .material = &MATERIALS[STONE]},
        {.color = {.id = 111, .fore = 237, .back = -1}, .material = &MATERIALS[STONE]},
        {.color = {.id = 112, .fore = 238, .back = -1}, .material = &MATERIALS[STONE]},
        {.color = {.id = 113, .fore = 243, .back = -1}, .material = &MATERIALS[STONE]}
    }
};

CellRegistryConfig CELL_REGISTRY_VOID = {
    .type = HOLLOW,
    .chr = ' ',
    .count = 1,
    .cells = {
        {.color = {.id = 100, .fore = -1, .back = -1}, .material = &MATERIALS[VOID]}
    }
};
