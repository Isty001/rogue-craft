#ifndef ROGUECRAFT_POINT_H
#define ROGUECRAFT_POINT_H


#define point_new(y, x) (Point) {y, x}

#define point_eq(p1, p2) (p1.x == p2.x && p1.y == p2.y)


typedef struct {
    int y;
    int x;
} Point;


#endif
