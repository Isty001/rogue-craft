#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include "src/level/level.h"
#include "src/ncurses/ncurses.h"
#include "src/level/camera.h"
#include "config/config.h"
#include "src/player/inventory.h"


#define DIRECTION_KEY_NUM 4


static int KEY_DIRECTION_MAP[DIRECTION_KEY_NUM][2] = {
    {KEY_NORTH, NORTH},
    {KEY_EAST,  EAST},
    {KEY_SOUTH, SOUTH},
    {KEY_WEST,  WEST},
};

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


static inline Direction direction_lookup(int input)
{
    for (int i = 0; i < DIRECTION_KEY_NUM; ++i) {
        if (input == KEY_DIRECTION_MAP[i][0]) {
            return KEY_DIRECTION_MAP[i][1];
        }
    }

    return 0;
}

int main(void)
{
    init();

    Camera camera;
    Level *level = level_new(size_new(200, 200), LEVEL_STONE_CAVE);
    Player *player = player_new(level, &camera);
    player_position_on_level(player);
    int in;

    inventory_display(player->inventory);
    camera_update(player, WINDOW_MAIN);
    level_display(player);
    player_attributes_display(player);

    while (1) {
        if ((in = wgetch(WINDOW_MAIN))) {
            if ('q' == in) break;

            if (-1 != in && in != KEY_MOUSE) {
                player_move(player, direction_lookup(in));
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
