#include "ui/ncurses.h"
#include "util/timer.h"
#include "sfx/sfx.h"
#include "config.h"
#include "player.h"



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

static void move_to(TimerArgs *args)
{
    Player *player = args->ptr[0];
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

void player_move(InputEvent *event)
{
    Direction direction = direction_lookup(event->input);

    event->player->movement.direction = direction;
    event->player->movement.moving = true;
}

void player_init_movement(Player *player)
{
    PlayerMovement *movement = &player->movement;
    uint16_t timeout = PLAYER_SPEED_MAX - player->attributes.speed;

    TimerArgs args = {.ptr = {player}};
    movement->timer = timer_new(timeout * 2, move_to, args);
    movement->moving = false;
}
