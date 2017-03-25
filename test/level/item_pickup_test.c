#include "../unit_test.h"
#include "../../src/level/level.h"
#include "../../src/level/camera.h"
#include "../../config/config.h"
#include "../../src/player/inventory.h"
#include "../fixture.h"


MU_TEST(test_item_pickup)
{
    Level *level = fixture_level();
    Item item;
    level->cells[0][0]->item = &item;
    level->cells[0][0]->type = ITEM_;

    Camera camera = {
        .position = point_new(0, 0),
        .size = level->size
    };

    Player *player = player_new(level, &camera);
    List *items = player->inventory->items;
    player->position.current = point_new(0, 1);

    InteractionEvent event = {
        .cell = level->cells[0][0],
        .player = player,
        .point = point_new(0, 0)
    };

    item_pickup(&event);
    mu_assert(&item == items->head(items), "Items are not the same");

    item_pickup(&event);
    mu_assert(&item == items->head(items), "Items are not the same");
    mu_assert(NULL == items->get(items, 1), "There should be no items");

    mu_assert_int_eq(HOLLOW, level->cells[0][0]->type);
    mu_assert(level->cells[0][0]->in_registry, "");

    player_free(player);
    fixture_level_free(level);
}

MU_TEST(test_bounds)
{
    Level *level = fixture_level();

    mu_assert(level_in_bounds(level, point_new(1, 0)), "Should be in bounds");
    mu_assert(level_in_bounds(level, point_new(1, 2)), "Should be in bounds");

    mu_assert(false == level_in_bounds(level, point_new(-1, 2)), "Should not be in bounds");
    mu_assert(false == level_in_bounds(level, point_new(1, 3)), "Should not be in bounds");

    fixture_level_free(level);
}

void run_level_interaction_test(void)
{
    TEST_NAME("Item Pickup");

    MU_RUN_TEST(test_item_pickup);
    MU_RUN_TEST(test_bounds);

    MU_REPORT();
}
