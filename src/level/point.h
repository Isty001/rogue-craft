#ifndef ROGUECRAFT_POINT_H
#define ROGUECRAFT_POINT_H


#include <stdint.h>
#include <stdbool.h>
#include <math.h>


#define point_new(y, x) (Point) {y, x}

#define point_eq(p1, p2) (p1.x == p2.x && p1.y == p2.y)

#define square_new(y, x, s) \
     (Square) {.left_upper = point_new(y ,x), .size = s }

#define square_eq(a, b) \
     (point_eq(a.left_upper, b.left_upper) && (a.size == b.size))

#define size_new(h, w) (Size) {.height = h, .width = w}

#define size_eq(a, b) (a.width == b.width && a.height == b.height)


typedef struct {
    int height;
    int width;
} Size;

typedef struct {
    uint16_t y;
    uint16_t x;
} Point;

typedef struct {
    Point left_upper;
    uint16_t size;
} Square;

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

uint16_t point_distance(Point a, Point b);

bool square_has_point(Square square, Point point);


#endif
