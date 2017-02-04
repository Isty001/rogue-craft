#include <mem_pool.h>
#include "../player/item.h"


static MemPool *POOL;


#define init_cell_at(cells, i, cfg, color, mat)    \
     cells[i].in_registry = true;               \
     cells[i].chr = cfg->chr;                   \
     cells[i].type = cfg->type;                 \
     cells[i].style = COLOR_PAIR(color);        \
     cells[i].material = mat;


static Cell *init_cells(CellRegistryConfig *cfg)
{
    CellPrototype cell;
    Cell *cells = alloc(cfg->count * sizeof(Cell));

    for (uint16_t i = 0; i < cfg->count; i++) {
        cell = cfg->cells[i];
        init_cell_at(cells, i, cfg, cell.color.id, cell.material);
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

void cell_free_custom(Cell *cell)
{
    if (false == cell->in_registry) {
        pool_free(POOL, cell);
    }
}

Cell *cell_random_item(void)
{
    Item *item = item_random();
    Cell *cell = pool_alloc(POOL);

    cell->chr = item->chr;
    cell->style = item->style;
    cell->type = ITEM_;
    cell->in_registry = false;
    cell->data = item;

    return cell;
}

void cell_pool_init(void)
{
    POOL = pool_init(sizeof(Cell), 100);
}

void cell_pool_cleanup(void)
{
    pool_destroy(POOL);
}
