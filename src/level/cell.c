#include "cell.h"


#define init_cell_at(cells, i, cfg, colour)   \
     cells[i].is_prototype = true;            \
     cells[i].chr = cfg.chr;                  \
     cells[i].type = cfg.type;                \
     cells[i].color = (Color) colour;


static Cell *init_cells_with_color_schema(CellConfig cfg)
{
    ColorSchema *colors = cfg.color.schema;

    Cell *cells = alloc(colors->size * sizeof(Cell));

    for (unsigned short i = 0; i < colors->size; i++) {
        init_cell_at(cells, i, cfg, colors->pairs_from + i);
    }

    return cells;
}

CellRegistry cell_registry_new(CellConfig cfg)
{
    CellRegistry registry;
    Cell *cells;

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
