#include <ncurses/ncurses.h>
#include "sfx/sfx.h"
#include "util/timer.h"
#include "config.h"


void player_position_on_level(Player *player)
{
    Cell ***cells = player->level->cells;
    Point current = player->position.current;
    Point previous = player->position.previous;

    cells[previous.y][previous.x] = player->cell.occupied;

    player->cell.occupied = cells[current.y][current.x];
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

static void update_player(Player *player, Point target)
{
    player->position.previous = player->position.current;
    player->position.current = target;
    player->state.modifiers.traveled++;
    player->movement.moving = false;
}

static void move_to(Player *player)
{
    Direction direction = player->movement.direction;
    Point target = point_move(player->position.current, direction, 1);

    if (!player->movement.moving) {
        return;
    }

    if (can_move_to(player, target)) {
        update_player(player, target);
        sfx_play_step(player->level->cells[target.y][target.x]);
        player_position_on_level(player);
    }
}

EventError player_move(InputEvent *event)
{
    Direction direction;

    if (KEY_MOUSE == event->input || 0 == (direction = direction_lookup(event->input))) {
        return EE_CONTINUE;
    }

    event->player->movement.direction = direction;
    event->player->movement.moving = true;

    return EE_BREAK;
}

void player_init_movement(Player *player)
{
    PlayerMovement *movement = &player->movement;
    uint16_t speed = player->attributes.speed;

    movement->timer = timer_new(PLAYER_SPEED_MAX - speed, move_to, player);
    movement->moving = false;
}
