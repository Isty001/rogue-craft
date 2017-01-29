#include <mem_pool.h>
#include "../player/item.h"


static MemPool *POOL;


#define init_cell_at(cells, i, cfg, color)   \
     cells[i].in_registry = true;             \
     cells[i].chr = cfg.chr;                  \
     cells[i].type = cfg.type;                \
     cells[i].style = COLOR_PAIR(color);


static Cell *init_cells_with_color_schema(CellConfig cfg)
{
    ColorSchema *colors = cfg.color.schema;
    Cell *cells = alloc(colors->size * sizeof(Cell));

    for (uint16_t i = 0; i < colors->size; i++) {
        init_cell_at(cells, i, cfg, colors->pairs_from + i);
    }

    return cells;
}

CellRegistry cell_registry_new(CellConfig cfg)
{
    Cell *cells;
    CellRegistry registry;

    if (cfg.has_color_schema) {
        cells = init_cells_with_color_schema(cfg);
        registry.size = cfg.color.schema->size;
    } else {
        cells = alloc(sizeof(Cell));
        init_cell_at(cells, 0, cfg, cfg.color.single);
        registry.size = 1;
    }
    registry.cells = cells;

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
