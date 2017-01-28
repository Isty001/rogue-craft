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

MU_TEST(test_neighbours)
{
    Point a = point_new(10, 10);

    mu_assert(point_are_neighbours(a, point_new(9, 9)), "Should be neighbours");
    mu_assert(point_are_neighbours(a, point_new(9, 10)), "Should be neighbours");
    mu_assert(point_are_neighbours(point_new(11, 11), a), "Should be neighbours");

    mu_assert(false == point_are_neighbours(a, point_new(10, 10)), "Shouldn't be neighbours");
    mu_assert(false == point_are_neighbours(a, point_new(7, 0)), "Shouldn't be neighbours");
}

void run_point_test(void)
{
    TEST_NAME("Point");

    MU_RUN_TEST(test_equal);
    MU_RUN_TEST(test_neighbours);

    MU_REPORT();
}
