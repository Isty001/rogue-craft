#include "fixture.h"
#include "../src/util.h"
#include "../src/level/level.h"
#include "../src/player/item.h"


Level *fixture_level(void)
{
    Level *level = allocate(sizeof(Level));
    level->size = (Size) {4, 3};

    Cell *hollow = allocate(sizeof(Cell));
    hollow->type = HOLLOW;
    hollow->in_registry = true;
    hollow->lighted = false;
    hollow->material = VOID;
    hollow->state = 100;
    hollow->style = FIXTURE_HOLLOW_STYLE;

    Cell *solid = allocate(sizeof(Cell));
    solid->type = SOLID;
    solid->in_registry = true;
    solid->lighted = false;
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

    Cell ***cells = allocate(4 * sizeof(Cell **));
    cells[0] = allocate(3 * sizeof(Cell *));
    cells[1] = allocate(3 * sizeof(Cell *));
    cells[2] = allocate(3 * sizeof(Cell *));
    cells[3] = allocate(3 * sizeof(Cell *));

    cells[0][0] = solid;
    cells[0][1] = hollow;
    cells[0][2] = solid;

    cells[1][0] = hollow;
    cells[1][1] = hollow;
    cells[1][2] = hollow;

    cells[2][0] = hollow;
    cells[2][1] = hollow;
    cells[2][2] = hollow;

    cells[3][0] = solid;
    cells[3][1] = solid;
    cells[3][2] = solid;

    level->cells = cells;

    return level;
}

void fixture_level_free(Level *level)
{
    probability_clean(&level->cfg->cells.hollow, release);
    probability_clean(&level->cfg->cells.solid, release);

    release(level->cells[0]);
    release(level->cells[1]);
    release(level->cells[2]);
    release(level->cells[3]);
    release(level->cfg);
    release(level->cells);
    release(level);
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

char *fixture_path(char *path)
{
    static char buff[400];

    sprintf(buff, "%s/%s", getenv(ENV_DIR_FIXTURE), path);

    return buff;
}
