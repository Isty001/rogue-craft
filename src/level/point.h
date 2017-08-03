#ifndef ROGUECRAFT_POINT_H
#define ROGUECRAFT_POINT_H


#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "util/util.h"


#define point_new(y, x) (Point) {y, x}

#define point_eq(p1, p2) (p1.x == p2.x && p1.y == p2.y)

#define size_new(h, w) (Size) {.height = h, .width = w}

#define size_inc(size, h, w) size_new(size.height + h, size.width + w)

#define bounds_from_size(size) \
     (Bounds) {.y = range_new(0, size.height - 1), .x = range_new(0, size.width - 1)}

#define bounds_has(bounds, point) \
            (in_range(point.x, bounds.x) && in_range(point.y, bounds.y))


typedef struct {
    int height;
    int width;
} Size;

typedef struct {
    uint16_t y;
    uint16_t x;
} Point;

typedef struct {
    Range y;
    Range x;
} Bounds;

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


Point point_move(Point point, Direction direction, uint16_t distance);

Direction direction_lookup(int input);

uint16_t point_distance(Point a, Point b);


#endif
