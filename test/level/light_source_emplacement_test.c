#include "../unit_test.h"
#include "../../src/player/player.h"
#include "../fixture.h"
#include "../../src/player/inventory.h"


static InteractionEvent TEST_EVENT;
static Player TEST_PLAYER;


static void setup(void)
{
    TEST_PLAYER.level = fixture_level();
    TEST_PLAYER.inventory = inventory_new(1);

    TEST_EVENT = (InteractionEvent) {
        .player = &TEST_PLAYER,
        .point = point_new(0, 1),
        .cell = TEST_PLAYER.level->cells[0][1],
        .player_distance = 1
    };
}

static void teardown(void)
{
    cell_free_custom(TEST_EVENT.cell);
    fixture_level_free(TEST_PLAYER.level);
    inventory_free(TEST_PLAYER.inventory);
}

MU_TEST(test_non_light_source)
{
    Item item = fixture_consumable(false);
    inventory_add(TEST_PLAYER.inventory, &item);

    mu_assert_int_eq(EE_CONTINUE, item_light_source_place(&TEST_EVENT));
    mu_assert_int_eq(1, TEST_PLAYER.inventory->items->count);
}

MU_TEST(test_placement)
{
    Style style = 100;
    Item *item = item_allocate();
    *item = (Item) {
        .type = LIGHT_SOURCE,
        .light_source = {.radius = 5, .style = style}
    };
    LightSource *source = &item->light_source;
    Cell ***cells = TEST_PLAYER.level->cells;

    inventory_add(TEST_PLAYER.inventory, item);

    mu_assert(NULL == item->occupied_cell, "");
    mu_assert_int_eq(EE_BREAK, item_light_source_place(&TEST_EVENT));
    mu_assert_int_eq(0, TEST_PLAYER.inventory->items->count);

    mu_assert(NULL != item->occupied_cell, "");
    mu_assert(NULL != source->lighting, "");
    mu_assert(TEST_EVENT.cell == item->occupied_cell, "");

    mu_assert_int_eq(style, cells[2][1]->style);
}

void run_light_source_placement_test(void)
{
    TEST_NAME("Light Source Emplacement");

    MU_SUITE_CONFIGURE(setup, teardown);

    MU_RUN_TEST(test_non_light_source);
    MU_RUN_TEST(test_placement);

    MU_REPORT();
}
