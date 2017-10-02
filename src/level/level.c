#include "ui/ncurses.h"
#include "config.h"
#include "memory/memory.h"
#include "cellular.h"


Cell ***level_allocate_cells(Size size)
{
    Cell ***cells = mem_alloc(sizeof(Cell[size.height][size.width]));
    Cell **from = (Cell **) cells + size.height;

    for (int i = 0; i < size.height; i++) {
        cells[i] = from + i * size.height;
    }

    return cells;
}

static void add_items(Level *level)
{
    uint16_t remaining = LEVEL_ITEM_COUNT(level);
    Point point;
    Cell *cell;

    do {
        point = level_rand_point(level);
        cell = level->cells[point.y][point.x];

        if (CELL_HOLLOW == cell->type) {
            level->cells[point.y][point.x] = cell_with_random_item();
            remaining--;
        }
    } while (remaining);
}

Level *level_new(Size size)
{
    Level *level = mem_alloc(sizeof(Level));
    level->size = size;
    level->cfg = probability_pick(&LEVEL_PROBABILITY);
    level_add_bounds(level);
    level->lightings = list_new();
    level->lightings->release_item = (Release) lighting_free;

    level->cells = level_allocate_cells(level->size);

    if (LEVEL_CELLULAR == level->cfg->type) {
        cellular_generate(level);
    }

    add_items(level);

    return level;
}

bool level_in_bounds(Level *level, Point point)
{
    return bounds_has(level->bounds, point);
}

Point level_rand_hollow(Level *level)
{
    Point point;
    Cell *cell;

    do {
        point = level_rand_point(level);

        cell = level->cells[point.y][point.x];
    } while (CELL_HOLLOW != cell->type);

    return point;
}

void level_set_hollow(Level *level, Point at)
{
    cell_free(level->cells[at.y][at.x]);
    level->cells[at.y][at.x] = probability_pick(&level->cfg->cells.hollow);
}

Cell *level_replace_cell_with_new(Level *level, Point at)
{
    Cell ***cells = level->cells;
    Cell *cell = cells[at.y][at.x];

    Cell *new = cell_clone(cell);
    cells[at.y][at.x] = new;

    cell_free(cell);

    return new;
}

void level_free(Level *level)
{
    level->lightings->free(level->lightings);
    Cell *cell;

    iterate_matrix(0, level->size,
        cell = level->cells[y][x];

        if (CELL_ITEM == cell->type) {
            item_free(cell->item);
        }
        cell_free(cell);
    )

    mem_dealloc(level->cells);
    mem_dealloc(level);
}
