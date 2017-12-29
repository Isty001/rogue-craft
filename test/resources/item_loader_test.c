#include <wchar.h>
#include <item/item_registry.h>
#include "../unit_test.h"
#include "item/item.h"
#include "../../config/config.h"


static void assert_tool(const ItemPrototype *prototype)
{
    const Item *item = &prototype->item;
    const Tool *tool = &item->tool;

    assert_string("Pickaxe", prototype->name);
    mu_assert(prototype->name == item->name, "");

    assert_range(range_new(1, 100), prototype->value_range);
    mu_assert_int_eq(ITEM_TOOL, prototype->item.type);
    mu_assert_int_eq(COLOR_PAIR(COLOR_GRAY_F), item->style);
    mu_assert_int_eq('p', item->chr);

    mu_assert_int_eq(1, tool->range);
    mu_assert_double_eq(1.5, tool->multipliers.defaults.solid);
    mu_assert_double_eq(1.0, tool->multipliers.defaults.creature);

    mu_assert_double_eq(1.4, tool->multipliers.materials[MATERIAL_STONE]);
    mu_assert_double_eq(1.1, tool->multipliers.materials[MATERIAL_DIRT]);
}

static void assert_consumable(const ItemPrototype *prototype)
{
    const Item *item = &prototype->item;
    const Consumable *consumable = &item->consumable;

    assert_string("Potion", prototype->name);
    mu_assert(prototype->name == item->name, "");

    assert_range(range_new(1, 30), prototype->value_range);
    mu_assert_int_eq(ITEM_CONSUMABLE, item->type);
    mu_assert_int_eq(COLOR_PAIR(COLOR_RED_F) | A_BOLD | A_UNDERLINE, item->style);

    assert_wchar("ð", item->chr, 2);

    mu_assert(consumable->permanent, "should be permanent");
    mu_assert_int_eq(STATE_STAMINA, consumable->state_type);
}

static void assert_light_source(const ItemPrototype *prototype)
{
    const Item *item = &prototype->item;
    const LightSource *light_source = &prototype->item.light_source;

    assert_string("Torch", prototype->name);
    mu_assert(prototype->name == item->name, "");

    mu_assert_int_eq(ITEM_LIGHT_SOURCE, item->type);
    assert_range(range_new(200, 300), prototype->value_range);

    mu_assert(NULL == light_source->lighting, "Actual Lighting shouldn't be initialized yet");
    mu_assert_int_eq(5, light_source->radius);
    mu_assert_int_eq(COLOR_PAIR(COLOR_YELLOW_B), light_source->style);
    mu_assert(true == light_source->portable, "");
}

static void assert_loaded_items(void)
{
    assert_consumable(item_registry_get("Potion"));
    assert_tool(item_registry_get("Pickaxe"));
    assert_light_source(item_registry_get("Torch"));
}

MU_TEST(test_load)
{
    mu_assert(!cache_exists(RESOURCE_ITEMS), "Items cache should not exist");

    item_registry_load();
    mu_assert(cache_exists(RESOURCE_ITEMS), "Items cache should exist");

    assert_loaded_items();

    item_registry_unload();
}

/**
 * After test_load
 */
MU_TEST(test_cache)
{
    char *from_1 = DIR_FIXTURE"/resources/items/items.json";
    char *to_1 = DIR_FIXTURE"/resources/tmp/items.json";
    char *from_2 = DIR_FIXTURE"/resources/items/items_2.json";
    char *to_2 = DIR_FIXTURE"/tmp/items_2.json";

    rename(from_1, to_1);
    rename(from_2, to_2);

    mu_assert(cache_exists(RESOURCE_ITEMS), "Items cache should exist");
    item_registry_load();

    assert_loaded_items();

    item_registry_unload();

    rename(to_1, from_1);
    rename(to_2, from_2);
}

void run_item_loader_test(void)
{
    TEST_NAME("Item loader");

    MU_RUN_TEST(test_load);
    MU_RUN_TEST(test_cache);

    MU_REPORT();
}
