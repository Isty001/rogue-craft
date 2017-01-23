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

    Level *level = level_new(size_new(500, 200), LEVEL_STONE_CAVE);
    Player *player = player_new(level);
    player_position_on_level(player);
    Camera camera;

    int ch;

    while (1) {
        if ((ch = wgetch(WINDOW_MAIN))) {
            if ('q' == ch) {
                break;
            }

            player_move(player, ch);
            player_position_on_level(player);

            camera_update(&camera, player, WINDOW_MAIN);
            level_display(player, &camera);

            if (ch == KEY_NORTH || ch == KEY_SOUTH) {
                napms(10);
            }

        }
        flushinp();
        napms(70);
    }

    cleanup();

    return 0;
}
