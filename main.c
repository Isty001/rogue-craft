#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include "src/level/level.h"
#include "src/ncurses/ncurses.h"
#include "src/level/camera.h"
#include "config/config.h"
#include "src/player/inventory.h"


static void init(void)
{
    setlocale(LC_ALL, "en_US.UTF-8");
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

static void render(Player *player)
{
    camera_update(player, WINDOW_MAIN);
    level_display(player);
    inventory_display(player->inventory);
    player_attributes_display(player);
}

int main(void)
{
    init();

    Camera camera;
    Level *level = level_new(size_new(200, 200), LEVEL_CAVE);
    Player *player = player_new(level, &camera);
    player_position_on_level(player);
    int in;

    render(player);

    while (1) {
        if ((in = wgetch(WINDOW_MAIN))) {
            if (KEY_F(2) == in) break;

            input_process(in, player);

            if (in == KEY_NORTH || in == KEY_SOUTH) {
                napms(20);
            }

            render(player);
        }
        flushinp();
        napms(70);
    }

    cleanup(player);

    return 0;
}
