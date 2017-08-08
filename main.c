#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include <parson.h>
#include "util/timer.h"
#include "sfx/sfx.h"
#include "memory/memory.h"
#include "level/camera.h"
#include "config/config.h"
#include "player/inventory/inventory.h"
#include "loop.h"


static void init(void)
{
    setlocale(LC_ALL, "en_US.UTF-8");
    srand((unsigned) time(NULL));
    json_set_allocation_functions((JSON_Malloc_Function) mem_alloc, mem_dealloc);

    profiler_init();
    timer_init();
    sfx_init();
    ncurses_init();
    mouse_init();
    panel_init();

    list_node_pool_init();
    item_pool_init();
    cell_pool_init();

    color_init();
    item_load();
    cell_load();
    level_load();
}

static void cleanup(Player *player)
{
    timer_cleanup();
    sfx_cleanup();
    ncurses_cleanup();
    panel_cleanup();
    item_pool_cleanup();
    level_free(player->level);
    cell_pool_cleanup();

    player_free(player);
    item_unload();
    cell_unload();
    level_unload();
    color_cleanup();

    list_node_pool_cleanup();
    profiler_cleanup();
}


static void check_terminal(void)
{
    if (!has_mouse()) {
        fatal("Your terminal or ncurses version has no mouse support.")
    }

    if (!has_colors() || !can_change_color()) {
        fatal("Please make sure your terminal has 256 color support.");
    }
}

static Player *load_player(void)
{
    Camera camera;
    Size size = size_new(300, 300);
    Level *level = level_new(size);
    Player *player = player_new(level, &camera);
    player_position_on_level(player);

    return player;
}

int main(int arc, char *argv[])
{
    env_setup(arc, argv);

    init();
    check_terminal();

    Player *player = load_player();
    loop_run(player);
    cleanup(player);

    return 0;
}
