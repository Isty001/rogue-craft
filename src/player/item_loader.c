#include <wchar.h>
#include <tinydir.h>
#include <list.h>
#include "item.h"
#include "../json.h"
#include "../randomization.h"
#include "../storage/storage.h"


Probability ITEM_CONSUMABLE_PROBABILITY;
Probability ITEM_TOOL_PROBABILITY;


static void add_item(JSON_Object *json, ItemType type, ItemPrototype *prototype, Probability *probability)
{
    int chance = (int) get_number(json, "chance");

    if (0 >= chance) {
        fatal("Item chance must be greater than 0");
    }

    probability->items[probability->count].chance = chance;
    probability->items[probability->count].value = prototype;

    probability->sum += chance;
    prototype->item.type = type;
    probability->count++;
}

static void build_consumable(ItemPrototype *prototype, JSON_Object *json)
{
    Consumable *consumable = &prototype->item.consumable;

    consumable->attribute = (AttributeType) constant(get_string(json, "attribute"));
    consumable->permanent = (bool) get_bool(json, "permanent");
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
    JSON_Object *multipliers = get_object(json, "multipliers");

    tool->range = (uint16_t) get_number(json, "range");
    tool->multipliers.defaults = (DefaultDamage) {
        .solid = dot_get_number(multipliers, "defaults.solid"),
        .creature = dot_get_number(multipliers, "defaults.creature")
    };

    if (has_object(multipliers, "materials")) {
        build_tool_material_multipliers(tool, get_object(multipliers, "materials"));
    }
}

static void build_type_specific(ItemPrototype *prototype, JSON_Object *json)
{
    if (has_object(json, "consumable")) {
        build_consumable(prototype, get_object(json, "consumable"));
        add_item(json, CONSUMABLE, prototype, &ITEM_CONSUMABLE_PROBABILITY);
    } else if (has_object(json, "tool")) {
        build_tool(prototype, get_object(json, "tool"));
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

static Style build_style(JSON_Array *attributes)
{
    Style style = 0;
    size_t count = json_array_get_count(attributes);

    for (size_t i = 0; i < count; ++i) {
        style |= constant((char *) json_array_get_string(attributes, i));
    }

    return style;
}

static void create_prototype_from(JSON_Object *json)
{
    ItemPrototype *prototype = allocate(sizeof(ItemPrototype));
    Item *item = &prototype->item;
    char *name = get_string(json, "name");

    item->style = build_style(get_array(json, "style"));

    memcpy(item->name, name, strlen(name) + 1);
    mbtowc(&item->chr, get_string(json, "char"), 2);
    build_value_range(&prototype->value_range, get_array(json, "valueRange"));
    build_type_specific(prototype, json);
}

static void parse_json(JSON_Array *items)
{
    size_t count = json_array_get_count(items);
    JSON_Object *item;

    for (size_t i = 0; i < count; i++) {
        item = json_array_get_object(items, i);
        create_prototype_from(item);
    }
}

void item_load(char *path)
{
    if (IE_CACHE_LOADED == item_cache_load()) {
        return;
    }

    json_set_allocation_functions((JSON_Malloc_Function) allocate, release);

    JSON_Value *json;
    JSON_Array *array;

    dir_foreach(path, function(void, (tinydir_file *file) {
        json = json_parse_file(file->path);

        if (json && (array = json_value_get_array(json))) {
            parse_json(array);
            json_value_free(json);
        }
    }));

    item_cache_save();
}

static void clean_probability(Probability *probability)
{

    for (int i = 0; i < probability->count; i++) {
        release(probability->items[i].value);
    }
    probability->count = 0;
    probability->sum = 0;
}

void item_unload(void)
{
    clean_probability(&ITEM_CONSUMABLE_PROBABILITY);
    clean_probability(&ITEM_TOOL_PROBABILITY);
}
