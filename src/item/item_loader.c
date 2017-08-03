#include <wchar.h>
#include <tinydir.h>
#include <list.h>
#include "item.h"
#include "util/memory.h"
#include "util/json.h"
#include "config.h"


Probability ITEM_CONSUMABLE_PROBABILITY;
Probability ITEM_TOOL_PROBABILITY;
Probability ITEM_LIGHT_SOURCE_PROBABILITY;


static void add_item(JSON_Object *json, ItemType type, ItemPrototype *prototype, Probability *map)
{
    uint16_t probability = (uint16_t) json_get_number(json, "probability");

    if (0 >= probability) {
        fatal("Item probability must be greater than 0");
    }

    probability_add(map, probability, prototype);
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

static void build_light_source(ItemPrototype *prototype, JSON_Object *json)
{
    LightSource *source = &prototype->item.light_source;
    source->radius = (uint16_t) json_get_number(json, "radius");
    source->style = json_get_style(json);
    source->portable = (bool) json_get_bool(json, "portable");
    source->lighting = NULL;
}

static void build_type_specific(ItemPrototype *prototype, JSON_Object *json)
{
    if (json_has_object(json, "consumable")) {
        build_consumable(prototype, json_get_object(json, "consumable"));
        add_item(json, CONSUMABLE, prototype, &ITEM_CONSUMABLE_PROBABILITY);
    } else if (json_has_object(json, "tool")) {
        build_tool(prototype, json_get_object(json, "tool"));
        add_item(json, TOOL, prototype, &ITEM_TOOL_PROBABILITY);
    } else if (json_has_object(json, "lightSource")) {
        build_light_source(prototype, json_get_object(json, "lightSource"));
        add_item(json, LIGHT_SOURCE, prototype, &ITEM_LIGHT_SOURCE_PROBABILITY);
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
    ItemPrototype *prototype = mem_calloc(1, sizeof(ItemPrototype));
    Item *item = &prototype->item;
    char *name = json_get_string(json, "name");

    item->occupied_cell = NULL;
    item->style = json_get_style(json);

    json_get_wchar(&item->chr, json, "char");
    memcpy(item->name, name, min(strlen(name) + 1, ITEM_NAME_MAX));

    build_value_range(&prototype->value_range, json_get_array(json, "valueRange"));
    build_type_specific(prototype, json);
}

void item_load(void)
{
    if (CE_LOADED != item_cache_load()) {
        json_parse_in_dir(env_config_dir(CONFIG_ITEMS), create_prototype_from);
        item_cache_save();
    }
}

void item_unload(void)
{
    probability_clean(&ITEM_CONSUMABLE_PROBABILITY, mem_dealloc);
    probability_clean(&ITEM_TOOL_PROBABILITY, mem_dealloc);
    probability_clean(&ITEM_LIGHT_SOURCE_PROBABILITY, mem_dealloc);
}
