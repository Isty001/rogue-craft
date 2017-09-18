#ifndef ROGUE_CRAFT_GAMESTATE_H
#define ROGUE_CRAFT_GAMESTATE_H


#include <stdbool.h>
#include <player/player.h>


typedef struct {
    bool in_game;
    Player *player;
} GameState;


GameState *game_state_new(void);

void game_state_with_new_player(GameState *state);

void game_state_free(GameState *state);


#endif
