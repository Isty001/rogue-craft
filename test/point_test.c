#include <stdbool.h>
#include <ncurses.h>
#include "unit_test.h"
#include "../src/level/point.h"


MU_TEST(test_equal)
{
    Point p1 = {1, 1};
    Point p2 = {1, 1};

    mu_assert(point_eq(p1, p2), "Should be equal");

    p2.x = 100;

    mu_assert(false == point_eq(p1, p2), "Shouldn't be equal");
}

void run_point_test(void)
{
    TEST_NAME("Point");

    MU_RUN_TEST(test_equal);

    MU_REPORT();
}
