#include <wchar.h>
#include <tinydir.h>
#include <list.h>
#include "item.h"
#include "memory/memory.h"
#include "util/json.h"
#include "config.h"
#include "item_registry.h"


static Probability ITEM_CONSUMABLE_PROBABILITY;
static Probability ITEM_TOOL_PROBABILITY;
static Probability ITEM_LIGHT_SOURCE_PROBABILITY;

const Probability ITEM_TYPE_PROBABILITY = {
    .count = 3,
    .sum = 70,
    .items = {
        {.chance = 40, .value = &ITEM_CONSUMABLE_PROBABILITY},
        {.chance = 20, .value = &ITEM_LIGHT_SOURCE_PROBABILITY},
        {.chance = 10, .value = &ITEM_TOOL_PROBABILITY},
    }
};

static List *ALL_ITEMS;


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

    consumable->state_type = (StateType) constant_prefixed("STATE", json_get_string(json, "attribute"));
    consumable->permanent = (bool) json_get_bool(json, "permanent");
}

static void build_tool_material_multipliers(Tool *tool, JSON_Object *json)
{
    size_t count = json_object_get_count(json);
    Material material;
    double multiplier;

    for (size_t i = 0; i < count; i++) {
        material = (Material) constant_prefixed("MATERIAL", json_object_get_name(json, i));
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
        add_item(json, ITEM_CONSUMABLE, prototype, &ITEM_CONSUMABLE_PROBABILITY);
    } else if (json_has_object(json, "tool")) {
        build_tool(prototype, json_get_object(json, "tool"));
        add_item(json, ITEM_TOOL, prototype, &ITEM_TOOL_PROBABILITY);
    } else if (json_has_object(json, "lightSource")) {
        build_light_source(prototype, json_get_object(json, "lightSource"));
        add_item(json, ITEM_LIGHT_SOURCE, prototype, &ITEM_LIGHT_SOURCE_PROBABILITY);
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
    memcpy(prototype->name, name, min(strlen(name) + 1, ITEM_NAME_MAX));

    build_value_range(&prototype->value_range, json_get_array(json, "valueRange"));
    build_type_specific(prototype, json);

    ALL_ITEMS->append(ALL_ITEMS, prototype);
}

void item_registry_load(void)
{
    ALL_ITEMS = list_new();

    if (CE_LOADED != item_cache_load()) {
        json_parse_in_dir(env_json_resource_path(RESOURCE_ITEMS), create_prototype_from);
        item_cache_save();
    }
}

static Probability *probability_for(ItemType type)
{
    switch (type) {
        case ITEM_TOOL:
            return &ITEM_TOOL_PROBABILITY;
        case ITEM_CONSUMABLE:
            return &ITEM_CONSUMABLE_PROBABILITY;
        case ITEM_LIGHT_SOURCE:
            return &ITEM_LIGHT_SOURCE_PROBABILITY;
        default:
            fatal("No Probability found for ItemType [%d]", type)
    }
}

void item_registry_add(const ItemPrototype *prototype, uint16_t chance)
{
    Probability *probability = probability_for(prototype->item.type);
    ALL_ITEMS->append(ALL_ITEMS, (void *) prototype);

    probability_add(probability, chance, prototype);
}

const ItemPrototype *item_registry_get(const char *name)
{
    ItemPrototype *found = ALL_ITEMS->find(ALL_ITEMS, (Predicate) function(bool, (ItemPrototype * item) {
        return 0 == strcmp(name, item->name);
    }));

    if (!found) {
        fatal("No such item %s", name);
    }

    return found;
}

const ItemPrototype *item_registry_random(void)
{
    Probability *type = probability_pick(&ITEM_TYPE_PROBABILITY);

    return probability_pick(type);
}

void item_registry_unload(void)
{
    ALL_ITEMS->free(ALL_ITEMS);

    probability_clean(&ITEM_CONSUMABLE_PROBABILITY, mem_dealloc);
    probability_clean(&ITEM_TOOL_PROBABILITY, mem_dealloc);
    probability_clean(&ITEM_LIGHT_SOURCE_PROBABILITY, mem_dealloc);
}
