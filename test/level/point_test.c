#include "../unit_test.h"


MU_TEST(test_equal)
{
    Point p1 = {1, 1};
    Point p2 = {1, 1};

    mu_assert(point_eq(p1, p2), "Should be equal");

    p2.x = 100;
    mu_assert(false == point_eq(p1, p2), "Shouldn't be equal");
}

MU_TEST(test_move)
{
    Point point = point_new(10, 10);

    assert_point(point_move(point, NORTH_EAST, 1), 9, 11);
    assert_point(point_move(point, SOUTH_WEST, 3), 13, 7);
    assert_point(point_move(point, SOUTH, 2), 12, 10);
}

MU_TEST(test_distance)
{
    Point a = point_new(10, 10);
    Point b = point_new(10, 10);

    mu_assert_int_eq(0, point_distance(a, b));

    b.y = 8;
    mu_assert_int_eq(2, point_distance(a, b));
}

void run_point_test(void)
{
    TEST_NAME("Point");

    MU_RUN_TEST(test_equal);
    MU_RUN_TEST(test_move);
    MU_RUN_TEST(test_distance);

    MU_REPORT();
}
