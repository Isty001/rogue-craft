#include <config.h>
#include "memory/memory.h"
#include "gamestate.h"


static Player *load_player(void)
{
    Size size = size_new(300, 300);
    Level *level = level_new(size);
    Player *player = player_new(level);
    player_position_on_level(player);

    return player;
}

GameState *game_state_new(void)
{
    GameState *state = mem_alloc(sizeof(GameState));
    state->in_game = false;
    state->player = NULL;

    return state;
}

static void free_player_context(GameState *state)
{
    Player *player = state->player;

    if (player) {
        level_free(player->level);
        player_free(player);
    }
}

void game_state_with_new_player(GameState *state)
{
    free_player_context(state);
    state->player = load_player();
}

void game_state_free(GameState *state)
{
    free_player_context(state);
    mem_dealloc(state);
}
