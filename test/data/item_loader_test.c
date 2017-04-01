#include <wchar.h>
#include "../unit_test.h"
#include "../../src/player/item.h"
#include "../../config/config.h"


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

    assert_wchar("ð", item->chr, 2);

    mu_assert(consumable->permanent, "should be permanent");
    mu_assert_int_eq(STAMINA, consumable->attribute);
}

static void assert_light_source(ItemPrototype *prototype)
{
    Item *item = &prototype->item;
    LightSource *light_source = &prototype->item.light_source;

    mu_assert_int_eq(LIGHT_SOURCE, item->type);
    assert_range(range_new(200, 300), prototype->value_range);

    mu_assert(NULL == light_source->lighting, "Actual Lighting shouldn't be initialized yet");
    mu_assert_int_eq(5, light_source->radius);
    mu_assert_int_eq(COLOR_PAIR(COLOR_PAIR_YELLOW_B), light_source->style);
    mu_assert(true == light_source->portable, "");

    mu_assert(prototype->item.clean == item_light_source_clean, "");
}

static void assert_loaded_items(void)
{
    mu_assert_int_eq(1, ITEM_CONSUMABLE_PROBABILITY.count);
    mu_assert_int_eq(10, ITEM_CONSUMABLE_PROBABILITY.sum);

    assert_consumable(ITEM_CONSUMABLE_PROBABILITY.items[0].value);

    mu_assert_int_eq(1, ITEM_TOOL_PROBABILITY.count);
    mu_assert_int_eq(15, ITEM_TOOL_PROBABILITY.sum);

    assert_tool(ITEM_TOOL_PROBABILITY.items[0].value);

    mu_assert_int_eq(1, ITEM_LIGHT_SOURCE_PROBABILITY.count);
    mu_assert_int_eq(5, ITEM_LIGHT_SOURCE_PROBABILITY.sum);

    assert_light_source(ITEM_LIGHT_SOURCE_PROBABILITY.items[0].value);
}

MU_TEST(test_load)
{
    mu_assert(!cache_exists(CACHE_CONFIG_ITEM), "Items cache should not exist");

    item_load();
    mu_assert(cache_exists(CACHE_CONFIG_ITEM), "Items cache should exist");

    assert_loaded_items();

    item_unload();
}

/**
 * After test_load
 */
MU_TEST(test_cache)
{
    rename(DIR_CONFIG_ITEMS"/items.json", DIR_CONFIG"/tmp/items.json");
    rename(DIR_CONFIG_ITEMS"/items_2.json", DIR_CONFIG"/tmp/items_2.json");

    mu_assert(cache_exists(CACHE_CONFIG_ITEM), "Items cache should exist");
    item_load();

    assert_loaded_items();

    item_unload();

    rename(DIR_CONFIG"/tmp/items.json", DIR_CONFIG_ITEMS"/items.json");
    rename(DIR_CONFIG"/tmp/items_2.json", DIR_CONFIG_ITEMS"/items_2.json");
}

void run_item_loader_test(void)
{
    TEST_NAME("Item loader");

    MU_RUN_TEST(test_load);
    MU_RUN_TEST(test_cache);

    MU_REPORT();
}
