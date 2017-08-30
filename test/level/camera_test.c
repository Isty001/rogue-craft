#include "../../src/player/player.h"
#include "player/camera.h"
#include "../unit_test.h"


MU_TEST(test_position)
{
    Camera camera;
    Point center = point_new(50, 50);

    Level level;
    level.size = size_new(100, 100);
    level_add_bounds((&level));

    WINDOW win;
    win._maxx = 9; /** @see getmaxx getmaxy */
    win._maxy = 9;

    camera_update(&camera, center, &level, &win);
    assert_point(camera.left_upper, 45, 45);

    center = point_new(3, 2);
    camera_update(&camera, center, &level, &win);
    assert_point(camera.left_upper, 0, 0);

    center = point_new(100, 100);
    camera_update(&camera, center, &level, &win);
    assert_point(camera.left_upper, 89, 89);
}

MU_TEST(test_camera_to_level_point)
{
    Point actual;
    Camera camera;
    camera.left_upper = point_new(500, 500);

    actual = camera_to_level_point(&camera, point_new(10, 10));
    assert_point(actual, 510, 510);

    camera.left_upper = point_new(0, 10);
    actual = camera_to_level_point(&camera, point_new(2, 12));
    assert_point(actual, 2, 22);
}

void run_camera_test(void)
{
    TEST_NAME("Camera");

    MU_RUN_TEST(test_position);
    MU_RUN_TEST(test_camera_to_level_point);

    MU_REPORT();
}
