#include "../ncurses/ncurses.h"
#include "../../config/config.h"


Cell ***level_allocate_cells(Size size)
{
    Cell ***cells = allocate(sizeof(Cell[size.height][size.width]));
    Cell **from = (Cell **) cells + size.height;

    for (int i = 0; i < size.height; i++) {
        cells[i] = from + i * size.height;
    }

    return cells;
}

static void initialize_cells(Level *level)
{
    probability_pick(&level->cfg->cells.solid);

    iterate_matrix(
        0, level->size,
        level->cells[y][x] = rand_bool(0.54)
                             ? level_registry_rand(level, solid)
                             : level_registry_rand(level, hollow)
    );
}

static void add_items(Level *level)
{
    uint16_t remaining = LEVEL_ITEM_COUNT(level);
    Point point;
    Cell *cell;

    do {
        point = level_rand_point(level);
        cell = level->cells[point.y][point.x];

        if (HOLLOW == cell->type) {
            level->cells[point.y][point.x] = cell_with_random_item();
            remaining--;
        }
    } while (remaining);
}

Level *level_new(Size size)
{
    Level *level = allocate(sizeof(Level));
    level->size = size;
    level->cfg = probability_pick(&LEVEL_PROBABILITY);
    level_add_bounds(level);

    level->cells = level_allocate_cells(level->size);
    initialize_cells(level);

    if (CELLULAR == level->cfg->type) {
        level_generate_cellular(level);
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
    } while (HOLLOW != cell->type);

    return point;
}

void level_set_hollow(Level *level, Point at)
{
    cell_free_custom(level->cells[at.y][at.x]);
    level->cells[at.y][at.x] = probability_pick(&level->cfg->cells.hollow);
}

Cell *level_replace_cell_with_new(Level *level, Point at)
{
    Cell ***cells = level->cells;
    Cell *cell = cells[at.y][at.x];
    cell_free_custom(cell);
    Cell *new = cell_clone(cell);

    cells[at.y][at.x] = new;

    return new;
}

void level_free(Level *level)
{
    release(level->cells);
    release(level);
}
