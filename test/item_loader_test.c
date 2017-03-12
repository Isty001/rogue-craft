#include <wchar.h>
#include "unit_test.h"
#include "../src/player/item.h"
#include "../config/config.h"
#include "../src/storage/cache.h"


static void assert_tool(ItemPrototype *prototype)
{
    Item *item = &prototype->item;
    Tool *tool = &item->tool;

    assert_string("Pickaxe", item->name);
    assert_range(range_new(1, 100), prototype->value_range);
    mu_assert_int_eq(TOOL, prototype->item.type);
    mu_assert_int_eq(COLOR_PAIR(COLOR_PAIR_GRAY_F), item->style);
    mu_assert_int_eq('p', item->chr);

    mu_assert_int_eq(1, tool->range);
    mu_assert_double_eq(1.5, tool->multipliers.defaults.solid);
    mu_assert_double_eq(1.0, tool->multipliers.defaults.creature);

    mu_assert_double_eq(1.4, tool->multipliers.materials[STONE]);
    mu_assert_double_eq(1.1, tool->multipliers.materials[DIRT]);
}

static void assert_consumable(ItemPrototype *prototype)
{
    Item *item = &prototype->item;
    Consumable *consumable = &item->consumable;

    assert_string("Potion", item->name);
    assert_range(range_new(1, 30), prototype->value_range);
    mu_assert_int_eq(CONSUMABLE, item->type);
    mu_assert_int_eq(COLOR_PAIR(COLOR_PAIR_RED_F) | A_BOLD | A_UNDERLINE, item->style);

    char actual[2];
    wcstombs(actual, &item->chr, 2);
    actual[2] = '\0';

    assert_string("ð", actual);

    mu_assert(consumable->permanent, "should be permanent");
    mu_assert_int_eq(STAMINA, consumable->attribute);
}

static void assert_loaded_items(void)
{
    mu_assert_int_eq(1, ITEM_CONSUMABLE_PROBABILITY.count);
    mu_assert_int_eq(10, ITEM_CONSUMABLE_PROBABILITY.sum);

    assert_consumable(ITEM_CONSUMABLE_PROBABILITY.items[0].value);

    mu_assert_int_eq(1, ITEM_TOOL_PROBABILITY.count);
    mu_assert_int_eq(15, ITEM_TOOL_PROBABILITY.sum);

    assert_tool(ITEM_TOOL_PROBABILITY.items[0].value);
}

MU_TEST(test_load)
{
    item_load(FIXTURE_DIR"/config/items");

    assert_loaded_items();

    item_unload();
}

/**
 * After test_load
 */
MU_TEST(test_cache)
{
    item_load("Doesn't matter, as we already cached the items in the previous test case");

    mu_assert(cache_exists(ITEM_CACHE), "Items cache should exist");
    assert_loaded_items();

    item_unload();
}

void run_item_loader_test(void)
{
    TEST_NAME("Item loader");

    MU_RUN_TEST(test_load);
    MU_RUN_TEST(test_cache);

    MU_REPORT();
}
