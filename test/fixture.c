#include "fixture.h"


Level *fixture_level(void)
{
    Level *level = allocate(sizeof(Level));
    level->size = (Size) {2, 3};

    Cell *hollow = allocate(sizeof(Cell));
    hollow->type = HOLLOW;
    hollow->in_registry = true;
    hollow->material = VOID;
    hollow->state = 100;
    hollow->style = FIXTURE_HOLLOW_STYLE;

    Cell *solid = allocate(sizeof(Cell));
    solid->type = SOLID;
    solid->in_registry = true;
    solid->material = STONE;
    solid->state = 100;
    solid->style = FIXTURE_SOLID_STYLE;

    level->cfg = allocate(sizeof(LevelConfig));

    level->cfg->type = CELLULAR;
    level->cfg->cells = (LevelCells) {
        .hollow = (Probability) {
            .count = 1,
            .items = {{.chance = 1, .value = hollow}}
        },
        .solid = (Probability) {
            .count = 1,
            .items = {{.chance=1, .value = solid}}
        }
    };

    level_add_bounds(level);

    level->cells = allocate(2 * sizeof(Cell **));
    level->cells[0] = allocate(3 * sizeof(Cell *));
    level->cells[1] = allocate(3 * sizeof(Cell *));

    iterate_matrix(
        0, level->size,

        if (y == 1 || ((y == 0) & (x == 1))) {
            level->cells[y][x] = level_registy_rand(level, hollow);
        } else {
            level->cells[y][x] = level_registy_rand(level, solid);
        }
    )

    return level;
}

void fixture_level_free(Level *level)
{
    probability_clean(&level->cfg->cells.hollow, release);
    probability_clean(&level->cfg->cells.solid, release);
    release(level->cells[0]);
    release(level->cells[1]);
    release(level->cfg);

    level_free(level);
}

Item fixture_consumable(bool permanent)
{
    return (Item) {
        .type = CONSUMABLE,
        .value = 10,
        .consumable = (Consumable) {
            .attribute = HEALTH,
            .permanent = permanent,
        }
    };
}
