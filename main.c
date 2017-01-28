#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <mem_pool.h>
#include <ncurses.h>
#include "src/level/level.h"
#include "src/ncurses/ncurses.h"
#include "src/level/camera.h"
#include "data/config.h"


static void init(void)
{
    srand((unsigned) time(NULL));

    ncurses_init();
    mouse_init();
}

static void cleanup(void)
{
    ncurses_cleanup();
}

int main(void)
{
    init();

    Camera camera;
    Level *level = level_new(size_new(200, 200), LEVEL_STONE_CAVE);
    Player *player = player_new(level, &camera);
    player_position_on_level(player);
    Input in;

    while (1) {
        if ((in = wgetch(WINDOW_MAIN))) {
            if ('q' == in) {
                break;
            }

            player_move(player, in);
            player_position_on_level(player);

            camera_update(player, WINDOW_MAIN);
            level_display(player);

            if (in == KEY_NORTH || in == KEY_SOUTH) {
                napms(10);
            }

            if (in == KEY_MOUSE) {
                mouse_interact(player);
            }
        }
        flushinp();
        napms(70);
    }

    cleanup();

    return 0;
}
