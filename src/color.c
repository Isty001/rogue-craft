#include "../data/config.h"


static void init_color_schema(ColorSchema cfg)
{
    for (size_t i = 0; i < cfg.size; i++) {
        init_pair(cfg.pairs_from + i, cfg.pairs[i].fore, cfg.pairs[i].back);
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

    init_color_schema(COLOR_SCHEMA_STONE_WALL);
}
