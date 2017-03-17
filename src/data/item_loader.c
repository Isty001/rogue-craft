#include <wchar.h>
#include <tinydir.h>
#include <list.h>
#include "../player/item.h"
#include "../json.h"
#include "../../config/config.h"


Probability ITEM_CONSUMABLE_PROBABILITY;
Probability ITEM_TOOL_PROBABILITY;


static void add_item(JSON_Object *json, ItemType type, ItemPrototype *prototype, Probability *map)
{
    uint16_t probability = (uint16_t) json_get_number(json, "probability");

    if (0 >= probability) {
        fatal("Item probability must be greater than 0");
    }

    probability_add(map,  probability, prototype);
    prototype->item.type = type;
}

static void build_consumable(ItemPrototype *prototype, JSON_Object *json)
{
    Consumable *consumable = &prototype->item.consumable;

    consumable->attribute = (AttributeType) constant(json_get_string(json, "attribute"));
    consumable->permanent = (bool) json_get_bool(json, "permanent");
}

static void build_tool_material_multipliers(Tool *tool, JSON_Object *json)
{
    size_t count = json_object_get_count(json);
    Material material;
    double multiplier;

    for (size_t i = 0; i < count; i++) {
        material = (Material) constant(json_object_get_name(json, i));
        multiplier = json_value_get_number(json_object_get_value_at(json, i));
        tool->multipliers.materials[material] = multiplier;
    }
}

static void build_tool(ItemPrototype *prototype, JSON_Object *json)
{
    Tool *tool = &prototype->item.tool;
    JSON_Object *multipliers = json_get_object(json, "multipliers");

    tool->range = (uint16_t) json_get_number(json, "range");
    tool->multipliers.defaults = (DefaultDamage) {
        .solid = json_dot_get_number(multipliers, "defaults.solid"),
        .creature = json_dot_get_number(multipliers, "defaults.creature")
    };

    if (json_has_object(multipliers, "materials")) {
        build_tool_material_multipliers(tool, json_get_object(multipliers, "materials"));
    }
}

static void build_type_specific(ItemPrototype *prototype, JSON_Object *json)
{
    if (json_has_object(json, "consumable")) {
        build_consumable(prototype, json_get_object(json, "consumable"));
        add_item(json, CONSUMABLE, prototype, &ITEM_CONSUMABLE_PROBABILITY);
    } else if (json_has_object(json, "tool")) {
        build_tool(prototype, json_get_object(json, "tool"));
        add_item(json, TOOL, prototype, &ITEM_TOOL_PROBABILITY);
    } else {
        fatal("Unable to determine the Item's type");
    }
}

static void build_value_range(Range *range, JSON_Array *value)
{
    size_t count = json_array_get_count(value);

    if (2 != count) {
        fatal("Invalid Range. Format: [from, to]");
    }

    *range = range_new(
        json_array_get_number(value, 0),
        json_array_get_number(value, 1)
    );
}

static void create_prototype_from(JSON_Object *json)
{
    ItemPrototype *prototype = callocate(1, sizeof(ItemPrototype));
    Item *item = &prototype->item;
    char *name = json_get_string(json, "name");

    item->style = json_get_style(json);

    json_get_wchar(&item->chr, json, "char");
    memcpy(item->name, name, strlen(name) + 1);

    build_value_range(&prototype->value_range, json_get_array(json, "valueRange"));
    build_type_specific(prototype, json);
}

void item_load(void)
{
    if (CE_LOADED != item_cache_load()) {
        json_parse_in_dir(DIR_CONFIG_ITEMS, create_prototype_from);
        item_cache_save();
    }
}

void item_unload(void)
{
    probability_clean(&ITEM_CONSUMABLE_PROBABILITY, release);
    probability_clean(&ITEM_TOOL_PROBABILITY, release);
}
