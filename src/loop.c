#include <ncurses.h>
#include <gamestate/gamestate.h>
#include "ui/menu/main_menu.h"
#include "keymap.h"
#include "ui/panel.h"
#include "inventory/inventory.h"


#define MAX_TIMEOUT 50


typedef enum {
    LOOP_CONTINUE,
    LOOP_BREAK,
} LoopAction;


static void update(int input, Player *player)
{
    input_process(input, player);

    camera_update(&player->camera, player->position.current, player->level, WINDOW_MAIN);
    player_sight_update(player);
}

static void render(Player *player)
{
    panel_hide();
    level_display(player);
    inventory_shortcut_display(player->inventory);
    player_state_display(player);
    profiler_display();
    panel_show();
}

static void loop_timeout(struct timespec *start, struct timespec *end)
{
    clock_gettime(CLOCK_MONOTONIC, end);
    timer_tick(*end);

    napms(max(0, MAX_TIMEOUT - time_diff_ms((*end), (*start))));
}

static LoopAction display_main_menu(GameState *state)
{
    switch (menu_main_display(state)) {
        case MAIN_MENU_NEW_GAME:
            game_state_with_new_player(state);
            state->in_game = true;
            break;
        case MAIN_MENU_RESUME:
            break;
        case MAIN_MENU_QUIT:
            if (state->in_game) {
                state->in_game = false;
            } else {
                return LOOP_BREAK;
            }
    }
    ncurses_display_game_windows();

    return LOOP_CONTINUE;
}

void loop_run(void)
{
    int input;
    struct timespec start;
    struct timespec end;
    GameState *state = game_state_new();

    while (1) {
        clock_gettime(CLOCK_MONOTONIC, &start);

        if (-1 != (input = wgetch(WINDOW_MAIN))) {
            if (KEY_QUIT == input) {
                if (panel_is_open()) {
                    panel_close_top(input, state->player);
                } else {
                    if (LOOP_BREAK == display_main_menu(state)) break;
                }
            }
        }

        if (state->in_game) {
            update(input, state->player);
            render(state->player);
            loop_timeout(&start, &end);
        } else {
            if (LOOP_BREAK == display_main_menu(state)) break;
        }
    }

    game_state_free(state);
}
