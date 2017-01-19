#include "../src/player/player.h"
#include "unit_test.h"
#include "../src/level/camera.h"


static void assert_point(Point point, int y, int x)
{
    mu_assert_int_eq(y, point.y);
    mu_assert_int_eq(x, point.x);
}

MU_TEST(test_position)
{
    Camera camera;
    Player player;
    player.position.current = point_new(50, 50);

    Level level;
    level.size = size_new(100, 100);
    level_add_bounds((&level));
    player.level = &level;

    WINDOW win;
    win._maxx = 9; /** @see getmaxx | getmaxy */
    win._maxy = 9;

    camera = camera_new(&player, &win);
    assert_point(camera.position, 45, 45);

    player.position.current = point_new(3, 2);
    camera = camera_new(&player, &win);
    assert_point(camera.position, 0, 0);

    player.position.current = point_new(100, 100);
    camera = camera_new(&player, &win);
    assert_point(camera.position, 89, 89);
}

MU_TEST(test_calculate_from_level_point)
{

    Point actual;
    Camera camera;
    camera.position = point_new(500, 500);

    actual = camera_adjust_level_point(&camera, point_new(525, 550));
    assert_point(actual, 25, 50);

    camera.position = point_new(0, 10);
    actual = camera_adjust_level_point(&camera, point_new(2, 12));
    assert_point(actual, 2, 2);
}

void run_camera_test(void)
{
    TEST_NAME("Camera");

    MU_RUN_TEST(test_position);
    MU_RUN_TEST(test_calculate_from_level_point);

    MU_REPORT();
}
