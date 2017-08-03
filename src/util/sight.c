#include <math.h>
#include <util/util.h>
#include "util/memory.h"
#include "sight.h"


#define setup_modifiers(d, inc, target, current, axis)  \
    d = abs(target.axis - current.axis);                \
    inc = current.axis < target.axis ? 1 : -1;


static void add_point(Sight *sight, Point point)
{
    if (!sight_has(sight, point)) {
        quadtree_insert(sight->points, point.x, point.y, NULL);
    }
}

static void ray_cast(Sight *sight, Point target)
{
    Point current = sight->center;
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
            is_solid = CELL_SOLID == cell->type;

            add_point(sight, current);
        }
        if (!in_bounds || is_solid) {
            break;
        }
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
    add_point(sight, sight->center);
}

static quadtree_t *quadtree_for(Sight *sight)
{
    Point center = sight->center;
    uint16_t radius = sight->radius;

    return quadtree_new(
        center.x - radius, center.y - radius,
        center.x + radius, center.y + radius
    );
}

Sight *sight_new(Level *level, Point center, uint16_t radius)
{
    Sight *sight = mem_alloc(sizeof(Sight));
    sight->center = center;
    sight->radius = radius;
    sight->points = quadtree_for(sight);
    sight->level = level;

    collect_points(sight);

    return sight;
}

bool sight_has(Sight *sight, Point point)
{
    if (sight->points->length) {
        return (bool) quadtree_search(sight->points, point.x, point.y);
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
        changed = true;
    }
    if (changed) {
        quadtree_free(sight->points);
        sight->points = quadtree_for(sight);
        collect_points(sight);
    }

    return changed ? UPDATED : REMAINED;
}

void sight_free(Sight *sight)
{
    quadtree_free(sight->points);
    mem_dealloc(sight);
}
