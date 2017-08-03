#ifndef ROGUECRAFT_CAMERA_H
#define ROGUECRAFT_CAMERA_H


#include "player/player.h"


#define CAMERA_BORDER 1


typedef struct Camera {
    Point left_upper;
    Size size;
} Camera;


void camera_update(Player *player, WINDOW *window);

Point camera_to_level_point(Camera *camera, Point level_point);


#endif
