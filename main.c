#include <stdio.h>
#include <time.h>
#include "src/level/level.h"
#include "src/display/ncurses.h"
#include "src/player/player.h"
#include "src/level/camera.h"


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

    Level *level = level_new(size_new(100, 200), LEVEL_STONE_CAVE);
    Player *player = player_new(level);
    player_position_on_level(player);
    Camera camera = camera_new(player, WINDOW_MAIN);

    int ch;

    while (1) {
        if ((ch = wgetch(WINDOW_MAIN))) {
            player_move(player, ch);
            player_position_on_level(player);

            if ('q' == ch) {
                break;
            }
            flushinp();
        }
        level_display(player, &camera);
        napms(1000 / 15);
    }

    cleanup();

    return 0;
}
