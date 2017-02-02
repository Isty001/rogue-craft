#include "../util.h"
#include "point.h"


#define is_neighbouring(i) (1 == i || 0 == i)


bool point_are_neighbours(Point a, Point b)
{
    if (point_eq(a, b)) {
        return false;
    }

    int16_t y_diff = max(a.y, b.y) - min(a.y, b.y);
    int16_t x_diff = max(a.x, b.x) - min(a.x, b.x);

    return is_neighbouring(y_diff) && is_neighbouring(x_diff);
}

Point point_move(Point point, Direction direction, uint16_t distance)
{
    if (has_flag(direction, NORTH)) point.y -= distance;
    if (has_flag(direction, EAST)) point.x += distance;
    if (has_flag(direction, SOUTH)) point.y += distance;
    if (has_flag(direction, WEST)) point.x -= distance;

    return point;
}
