#include <time.h>
#include <ncurses.h>
#include "src/level/level.h"
#include "src/ncurses/ncurses.h"
#include "src/level/camera.h"
#include "data/config.h"
#include "src/player/inventory.h"


static void init(void)
{
    srand((unsigned) time(NULL));

    ncurses_init();
    mouse_init();
    cell_pool_init();
    item_pool_init();
}

static void cleanup(Player *player)
{
    ncurses_cleanup();
    item_pool_cleanup();
    cell_pool_cleanup();
    level_free(player->level);
    player_free(player);
}

int main(void)
{
    init();

    Camera camera;
    Level *level = level_new(size_new(200, 200), LEVEL_STONE_CAVE);
    Player *player = player_new(level, &camera);
    player_position_on_level(player);
    Input in;

    inventory_display(player->inventory);
    player_display_stats(player);

    while (1) {
        if ((in = wgetch(WINDOW_MAIN))) {
            if ('q' == in) break;

            if (in != KEY_MOUSE) {
                player_move(player, in);

                camera_update(player, WINDOW_MAIN);
                level_display(player);
            }

            if (in == KEY_NORTH || in == KEY_SOUTH) {
                napms(10);
            }

            if (in == KEY_MOUSE) {
                mouse_interact(player);
                inventory_display(player->inventory);
            }
        }
        flushinp();
        napms(70);
    }

    cleanup(player);

    return 0;
}
