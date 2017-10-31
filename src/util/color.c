#include "config.h"


static uint16_t LAST_PAIR = 100;
static Cache CACHE;


static void new_cached(CachedColor *color, Color fore, Color back)
{
    memset(color, 0, sizeof(CachedColor));

    color->id = LAST_PAIR;
    color->fore = fore;
    color->back = back;
}

ColorPair color_add(Color fore, Color back)
{
    init_pair(LAST_PAIR, fore, back);

    CachedColor color;
    new_cached(&color, fore, back);
    cache_add(&CACHE, &color);

    return COLOR_PAIR(LAST_PAIR++);
}

static void load_color(CachedColor *color)
{
    init_pair(color->id, color->fore, color->back);

    LAST_PAIR = max(LAST_PAIR, color->id);
}

void color_init(void)
{
    cache_open_colors(&CACHE);

    if (cache_is_empty(CACHE_COLORS)) {
        cache_foreach(&CACHE, (Reader) load_color);
    }

    init_pair(COLOR_RED_F, COLOR_RED, -1);
    init_pair(COLOR_GREEN_F, COLOR_GREEN, -1);
    init_pair(COLOR_GREEN_B, -1, COLOR_GREEN);
    init_pair(COLOR_BLUE_F, COLOR_BLUE, -1);
    init_pair(COLOR_YELLOW_F, COLOR_YELLOW, -1);
    init_pair(COLOR_GRAY_F, 8, -1);
    init_pair(COLOR_DARK_GRAY_F, 235, -1);
    init_pair(COLOR_BROWN_F, 94, -1);
    init_pair(COLOR_DARK_GREEN_F, 22, -1);
    init_pair(COLOR_CLARET_F, 52, -1);
    init_pair(COLOR_NONE, -1, -1);
    init_pair(COLOR_DARK_GREEN_B, -1, 22);
    init_pair(COLOR_BROWN_B, -1, 94);
    init_pair(COLOR_ORANGE_B, -1, 202);
    init_pair(COLOR_YELLOW_B, -1, COLOR_YELLOW);
    init_pair(COLOR_RED_F_B, COLOR_RED, COLOR_RED);
    init_pair(COLOR_WHITE_B, -1, 15);
    init_pair(COLOR_GRAY_B, -1, 8);
    init_pair(COLOR_DARK_GRAY_B, -1, 235);
    init_pair(COLOR_DARK_BROWN_B, -1, 58);
    init_pair(COLOR_DARK_BROWN_F, 58, -1);
    init_pair(COLOR_ORANGE_F, 202, -1);
    init_pair(COLOR_WHITE_F, -1, 15);
    init_pair(COLOR_PURPLE_F, 91, -1);
}

uint16_t color_last(void)
{
    return LAST_PAIR;
}

void color_cleanup(void)
{
    cache_close(&CACHE);
}
