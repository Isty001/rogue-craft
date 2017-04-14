#include "sight.h"

/** The collected points wont always be exactly the same amount,
 * so just in case, allocate a bigger buffer */
#define area_size(r) (M_PI * sqr(r + 1)) * sizeof(Point)


#define setup_modifiers(d, inc, target, current, axis)  \
    d = abs(target.axis - current.axis);                \
    inc = current.axis < target.axis ? 1 : -1;


static void add_point(Sight *sight, Point point)
{
    if (!sight_has(sight, point)) {
        sight->points[sight->count++] = point;
    }
}

static void ray_cast(Sight *sight, Point target)
{
    Point current = sight->center;
    Point prev = current;
    Cell *cell;
    bool is_solid = false;
    bool in_bounds;

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
        in_bounds = level_in_bounds(sight->level, current);

        if (in_bounds) {
            cell = sight->level->cells[current.y][current.x];
            is_solid = SOLID == cell->type;

            if (ALL == sight->type) {
                add_point(sight, current);
            } else if (EDGES == sight->type && is_solid) {
                add_point(sight, current);
                add_point(sight, prev);
            }
            cell->lighted = true;
        }
        if (!in_bounds || is_solid) {
            break;
        }
        prev = current;
    }
}

/**
 * @see http://www.roguebasin.com/index.php?title=Ray_casting
 * @see https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
 */
static void collect_points(Sight *sight)
{
    int16_t y, x;

    for (double deg = 0; deg < 360; deg += 3) {

        y = sight->center.y + (int16_t) (sight->radius * sin(deg));
        x = sight->center.x + (int16_t) (sight->radius * cos(deg));

        if (y >= 0 && x >= 0) {
            ray_cast(sight, point_new(y, x));
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
    sight->level = level;

    collect_points(sight);

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

SightChange sight_update(Sight *sight, Point center, uint16_t radius)
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
        collect_points(sight);
    }

    return changed ? UPDATED : REMAINED;
}

void sight_free(Sight *sight)
{
    release(sight->points);
    release(sight);
}
