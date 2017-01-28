#include "camera.h"


static inline int adjust_coordinate(int coordinate, int cam_size, int max)
{
    if (coordinate <= 0) {
        return 0;
    } else if (coordinate > (max - cam_size)) {
        return max - cam_size;
    }

    return coordinate;
}

void camera_update(Player *player, WINDOW *window)
{
    Camera *camera = player->camera;
    Point center = player->position.current;
    Level *level = player->level;

    Size cam_size = size_new(
        getmaxy(window),
        getmaxx(window)
    );

    camera->size = cam_size;
    camera->position.y = adjust_coordinate(center.y - cam_size.height / 2, cam_size.height, level->bounds.y.to);
    camera->position.x = adjust_coordinate(center.x - cam_size.width / 2, cam_size.width, level->bounds.x.to);
}

Point camera_to_level_point(Camera *camera, Point camera_point)
{
    Point cam_pos = camera->position;

    return point_new(
        camera_point.y + cam_pos.y,
        camera_point.x + cam_pos.x
    );
}
