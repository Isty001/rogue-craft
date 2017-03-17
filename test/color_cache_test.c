#include "unit_test.h"
#include "../src/color.h"


static void assert_color(CachedColor *color)
{
    static int i = 2;

    mu_assert_int_eq(color_last() - i, color->id);
    mu_assert_int_eq(i, color->fore);
    mu_assert_int_eq(i + 1, color->back);

    i--;
}

MU_TEST(test_cache)
{
    color_init();

    mu_assert_int_eq(COLOR_PAIR(color_last()), color_add(2, 3));
    mu_assert_int_eq(COLOR_PAIR(color_last()), color_add(1, 2));

    color_cleanup();

    Cache cache;
    cache_open_colors(&cache);

    cache_foreach(&cache, (Reader) assert_color);

    cache_close(&cache);
}

void run_color_cache_test(void)
{
    TEST_NAME("Color");

    MU_RUN_TEST(test_cache);

    MU_REPORT();
}
