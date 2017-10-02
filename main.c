#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include <parson.h>
#include "item/item_registry.h"
#include "util/logger.h"
#include "util/timer.h"
#include "sfx/sfx.h"
#include "memory/memory.h"
#include "player/camera.h"
#include "config/config.h"
#include "loop.h"


static void init(void)
{
    setlocale(LC_ALL, "en_US.UTF-8");
    srand((unsigned) time(NULL));
    json_set_allocation_functions((JSON_Malloc_Function) mem_alloc, mem_dealloc);

    log_init();
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
    item_registry_load();
    cell_registry_load();
    level_load();
}

static void cleanup(void)
{
    timer_cleanup();
    sfx_cleanup();
    ncurses_cleanup();
    panel_cleanup();
    item_pool_cleanup();
    cell_pool_cleanup();

    item_registry_unload();
    cell_registry_unload();
    level_unload();
    color_cleanup();

    list_node_pool_cleanup();
    profiler_cleanup();
    log_cleanup();
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

int main(int arc, char *argv[])
{
    env_setup(arc, argv);

    init();
    check_terminal();

    loop_run();

    cleanup();

    return 0;
}
