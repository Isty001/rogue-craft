#include "player.h"


#define setup_modifiers(d, inc, target, current, axis)  \
    d = abs(target.axis - current.axis);                \
    inc = current.axis < target.axis ? 1 : -1;


static inline bool in_sight(Player *player, Point point)
{
    for (uint16_t i = 0; i < player->sight.visible_count; i++) {
        if (point_eq(point, player->sight.visible[i])) {
            return true;
        }
    }
    return false;
}

/**
 * @see https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
 */
static inline void collect_at(Player *player, Point target)
{
    Level *level = player->level;
    Point current = player->position.current;

    int dx, inc_x, dy, inc_y;
    setup_modifiers(dx, inc_x, target, current, x);
    setup_modifiers(dy, inc_y, target, current, y);

    int err = (dx > dy ? dx : -dy) / 2, err_prev;

    while (!point_eq(current, target)) {
        if (level_in_bounds(level, current)) {
            if (!in_sight(player, current)) {
                player->sight.visible[player->sight.visible_count++] = current;
            }
            if (SOLID == level->cells[current.y][current.x]->type) {
                break;
            }
        }

        err_prev = err;
        if (err_prev > -dx) {
            err -= dy;
            current.x += inc_x;
        }
        if (err_prev < dy) {
            err += dx;
            current.y += inc_y;
        }
    }
}

/**
 * @see http://www.roguebasin.com/index.php?title=Ray_casting
 */
void player_calculate_sight(Player *player)
{
    player->sight.visible_count = 0;

    Point target;
    Point player_pos = player->position.current;
    uint16_t radius_y = player->sight.radius;
    uint16_t radius_x = radius_y * 1.5;

    for (float deg = 0; deg < 360.0; deg += 1.0) {
        target = point_new(
            player_pos.y + (radius_y * sin(deg)),
            player_pos.x + (radius_x * 1.5 * cos(deg))
        );
        collect_at(player, target);
    }
}

bool player_can_see(Player *player, Point point)
{
    Point player_pos = player->position.current;
    bool in_distance = player->sight.radius >= point_distance(point, player_pos);

    if (!in_distance) return false;

    return in_sight(player, point);
}
