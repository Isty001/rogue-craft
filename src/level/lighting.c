#include <mem_pool.h>
#include "lighting.h"


static MemPool *POOL;


typedef struct LightedCell {
    Point point;
    Cell *original;
    Cell *current;
    struct LightedCell *next;
} LightedCell;

struct Lighting {
    Sight *sight;
    Style style;
    LightedCell *cells;
};


#ifdef DEBUG_MODE

size_t lighted_cell_size(void)
{
    return sizeof(LightedCell);
}

#endif

void lighted_cell_pool_init(void)
{
    POOL = pool_init(sizeof(LightedCell), 100);
}

void lighted_cell_pool_cleanup(void)
{
    pool_destroy(POOL);
}

static LightedCell *light_cell(Lighting *lighting, Cell *original)
{
    LightedCell *lighted = pool_alloc(POOL);
    profile_lighted_cell(++);

    lighted->original = original;
    lighted->current = cell_clone(original);
    lighted->current->style = lighting->style;

    lighted->next = lighting->cells;
    lighting->cells = lighted;

    return lighted;
}

static void light_cells(Lighting *lighting, Cell ***cells)
{
    Point point;
    Cell *original;
    LightedCell *lighted;
    Sight *sight = lighting->sight;

    for (int i = 0; i < sight->count; i++) {
        point = sight->points[i];
        original = cells[point.y][point.x];

        if (SOLID == original->type) {
            lighted = light_cell(lighting, original);
            lighted->point = point;
            cells[point.y][point.x] = lighted->current;
        }
    }
}

Lighting *lighting_new(Level *level, Point source, uint16_t radius, Style style)
{
    Lighting *lighting = allocate(sizeof(Lighting));
    lighting->style = style;

    Sight *sight = sight_new(level, source, radius);
    lighting->sight = sight;
    lighting->cells = NULL;

    light_cells(lighting, level->cells);

    return lighting;
}

static void free_cells(Lighting *lighting, Cell ***cells)
{
    Point point;
    LightedCell *tmp, *head = lighting->cells;

    while (head) {
        tmp = head;
        head = head->next;

        point = tmp->point;
        cells[point.y][point.x] = tmp->original;

        cell_free_custom(tmp->current);
        pool_release(POOL, tmp);
        profile_lighted_cell(--);
    }
    lighting->cells = NULL;
}

void lighting_update(Lighting *lighting, Level *level, Point source, uint16_t radius)
{
    if (SIGHT_UPDATED == sight_update(lighting->sight, level, source, radius)) {

        /** It's ok to 'throw away' all the LightedCells at each update,
         * because we'll just grab them from the pool anyway */
        free_cells(lighting, level->cells);
        light_cells(lighting, level->cells);
    }
}

void lighting_free(Lighting *lighting, Level *level)
{
    free_cells(lighting, level->cells);
    sight_free(lighting->sight);
    release(lighting);
}
