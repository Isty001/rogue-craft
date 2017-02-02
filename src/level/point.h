#ifndef ROGUECRAFT_POINT_H
#define ROGUECRAFT_POINT_H


#include <stdint.h>
#include <stdbool.h>


#define point_new(y, x) (Point) {y, x}

#define point_eq(p1, p2) (p1.x == p2.x && p1.y == p2.y)


typedef struct {
    uint16_t y;
    uint16_t x;
} Point;

typedef enum {
    NORTH = 1,
    EAST = 2,
    SOUTH = 4,
    WEST = 8,
    NORTH_EAST = NORTH | EAST,
    SOUTH_EAST = SOUTH | EAST,
    SOUTH_WEST = SOUTH | WEST,
    NORTH_WEST = NORTH | WEST
} Direction;


bool point_are_neighbours(Point a, Point b);

Point point_move(Point point, Direction direction, uint16_t distance);


#endif
