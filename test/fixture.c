#include "util/memory.h"
#include "fixture.h"
#include "util/util.h"
#include "level/level.h"
#include "item/item.h"


Level *fixture_level(void)
{
    Level *level = mem_alloc(sizeof(Level));
    level->size = (Size) {4, 3};
    level->lightings = list_new();

    Cell *hollow = mem_alloc(sizeof(Cell));
    hollow->type = HOLLOW;
    hollow->in_registry = true;
    hollow->material = STONE;
    hollow->state = 100;
    hollow->style = FIXTURE_HOLLOW_STYLE;

    Cell *solid = mem_alloc(sizeof(Cell));
    solid->type = SOLID;
    solid->in_registry = true;
    solid->material = STONE;
    solid->state = 100;
    solid->style = FIXTURE_SOLID_STYLE;

    level->cfg = mem_alloc(sizeof(LevelConfig));

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

    Cell ***cells = mem_alloc(4 * sizeof(Cell **));
    cells[0] = mem_alloc(3 * sizeof(Cell *));
    cells[1] = mem_alloc(3 * sizeof(Cell *));
    cells[2] = mem_alloc(3 * sizeof(Cell *));
    cells[3] = mem_alloc(3 * sizeof(Cell *));

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
    probability_clean(&level->cfg->cells.hollow, mem_dealloc);
    probability_clean(&level->cfg->cells.solid, mem_dealloc);

    level->lightings->release_item = (Release) lighting_free;
    level->lightings->free(level->lightings);

    mem_dealloc(level->cells[0]);
    mem_dealloc(level->cells[1]);
    mem_dealloc(level->cells[2]);
    mem_dealloc(level->cells[3]);
    mem_dealloc(level->cfg);
    mem_dealloc(level->cells);
    mem_dealloc(level);
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
