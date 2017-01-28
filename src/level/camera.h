#ifndef ROGUECRAFT_CAMERA_H
#define ROGUECRAFT_CAMERA_H


#include "../player/player.h"


typedef struct Camera {
    Point position;
    Size size;
} Camera;


void camera_update(Player *player, WINDOW *window);

Point camera_adjust_level_point(Camera *camera, Point level_point);


#endif
