#include <math.h>
#include "keymap.h"
#include "point.h"


static int KEY_MAIN_DIRECTION_MAP[4][2] = {
    {KEY_NORTH, NORTH},
    {KEY_EAST,  EAST},
    {KEY_SOUTH, SOUTH},
    {KEY_WEST,  WEST},
};

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

Direction direction_lookup(int input)
{
    for (int i = 0; i < 4; ++i) {
        if (input == KEY_MAIN_DIRECTION_MAP[i][0]) {
            return (Direction) KEY_MAIN_DIRECTION_MAP[i][1];
        }
    }
    return 0;
}
