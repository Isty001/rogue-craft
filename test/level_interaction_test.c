#include "unit_test.h"
#include "../src/level/level.h"
#include "../src/level/camera.h"
#include "../data/item_config.h"
#include "../src/player/inventory.h"
#include "fixture.h"


MU_TEST(test_item_pickup)
{
    Level *level = fixture_level();
    Item *item = item_clone(&ITEM_CONSUMABLE_HP);
    level->cells[0][0]->data = item;
    level->cells[0][0]->type = ITEM_;

    Camera camera = {
        .position = point_new(0, 0),
        .size = level->size
    };

    Player *player = player_new(level, &camera);
    player->position.current = point_new(0, 1);

    Point click = point_new(0, 0);

    level_interact(player, click);
    mu_assert(item == player->inventory->items[0], "Items are not the same");

    level_interact(player, click);
    mu_assert(item == player->inventory->items[0], "Items are not the same");
    mu_assert(NULL == player->inventory->items[1], "There should be no item");

    mu_assert_int_eq(HOLLOW, level->cells[0][0]->type);

    player_free(player);
    fixture_level_free(level);
    item_free(item);
}

void run_level_interaction_test(void)
{
    TEST_NAME("Level Interaction");

    MU_RUN_TEST(test_item_pickup);

    MU_REPORT();
}
