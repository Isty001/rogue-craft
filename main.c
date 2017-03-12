#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include <parson.h>
#include "src/level/level.h"
#include "src/ncurses/ncurses.h"
#include "src/level/camera.h"
#include "config/config.h"
#include "src/player/inventory.h"
#include "src/worker/worker.h"
#include "src/level/lighting.h"
#include "src/storage/cache.h"

static void init(void)
{
    setlocale(LC_ALL, "en_US.UTF-8");
    srand((unsigned) time(NULL));
    json_set_allocation_functions((JSON_Malloc_Function) allocate, release);

    profiler_init();
    cache_init(DIR_CACHE);
    item_load(DIR_CONFIG_ITEMS);

    ncurses_init();
    mouse_init();
    cell_pool_init();
    item_pool_init();
    lighted_cell_pool_init();
    message_pool_init();
    worker_init();
}

static void cleanup(Player *player)
{
    ncurses_cleanup();
    worker_cleanup();
    message_pool_cleanup();
    item_pool_cleanup();
    cell_pool_cleanup();
    lighted_cell_pool_cleanup();
    level_free(player->level);
    player_free(player);
    item_unload();
    profiler_cleanup();
}

static void update(Player *player)
{
    camera_update(player, WINDOW_MAIN);
    player_update_sight(player);
}

static void render(Player *player)
{
    level_display(player);
    inventory_shortcut_display(player->inventory);
    player_attributes_display(player);
    profiler_display();
}

int main(void)
{
    init();

    if (!has_colors() || !can_change_color()) {
        fatal("Please make sure that your terminal has 256 color support.");
    }

    Camera camera;
    Size size = size_new(300, 300);
    Level *level = level_new(size, &LEVEL_CAVE);
    Player *player = player_new(level, &camera);
    player_position_on_level(player);
    int in;

    liquid_add(level);

    update(player);
    render(player);

    int i = 0;

    while (1) {
        if ((in = wgetch(WINDOW_MAIN))) {
            if (KEY_F(2) == in) break;

            input_process(in, player);

            if (in == KEY_NORTH || in == KEY_SOUTH) {
                napms(20);
            }

            update(player);
            render(player);
        }
        flushinp();
        napms(60);

        if (i++ == 30) {
            message_player_state_send(player);
            i = 0;
        }
    }
    cleanup(player);

    return 0;
}
