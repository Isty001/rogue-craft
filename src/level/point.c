#include <math.h>
#include "../util.h"
#include "point.h"


Point point_move(Point point, Direction direction, uint16_t distance)
{
    if (has_flag(direction, NORTH)) point.y -= distance;
    if (has_flag(direction, EAST)) point.x += distance;
    if (has_flag(direction, SOUTH)) point.y += distance;
    if (has_flag(direction, WEST)) point.x -= distance;

    return point;
}

uint16_t point_distance(Point a, Point b)
{
    return sqrt(sqr(a.x - b.x) + sqr(a.y - b.y));
}
