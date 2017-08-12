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
    player_state_display(player);
    profiler_display();
    panel_show();
}

static void update(int input, Player *player)
{
    input_process(input, player);

    camera_update(player, WINDOW_MAIN);
    player_sight_update(player);

    render(player);
}

static void loop_timeout(int input, time_t start)
{
    napms(max(0, TIMEOUT - (time_now_ms() - start)));

    if (input == KEY_NORTH || input == KEY_SOUTH) {
        napms(VERTICAL_TIMEOUT);
    }
}

void loop_run(Player *player)
{
    int input;
    time_t start;

    TimerArgs args = {.ptr = {player, &PLAYER_STATE_CONFIG}};
    timer_new(200, player_state_update, args);

    while (1) {
        start = time_now_ms();

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

        loop_timeout(input, start);
    }
}
