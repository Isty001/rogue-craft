#include "../config/config.h"


static uint16_t LAST_PAIR = 100;


ColorPair color_add(Color fore, Color back)
{
    init_pair(LAST_PAIR, fore, back);

    return COLOR_PAIR(LAST_PAIR++);
}

void color_init(void)
{
    start_color();
    use_default_colors();

    init_pair(COLOR_PAIR_RED_F, COLOR_RED, -1);
    init_pair(COLOR_PAIR_GREEN_F, COLOR_GREEN, -1);
    init_pair(COLOR_PAIR_GREEN_B, -1, COLOR_GREEN);
    init_pair(COLOR_PAIR_BLUE_F, COLOR_BLUE, -1);
    init_pair(COLOR_PAIR_YELLOW_F, COLOR_YELLOW, -1);
    init_pair(COLOR_PAIR_GRAY_F, 8, -1);
    init_pair(COLOR_PAIR_DARK_GRAY_F, 235, -1);
    init_pair(COLOR_PAIR_BROWN_F, 94, -1);
    init_pair(COLOR_PAIR_DARK_GREEN_F, 22, -1);
    init_pair(COLOR_PAIR_CLARET_F, 52, -1);
    init_pair(COLOR_PAIR_NONE, -1, -1);
    init_pair(COLOR_PAIR_DARK_GREEN_B, -1, 22);
    init_pair(COLOR_PAIR_BROWN_B, -1, 94);
}
