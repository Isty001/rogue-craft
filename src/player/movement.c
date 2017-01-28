#include "player.h"
#include "../../data/config.h"


void player_position_on_level(Player *player)
{
    Cell ***cells = player->level->cells;
    Point current = player->position.current;
    Point previous = player->position.previous;

    cells[previous.y][previous.x] = player->cell.previous;

    player->cell.previous = cells[current.y][current.x];
    cells[current.y][current.x] = &player->cell.prototype;
}

static inline bool can_move_to(Player *player, Point target)
{
    Level *level = player->level;

    return
        (
            in_range(target.x, level->bounds.x) &&
            in_range(target.y, level->bounds.y)
        ) &&
        (
            SOLID != level->cells[target.y][target.x]->type
        );
}

void player_move(Player *player, Input input)
{
    Point target = player->position.current;

    switch (input) {
        case KEY_NORTH:
            target.y -= 1;
            break;
        case KEY_SOUTH:
            target.y += 1;
            break;
        case KEY_EAST:
            target.x += 1;
            break;
        case KEY_WEST:
            target.x -= 1;
            break;
    }

    if (can_move_to(player, target)) {
        player->position.previous = player->position.current;
        player->position.current = target;
    }
}

bool player_can_see(Player *player, uint16_t y, uint16_t x)
{
    Point pos = player->position.current;
    uint16_t dist = square(pos.x - x) + square(pos.y - y);

    return dist <= square(player->eyesight);
}
