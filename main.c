#include <stdio.h>
#include <time.h>
#include "src/level/level.h"
#include "src/display/ncurses.h"
#include "src/player/player.h"
#include "src/level/camera.h"
#include "data/config.h"


static void init(void)
{
    srand((unsigned) time(NULL));

    ncurses_init();
}

static void cleanup(void)
{
    ncurses_cleanup();
}

int main(void)
{
    init();

    Level *level = level_new(size_new(200, 200), LEVEL_STONE_CAVE);
    Player *player = player_new(level);
    player_position_on_level(player);
    Camera camera;

    Input in;

    while (1) {
        if ((in = wgetch(WINDOW_MAIN))) {
            if ('q' == in) {
                break;
            }

            player_move(player, in);
            player_position_on_level(player);

            camera_update(&camera, player, WINDOW_MAIN);
            level_display(player, &camera);

            if (in == KEY_NORTH || in == KEY_SOUTH) {
                napms(10);
            }

        }
        flushinp();
        napms(70);
    }

    cleanup();

    return 0;
}
