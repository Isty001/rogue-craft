#include "sight.h"

/** The collected points wont always be exactly the same amount,
 * so just in case, allocate a bigger buffer */
#define area_size(r) (M_PI * sqr(r + 1)) * sizeof(Point)


#define setup_modifiers(d, inc, target, current, axis)  \
    d = abs(target.axis - current.axis);                \
    inc = current.axis < target.axis ? 1 : -1;


static void add_point_by_type(Sight *sight, Point point, SightType required_type)
{
    if (required_type == sight->type && !sight_has(sight, point)) {
        sight->points[sight->count++] = point;
    }
}

static void ray_cast(Sight *sight, Point target, Level *level)
{
    Point current = sight->center;
    Point prev = current;

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
        add_point_by_type(sight, current, ALL);

        if (!level_in_bounds(level, current) || SOLID == level->cells[current.y][current.x]->type) {
            add_point_by_type(sight, prev, EDGES);
            break;
        }
        prev = current;
    }
}

/**
 * @see http://www.roguebasin.com/index.php?title=Ray_casting
 * @see https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
 */
static void collect_points(Sight *sight, Level *level)
{
    int16_t y, x;

    for (double deg = 0; deg < 360; deg++) {

        y = sight->center.y + (int16_t) (sight->radius * sin(deg));
        x = sight->center.x + (int16_t) (sight->radius * cos(deg));

        if (y >= 0 && x >= 0) {
            ray_cast(sight, point_new(y, x), level);
        }
    }
    sight->points[sight->count++] = sight->center;
}

Sight *sight_new(Level *level, Point center, uint16_t radius, SightType type)
{
    Sight *sight = allocate(sizeof(Sight));
    sight->center = center;
    sight->radius = radius;
    sight->count = 0;
    sight->points = allocate(area_size(radius));
    sight->type = type;

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

SightChange sight_update(Sight *sight, Level *level, Point center, uint16_t radius)
{
    bool changed = false;

    if (!point_eq(sight->center, center)) {
        sight->center = center;
        changed = true;
    }
    if (sight->radius != radius) {
        sight->radius = radius;
        sight->points = reallocate(sight->points, area_size(radius));
        changed = true;
    }
    if (changed) {
        sight->count = 0;
        collect_points(sight, level);
    }

    return changed ? UPDATED : REMAINED;
}

void sight_free(Sight *sight)
{
    release(sight->points);
    release(sight);
}
