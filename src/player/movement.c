#include "../../config/config.h"


static int KEY_MAIN_DIRECTION_MAP[4][2] = {
    {KEY_NORTH, NORTH},
    {KEY_EAST,  EAST},
    {KEY_SOUTH, SOUTH},
    {KEY_WEST,  WEST},
};

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
        level_in_bounds(level, target)
        &&
        SOLID != level->cells[target.y][target.x]->type;
}

static inline Direction direction_lookup(int input)
{
    if (KEY_MOUSE == input) {
        return -1;
    }

    for (int i = 0; i < 4; ++i) {
        if (input == KEY_MAIN_DIRECTION_MAP[i][0]) {
            return (Direction) KEY_MAIN_DIRECTION_MAP[i][1];
        }
    }
    return -1;
}

EventError player_move(InputEvent *event)
{
    Direction direction;

    if (!(direction = direction_lookup(event->input))) {
        return EE_CONTINUE;
    }

    Player *player = event->player;
    Point target = point_move(player->position.current, direction, 1);

    if (can_move_to(player, target)) {
        player->position.previous = player->position.current;
        player->position.current = target;
        player->attributes.modifiers.traveled++;

        player_position_on_level(player);
    }

    return EE_BREAK;
}
