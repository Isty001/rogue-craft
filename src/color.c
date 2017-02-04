#include "../config/config.h"


static void init_from_cell_config(CellRegistryConfig *cfg)
{
    CellPrototype cell;

    for (size_t i = 0; i < cfg->count; i++) {
        cell = cfg->cells[i];
        init_pair(cell.color.id, cell.color.fore, cell.color.back);
    }
}

void color_init(void)
{
    start_color();
    use_default_colors();

    init_pair(COLOR_PAIR_RED_F, COLOR_RED, -1);
    init_pair(COLOR_PAIR_GREEN_F, COLOR_GREEN, -1);
    init_pair(COLOR_PAIR_BLUE_F, COLOR_BLUE, -1);
    init_pair(COLOR_PAIR_YELLOW_F, COLOR_YELLOW, -1);
    init_pair(COLOR_PAIR_NONE, -1, -1);

    init_from_cell_config(&CELL_REGISTRY_VOID);
    init_from_cell_config(&CELL_REGISTRY_CAVE);
}
