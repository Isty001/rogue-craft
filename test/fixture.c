#include "fixture.h"


Level *fixture_level(void)
{
    Level *level = malloc(sizeof(Level));
    level->size = (Size) {2, 3};

    level->registry.hollow.size = 1;
    level->registry.hollow.cells = malloc(sizeof(Cell));
    level->registry.hollow.cells[0].type = HOLLOW;
    level->registry.hollow.cells[0].in_registry = true;

    level->registry.solid.size = 1;
    level->registry.solid.cells = malloc(sizeof(Cell));
    level->registry.solid.cells[0].type = SOLID;
    level->registry.solid.cells[0].in_registry = true;

    level_add_bounds(level);

    level->cells = alloc(2 * sizeof(Cell **));
    level->cells[0] = alloc(3 * sizeof(Cell *));
    level->cells[1] = alloc(3 * sizeof(Cell *));

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

void fixture_level_free(Level *level)
{
    free(level->cells[0]);
    free(level->cells[1]);

    level_free(level);
}
