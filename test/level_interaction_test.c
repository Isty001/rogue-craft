#include "unit_test.h"
#include "../src/level/level.h"
#include "../src/level/camera.h"
#include "../config/config.h"
#include "../src/player/inventory.h"
#include "fixture.h"


MU_TEST(test_item_pickup)
{
    Level *level = fixture_level();
    Item item;
    level->cells[0][0]->data = &item;
    level->cells[0][0]->type = ITEM_;

    Camera camera = {
        .position = point_new(0, 0),
        .size = level->size
    };

    Player *player = player_new(level, &camera);
    player->position.current = point_new(0, 1);

    Point click = point_new(0, 0);

    level_interact(player, click);
    mu_assert(&item == player->inventory->items[0], "Items are not the same");

    level_interact(player, click);
    mu_assert(&item == player->inventory->items[0], "Items are not the same");
    mu_assert(NULL == player->inventory->items[1], "There should be no item");

    mu_assert_int_eq(HOLLOW, level->cells[0][0]->type);

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
    TEST_NAME("Level Interaction");

    MU_RUN_TEST(test_item_pickup);
    MU_RUN_TEST(test_bounds);

    MU_REPORT();
}
