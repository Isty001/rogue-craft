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
    level_free(player->level);
    cell_pool_cleanup();
    lighted_cell_pool_cleanup();

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
    panel_hide();
    level_display(player);
    inventory_shortcut_display(player->inventory);
    player_attributes_display(player);
    profiler_display();
    panel_show();
}

static void check_env(void)
{
    if (!has_mouse()) {
        fatal("Your terminal or ncurses version has no mouse support.")
    }

    if (!has_colors() || !can_change_color()) {
        fatal("Please make sure that your terminal has 256 color support.");
    }
}

int main(void)
{
    init();
    check_env();

    Camera camera;
    Size size = size_new(300, 300);
    Level *level = level_new(size);
    Player *player = player_new(level, &camera);
    player_position_on_level(player);

    update(player);
    render(player);

    int in;
    int i = 0;

    while (1) {
        if ((in = wgetch(WINDOW_MAIN))) {
            if (KEY_F(2) == in) {
                if (panel_is_open()) {
                    panel_close_top();
                } else {
                    break;
                }
            }
            input_process(in, player);

            if (in == KEY_NORTH || in == KEY_SOUTH) {
                napms(20);
            }
            update(player);
            render(player);
        }
        napms(60);

        if (i++ == 10) {
            player_state_update(player, &PLAYER_STATE_CONFIG);
            i = 0;
        }
    }
    cleanup(player);

    return 0;
}
