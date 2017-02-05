#include "config.h"


/**
 * MATERIAL
 */
Range MATERIAL_STRENGTH[] = {
    [VOID] = range_new(0, 0),
    [STONE] = range_new(40, 80)
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
    .count = 4,
    .chr = '#',
    .cells = {
        {
            .color = {.id = 110, .fore = 2, .back = -1},
            .material = STONE
        },
        {
            .color = {.id = 111, .fore = 237, .back = -1},
            .material = STONE,
        },
        {
            .color = {.id = 112, .fore = 238, .back = -1},
            .material = STONE
        },
        {
            .color = {.id = 113, .fore = 243, .back = -1},
            .material = STONE
        }
    }
};

CellRegistryConfig CELL_REGISTRY_VOID = {
    .type = HOLLOW,
    .count = 1,
    .chr = ' ',
    .cells = {
        {
            .color = {.id = 100, .fore = -1, .back = -1},
            .material = VOID
        }
    }
};
