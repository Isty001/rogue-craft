#include <level/point.h>
#include <player/sight.h>
#include "level/level.h"
#include "player/player.h"


static inline int adjust_coordinate(int coordinate, int cam_size, int max)
{
    if (coordinate <= 0) {
        return 0;
    } else if (coordinate > (max - cam_size)) {
        return max - cam_size;
    }

    return coordinate;
}

void camera_update(Camera *camera, Point center, Level *level, WINDOW *window)
{
    Size cam_size = size_new(
        getmaxy(window),
        getmaxx(window)
    );

    camera->size = cam_size;
    camera->left_upper.y = adjust_coordinate(center.y - (cam_size.height / 2), cam_size.height, level->bounds.y.to);
    camera->left_upper.x = adjust_coordinate(center.x - (cam_size.width / 2), cam_size.width, level->bounds.x.to);
}

Point camera_to_level_point(Camera *camera, Point camera_point)
{
    Point cam_pos = camera->left_upper;

    return point_new(
        camera_point.y + cam_pos.y,
        camera_point.x + cam_pos.x
    );
}

Point camera_level_to_camera_point(Camera *camera, Point level_point)
{
    Point cam_pos = camera->left_upper;

    return point_new(
        level_point.y - cam_pos.y,
        level_point.x - cam_pos.x
    );
}

/**
 * source: https://yal.cc/rectangle-circle-intersection-test/
 */
bool camera_has_sigh(Camera *camera, Sight *sight)
{
    Point circle_center = sight->center;
    Point rect_point = camera->left_upper;
    Size rect_size = camera->size;

    int16_t dist_x = circle_center.x - max(rect_point.x, min(circle_center.x, rect_point.x + rect_size.width));
    int16_t dist_y = circle_center.y - max(rect_point.y, min(circle_center.y, rect_point.y + rect_size.height));

    return (dist_x * dist_x + dist_y * dist_y) < (sight->radius * sight->radius);
}
