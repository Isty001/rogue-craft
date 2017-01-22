#include "camera.h"


static int adjust_coordinate(int coordinate, int cam_size, int max)
{
    if (coordinate <= 0) {
        return 0;
    } else if (coordinate > (max - cam_size)) {
        return max - cam_size;
    }

    return coordinate;
}

Camera camera_new(Player *player, WINDOW *window)
{
    Point center = player->position.current;
    Level *level = player->level;
    Size cam_size = size_new(
        getmaxy(window),
        getmaxx(window)
    );

    return (Camera) {
        .size = cam_size,
        .position = point_new(
            adjust_coordinate(center.y - cam_size.height / 2, cam_size.height, level->bounds.y.to),
            adjust_coordinate(center.x - cam_size.width / 2, cam_size.width, level->bounds.x.to)
        )
    };
}

Point camera_adjust_level_point(Camera *camera, Point level_point)
{
    Point cam_pos = camera->position;

    return point_new(
        level_point.y - cam_pos.y,
        level_point.x - cam_pos.x
    );
}
