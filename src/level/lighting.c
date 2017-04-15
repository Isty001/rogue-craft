#include <mem_pool.h>
#include "lighting.h"


static MemPool *LIGHTED_CELL_POOL;


typedef struct LightedCell {
    Point point;
    Cell *original;
    Cell *current;
    struct LightedCell *next;
} LightedCell;

struct Lighting {
    Sight *sight;
    Style style;
    LightedCell *lighted;
};


size_t lighted_cell_size(void)
{
    return sizeof(LightedCell);
}

void lighted_cell_pool_init(void)
{
    LIGHTED_CELL_POOL = pool_init(sizeof(LightedCell), 100);
}

void lighted_cell_pool_cleanup(void)
{
    pool_destroy(LIGHTED_CELL_POOL);
}

static LightedCell *create_lighted_cell(Lighting *lighting, Cell *original, Point point)
{
    LightedCell *lighted = pool_alloc(LIGHTED_CELL_POOL);
    profile_lighted_cell(++);

    lighted->point = point;
    lighted->original = original;
    lighted->current = cell_clone(original);
    lighted->current->lighted = true;
    lighted->next = lighting->lighted;
    lighting->lighted = lighted;

    return lighted;
}

static void light_cells(Lighting *lighting)
{
    Point point;
    Cell *cell;
    LightedCell *lighted;
    Point center = lighting->sight->center;
    Cell ***cells = lighting->sight->level->cells;
    Sight *sight = lighting->sight;

    for (uint16_t i = 0; i < sight->count; i++) {
        point = sight->points[i];
        cell = cells[point.y][point.x];

        lighted = create_lighted_cell(lighting, cell, point);
        cells[point.y][point.x] = lighted->current;

        if (SOLID == cell->type && !point_eq(point, center)) {
            lighted->current->style = lighting->style;
        }
    }
}

Lighting *lighting_new(Level *level, Point source, uint16_t radius, Style style)
{
    Lighting *lighting = allocate(sizeof(Lighting));
    lighting->sight = sight_new(level, source, radius);
    lighting->style = style;
    lighting->lighted = NULL;
    light_cells(lighting);

    return lighting;
}

static void free_cells(Lighting *lighting)
{
    Point point;
    Cell ***cells = lighting->sight->level->cells;
    LightedCell *cell = lighting->lighted;

    while (cell) {
        point = cell->point;
        cells[point.y][point.x] = cell->original;

        cell_free_custom(cell->current);
        pool_release(LIGHTED_CELL_POOL, cell);
        profile_lighted_cell(--);
        cell = cell->next;
    }
    lighting->lighted = NULL;
}

void lighting_update(Lighting *lighting, Point source, uint16_t radius)
{
    if (UPDATED == sight_update(lighting->sight, source, radius)) {
        /** It's ok to 'throw away' all the LightedCells at each update,
         * because we'll just grab them from the pool anyway */
        free_cells(lighting);
        light_cells(lighting);
    }
}

void lighting_free(Lighting *lighting)
{
    free_cells(lighting);
    sight_free(lighting->sight);
    release(lighting);
}
