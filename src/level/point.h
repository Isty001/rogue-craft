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


bool point_are_neighbours(Point a, Point b);


#endif
