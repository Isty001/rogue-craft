#include "unit_test.h"


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

MU_TEST(test_square_ha_point)
{
    Square rect = square_new(0, 0, 10);
    Point point = point_new(9, 10);

    mu_assert(square_has_point(rect, point), "Point should be inside");

    point.y = 100;
    mu_assert(false == square_has_point(rect, point), "Point should not be inside");
}

MU_TEST(test_rect_equal)
{
    Square a = square_new(0, 0, 10);
    Square b = square_new(0, 0, 10);

    mu_assert(square_eq(a, b), "Rectangles should be equal");

    b.left_upper.y = 10;
    mu_assert(false == square_eq(a, b), "Squares should not be equal");

    b.left_upper.y = 0;
    b.size = 90;
    mu_assert(false == square_eq(a, b), "Squares should not be equal");
}

void run_point_test(void)
{
    TEST_NAME("Point");

    MU_RUN_TEST(test_equal);
    MU_RUN_TEST(test_move);
    MU_RUN_TEST(test_distance);
    MU_RUN_TEST(test_square_ha_point);
    MU_RUN_TEST(test_rect_equal);

    MU_REPORT();
}
