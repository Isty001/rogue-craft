#ifndef ROGUECRAFT_CAMERA_H
#define ROGUECRAFT_CAMERA_H


#include <ncurses.h>
#include <player/sight.h>
#include "level/point.h"


#define CAMERA_BORDER 1


typedef struct Camera {
    Point left_upper;
    Size size;
} Camera;

typedef struct Level Level;


void camera_update(Camera *camera, Point center, Level *level, WINDOW *window);

Point camera_to_level_point(Camera *camera, Point level_point);

Point camera_level_to_camera_point(Camera *camera, Point level_point);

bool camera_has_sigh(Camera *camera, Sight *sight);


#endif
