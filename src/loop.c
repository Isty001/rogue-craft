#include <ncurses.h>
#include "util/timer.h"
#include "loop.h"
#include "config.h"
#include "player/inventory/inventory.h"
#include "level/camera.h"


#define TIMEOUT 85
#define VERTICAL_TIMEOUT 35


static void render(Player *player)
{
    panel_hide();
    level_display(player);
    inventory_shortcut_display(player->inventory);
    player_attributes_display(player);
    profiler_display();
    panel_show();
}

static void update(int input, Player *player)
{
    input_process(input, player);

    if (input == KEY_NORTH || input == KEY_SOUTH) {
        napms(VERTICAL_TIMEOUT);
    }

    camera_update(player, WINDOW_MAIN);
    player_sight_update(player);

    render(player);
}

void loop_run(Player *player)
{
    int input;

    TimerArgs args = {.ptr = {player, &PLAYER_STATE_CONFIG}};
    timer_new(200, player_state_update, args);

    while (1) {
        if (-1 != (input = wgetch(WINDOW_MAIN))) {
            if (KEY_ESCAPE == input) {
                if (panel_is_open()) {
                    panel_close_top(input, player);
                } else {
                    break;
                }
            }
        }
        update(input, player);
        render(player);
        timer_tick();

        napms(TIMEOUT);
    }
}
