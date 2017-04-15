#include <ncursesw/ncurses.h>
#include "loop.h"
#include "ncurses/ncurses.h"
#include "../config/config.h"
#include "player/inventory.h"
#include "level/camera.h"


#define TIMEOUT 60
#define VERTICAL_TIMEOUT 20


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
    int i = 0;

    while (1) {
        if ((input = wgetch(WINDOW_MAIN))) {
            if (KEY_ESCAPE == input) {
                if (panel_is_open()) {
                    panel_close_top(input, player);
                } else {
                    break;
                }
            }
            update(input, player);
            render(player);
        }
        napms(TIMEOUT);

        if (i++ == 10) {
            player_state_update(player, &PLAYER_STATE_CONFIG);
            i = 0;
        }
    }
}
