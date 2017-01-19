#include "fixture.h"
#include "../src/level/cell.h"
#include "../src/level/level.h"


Level *fixture_level(void)
{
    Level *level = malloc(sizeof(Level));
    level->size = (Size) {2, 3};

    level->registry.hollow.size = 1;
    level->registry.hollow.cells = malloc(sizeof(Cell));
    level->registry.hollow.cells[0].type = HOLLOW;
    level->registry.hollow.cells[0].is_prototype = true;

    level->registry.solid.size = 1;
    level->registry.solid.cells = malloc(sizeof(Cell));
    level->registry.solid.cells[0].type = SOLID;
    level->registry.solid.cells[0].is_prototype = true;

    level_add_bounds(level);

    level->cells = malloc(2 * sizeof(Cell **));
    level->cells[0] = malloc(3 * sizeof(Cell *));
    level->cells[1] = malloc(3 * sizeof(Cell *));

    iterate_matrix(
        0, level->size,

        if (y == 1 || ((y == 0) & (x == 1))) {
            level->cells[y][x] = &level->registry.hollow.cells[0];
        } else {
            level->cells[y][x] = &level->registry.solid.cells[0];
        }
    )

    return level;
}

