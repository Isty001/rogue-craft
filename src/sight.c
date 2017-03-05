#include "sight.h"


#define area_size(r) (M_PI * sqr(r)) * sizeof(Point)


#define setup_modifiers(d, inc, target, current, axis)  \
    d = abs(target.axis - current.axis);                \
    inc = current.axis < target.axis ? 1 : -1;


struct Sight {
    uint16_t radius;
    uint16_t count;
    Point center;
    Point *points;
};

static void ray_cast(Sight *sight, Point target, Level *level)
{
    Point current = sight->center;

    int dx, inc_x, dy, inc_y;
    setup_modifiers(dx, inc_x, target, current, x);
    setup_modifiers(dy, inc_y, target, current, y);

    int err = (dx > dy ? dx : -dy) / 2, err_prev;

    while (!point_eq(current, target)) {

        err_prev = err;
        if (err_prev > -dx) {
            err -= dy;
            current.x += inc_x;
        }
        if (err_prev < dy) {
            err += dx;
            current.y += inc_y;
        }

        if (!sight_has(sight, current)) {
            sight->points[sight->count++] = current;
        }

        if (SOLID == level->cells[current.y][current.x]->type) {
            break;
        }
    }
}

/**
 * @see http://www.roguebasin.com/index.php?title=Ray_casting
 * @see https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
 */
static void collect_points(Sight *sight, Level *level)
{
    Point target;

    for (double deg = 0; deg < 360; deg++) {

        target.y = sight->center.y + (sight->radius * sin(deg));
        target.x = sight->center.x + (sight->radius * cos(deg));

        if (level_in_bounds(level, target)) {
            ray_cast(sight, target, level);
        }
    }
    sight->points[sight->count++] = sight->center;
}

Sight *sight_new(Level *level, Point center, uint16_t radius)
{
    Sight *sight = allocate(sizeof(Sight));
    sight->center = center;
    sight->radius = radius;
    sight->count = 0;
    sight->points = allocate(area_size(radius));

    collect_points(sight, level);

    return sight;
}

bool sight_has(Sight *sight, Point point)
{
    for (uint16_t i = 0; i < sight->count; i++) {
        if (point_eq(point, sight->points[i])) {
            return true;
        }
    }
    return false;
}

void sight_update(Sight *sight, Level *level, Point center, uint16_t radius)
{
    bool changed = false;

    if (!point_eq(sight->center, center)) {
        sight->center = center;
        changed = true;
    }
    if (sight->radius != radius) {
        sight->radius = radius;
        sight->points = realloc(sight->points, area_size(radius));
        changed = true;
    }
    if (changed) {
        sight->count = 0;
        collect_points(sight, level);
    }
}

void sight_free(Sight *sight)
{
    release(sight->points);
    release(sight);
}
