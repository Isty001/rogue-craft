#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <parson.h>
#include "../src/color.h"
#include "../src/player/item.h"

void run_player_movement_test(void);
void run_point_test(void);
void run_util_test(void);
void run_player_manager_test(void);
void run_camera_test(void);
void run_level_test(void);
void run_consumable_test(void);
void run_inventory_test(void);
void run_level_interaction_test(void);
void run_cell_test(void);
void run_player_hit_test(void);
void run_player_state_test(void);
void run_sight_test(void);
void run_json_test(void);
void run_item_loader_test(void);
void run_storage_test(void);
void run_cache_test(void);
void run_cell_loader_test(void);
void run_level_loader_test(void);
void run_color_cache_test(void);
void run_light_source_placement_test(void);


static void init(void)
{
    setlocale(LC_ALL, "en_US.UTF-8");
    srand((unsigned int) time(NULL));
    json_set_allocation_functions((JSON_Malloc_Function) allocate, release);

    profiler_init();
    list_node_pool_init();
    cache_init();
    item_pool_init();
    cell_pool_init();
    lighted_cell_pool_init();
}

static void cleanup(void)
{
    cell_pool_cleanup();
    lighted_cell_pool_cleanup();
    item_pool_cleanup();
    list_node_pool_cleanup();
    profiler_cleanup();
}

int main(void)
{
    init();

    run_player_movement_test();
    run_point_test();
    run_util_test();
    run_player_manager_test();
    run_camera_test();
    run_level_test();
    run_consumable_test();
    run_level_interaction_test();
    run_cell_test();
    run_player_hit_test();
    run_inventory_test();
    run_player_state_test();
    run_sight_test();
    run_json_test();
    run_item_loader_test();
    run_storage_test();
    run_cache_test();
    run_cell_loader_test();
    run_level_loader_test();
    run_color_cache_test();
    run_light_source_placement_test();

    cleanup();

    return 0;
}
