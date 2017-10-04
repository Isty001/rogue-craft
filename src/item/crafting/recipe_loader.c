#include "item/item_registry.h"
#include "memory/memory.h"
#include "util/json.h"
#include "util/randomization.h"
#include "util/environment.h"
#include "recipe.h"



static List *DEFAULTS;
static Probability ALL;


/*
 * Unnecessary to duplicate the item names in memory & check if the item exists
 */
static const char *refer_actual_item_name(const char *name)
{
    return item_registry_get(name)->name;
}

static void parse_ingredient(JSON_Object *ingredient_json, Ingredient *ingredients, uint16_t curr)
{
    Ingredient *new = &ingredients[curr];
    
    if (json_object_has_value_of_type(ingredient_json, "material", JSONString)) {
        new->type = INGREDIENT_MATERIAL;
        new->material = constant_prefixed("MATERIAL", json_object_get_string(ingredient_json, "material"));
    } else if (json_object_has_value_of_type(ingredient_json, "item", JSONString)) {
        new->type = INGREDIENT_ITEM;
        new->item_name = refer_actual_item_name(json_object_get_string(ingredient_json, "item"));
    } else {
        fatal("An ingredient must have a material or item member");
    }
    new->count = json_get_number(ingredient_json, "count");
}

static void parse_ingredients(JSON_Array *ingredients_json, Recipe *recipe)
{
    recipe->ingredient_count = json_array_get_count(ingredients_json);

    if (0 == recipe->ingredient_count) {
        fatal("A recipe must have at lease one ingredient");
    }
    recipe->ingredients = mem_alloc(recipe->ingredient_count * sizeof(Ingredient));

    repeat(recipe->ingredient_count,
        parse_ingredient(json_array_get_object(ingredients_json, i), recipe->ingredients, i);
    )
}

static void parse(JSON_Object *json)
{
    Recipe *recipe = mem_alloc(sizeof(Recipe));
    recipe->result = item_registry_get(json_get_string(json, "result"));
    recipe->result_count = json_get_number(json, "resultCount");

    parse_ingredients(json_get_array(json, "ingredients"), recipe);

    if (json_get_optional_bool(json, "default")) {
        DEFAULTS->append(DEFAULTS, recipe);
    }
    probability_add(&ALL, json_get_number(json, "probability"), recipe);
}

void receipe_registry_load(void)
{
    DEFAULTS = list_new();

    json_parse_in_dir(env_json_resource_path("recipes"), parse);
}

const List *receipe_registry_defaults(void)
{
    return DEFAULTS;
}

const Recipe *receipe_registry_random(void)
{
    return probability_pick(&ALL);
}

static void recipe_free(Recipe *recipe)
{
    mem_dealloc(recipe->ingredients);
    mem_dealloc(recipe);
}

void receipe_registry_unload(void)
{
    DEFAULTS->free(DEFAULTS);

    probability_clean(&ALL, (Release) recipe_free);
}

