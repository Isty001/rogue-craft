#ifndef ROGUECRAFT_PLAYER_H
#define ROGUECRAFT_PLAYER_H


#include "../../config/config.h"
#include "../level/point.h"


typedef struct player {
    Level *level;
    unsigned int eyesight;
    struct {
        Point current;
        Point previous;
    } position;
    struct {
        Cell prototype;
        Cell *occupied;
    } cell;
} Player;

typedef enum {
    NORTH = KEY_NORTH,
    EAST = KEY_EAST,
    SOUTH = KEY_SOUTH,
    WEST = KEY_WEST
} Direction;


Player *player_new(Level *level);

void player_free(Player *player);

void player_move(Player *player, Direction direction);

bool player_can_see(Player *player, uint32_t y, uint32_t x);

void player_position_on_level(Player *player);


#endif
