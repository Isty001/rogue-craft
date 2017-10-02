#include "../unit_test.h"
#include "../../src/level/level.h"
#include "player/camera.h"
#include "../../config/config.h"
#include "../../src/inventory/inventory.h"
#include "../fixture.h"


static LevelInteractionEvent create_event(Player *player, Level *level)
{
    return (LevelInteractionEvent) {
        .cell = level->cells[0][0],
        .player = player,
        .point = point_new(0, 0),
        .player_distance = 1
    };
}

MU_TEST(test_item_pickup)
{
    Level *level = fixture_level();
    Item item;
    item.occupied_cell = NULL;
    item.type = ITEM_TOOL;

    level->cells[0][0]->item = &item;
    level->cells[0][0]->type = CELL_ITEM;

    Player *player = player_new(level);
    Item **items = player->inventory->items;
    player->position.current = point_new(0, 1);

    LevelInteractionEvent event = create_event(player, level);

    item_pickup(&event);
    mu_assert(&item == items[0], "Items are not the same");

    event.cell = level->cells[0][0];
    item_pickup(&event);
    mu_assert(&item == items[0], "Items are not the same");
    mu_assert(NULL == items[1], "There should be no items");

    mu_assert_int_eq(CELL_HOLLOW, level->cells[0][0]->type);
    mu_assert(level->cells[0][0]->in_registry, "");

    fixture_level_free(level);
    player_free(player);
}

MU_TEST(test_pickup_lighting)
{
    Level *level = fixture_level();
    Lighting *lighting = lighting_new(level, point_new(0, 0), 5, 100);
    Cell occupied = {.type = CELL_HOLLOW};
    Item item = {
        .type = ITEM_LIGHT_SOURCE,
        .occupied_cell = &occupied,
        .light_source = {.lighting = lighting}
    };
    level->cells[0][0] = cell_from_item(&item);

    Player player = {
        .inventory = inventory_new(1),
        .level = level
    };

    LevelInteractionEvent event = create_event(&player, level);

    item_pickup(&event);
    mu_assert(&occupied == level->cells[0][0], "");
    mu_assert(NULL == item.occupied_cell, "");

    fixture_level_free(level);
    inventory_free(player.inventory);
    lighting_free(lighting);
}

void run_level_interaction_test(void)
{
    TEST_NAME("Item Pickup");

    MU_RUN_TEST(test_item_pickup);
    MU_RUN_TEST(test_pickup_lighting);

    MU_REPORT();
}
