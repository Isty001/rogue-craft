#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include <parson.h>
#include "src/level/level.h"
#include "src/level/camera.h"
#include "config/config.h"
#include "src/player/inventory.h"


static void init(void)
{
    setlocale(LC_ALL, "en_US.UTF-8");
    srand((unsigned) time(NULL));
    json_set_allocation_functions((JSON_Malloc_Function) allocate, release);

    cache_init();

    profiler_init();
    ncurses_init();

    list_node_pool_init();
    item_pool_init();
    cell_pool_init();
    lighted_cell_pool_init();

    color_init();
    item_load();
    cell_load();
    level_load();

    mouse_init();
}

static void cleanup(Player *player)
{
    ncurses_cleanup();
    item_pool_cleanup();
    cell_pool_cleanup();
    lighted_cell_pool_cleanup();

    level_free(player->level);
    player_free(player);
    item_unload();
    cell_unload();
    level_unload();
    color_cleanup();

    list_node_pool_cleanup();
    profiler_cleanup();
}

static void update(Player *player)
{
    camera_update(player, WINDOW_MAIN);
    player_sight_update(player);
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
    Level *level = level_new(size);
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


        if (i++ == 10) {
            player_state_update(player, &PLAYER_STATE_CONFIG);
            i = 0;
        }
    }
    cleanup(player);

    return 0;
}
