#include <mem_pool.h>
#include <memory.h>
#include "../player/item.h"


static MemPool *CELL_POOL;


Cell *cell_allocate(void)
{
    profile_cell(++);

    return pool_alloc(CELL_POOL);
}

Cell *cell_with_random_item(void)
{
    Item *item = item_random();
    Cell *cell = cell_allocate();

    cell->chr = item->chr;
    cell->style = item->style;
    cell->type = ITEM_;
    cell->in_registry = false;
    cell->item = item;

    return cell;
}

Cell *cell_clone(const Cell *cell)
{
    Cell *new = cell_allocate();
    memcpy(new, cell, sizeof(Cell));
    new->in_registry = false;

    return new;
}

void cell_free_custom(Cell *cell)
{
    if (false == cell->in_registry) {
        pool_free(CELL_POOL, cell);
        profile_cell(--);
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
