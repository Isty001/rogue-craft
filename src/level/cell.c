#include <mem_pool.h>
#include <memory.h>
#include "../player/item.h"
#include "../../config/config.h"
#include "../debug.h"


static MemPool *CELL_POOL;


static Cell *init_cells(CellRegistryConfig *cfg)
{
    CellPrototype prototype;
    Cell *cells = alloc(cfg->count * sizeof(Cell));

    for (uint16_t i = 0; i < cfg->count; i++) {
        prototype = cfg->cells[i];
        cells[i].style = COLOR_PAIR(prototype.color.id);
        cells[i].material = prototype.material;
        cells[i].chr = cfg->chr;
        cells[i].type = cfg->type;
        cells[i].in_registry = true;
        cells[i].state = rand_in_range(MATERIAL_STRENGTH[prototype.material]);
    }

    return cells;
}

CellRegistry cell_registry_new(CellRegistryConfig *cfg)
{
    CellRegistry registry;

    registry.size = cfg->count;
    registry.cells = init_cells(cfg);;

    return registry;
}

static inline Cell *alloc_cell(void)
{
    dbg_cell(++);

    return pool_alloc(CELL_POOL);
}

Cell *cell_with_random_item(void)
{
    Item *item = item_random();
    Cell *cell = alloc_cell();

    cell->chr = item->chr;
    cell->style = item->style;
    cell->type = ITEM_;
    cell->in_registry = false;
    cell->item = item;

    return cell;
}

Cell *cell_clone(Cell *cell)
{
    Cell *new = alloc_cell();
    memcpy(new, cell, sizeof(Cell));
    new->in_registry = false;

    return new;
}

void cell_free_custom(Cell *cell)
{
    if (false == cell->in_registry) {
        pool_free(CELL_POOL, cell);
        dbg_cell(--);
    }
}

void cell_pool_init(void)
{
    CELL_POOL = pool_init(sizeof(Cell), 100);
}

void cell_pool_cleanup(void)
{
    pool_destroy(CELL_POOL);
}
