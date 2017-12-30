#include "item/item_registry.h"
#include "item/item.h"
#include "memory/memory.h"
#include "recipe_registry.h"
#include "util/json.h"
#include "util/randomization.h"
#include "util/environment.h"


static Probability PROBABILITY;


static void parse_ingredient(JSON_Object *ingredient_json, Ingredient *ingredients, uint16_t curr)
{
    Ingredient *new = &ingredients[curr];

    if (json_object_has_value_of_type(ingredient_json, "material", JSONString)) {
        new->type = INGREDIENT_MATERIAL;
        new->material = constant_prefixed("MATERIAL", json_object_get_string(ingredient_json, "material"));
    } else if (json_object_has_value_of_type(ingredient_json, "item", JSONString)) {
        new->type = INGREDIENT_ITEM;
        new->item_name = item_registry_get(json_object_get_string(ingredient_json, "item"))->name;
    } else {
        fatal("An ingredient must have a material or item member");
    }
    new->value = json_get_number(ingredient_json, "value");
}

static void parse_ingredients(JSON_Array *ingredients_json, size_t ingredient_count, Recipe *recipe)
{
    recipe->ingredient_count = ingredient_count;

    if (0 == recipe->ingredient_count) {
        fatal("A recipe must have at lease one ingredient");
    }

    repeat(recipe->ingredient_count,
        parse_ingredient(json_array_get_object(ingredients_json, i), recipe->ingredients, i);
    )
}

static void parse(JSON_Object *json)
{
    JSON_Array *ingredients_json = json_get_array(json, "ingredients");
    size_t ingredient_count = json_array_get_count(ingredients_json);

    Recipe *recipe = mem_alloc(sizeof(Recipe) + (ingredient_count * sizeof(Ingredient)));
    recipe->result = item_registry_get(json_get_string(json, "result"));
    recipe->known_by_default = json_get_optional_bool(json, "known_by_default");

    parse_ingredients(ingredients_json, ingredient_count, recipe);

    probability_add(&PROBABILITY, json_get_number(json, "probability"), recipe);
}

void recipe_registry_load(void)
{
    if (CE_LOADED != recipe_cache_load()) {
        json_parse_in_dir(env_json_resource_path(RESOURCE_RECIPES), parse);
        recipe_cache_save(&PROBABILITY);
    }
}

void recipe_registry_add(const Recipe *recipe, uint16_t chance)
{
    probability_add(&PROBABILITY, chance, recipe);
}

const Probability *recipe_registry_all(void)
{
    return &PROBABILITY;
}

const Recipe *recipe_registry_random(void)
{
    return probability_pick(&PROBABILITY);
}

List *recipe_known_by_default(void)
{
    List *recipes = list_new();
    Recipe *recipe;

    repeat(PROBABILITY.count,
        recipe = (Recipe *) PROBABILITY.items[i].value;

        if (recipe->known_by_default) {
            recipes->append(recipes, recipe);
        }
    )

    return recipes;
}

void recipe_registry_unload(void)
{
    probability_clean(&PROBABILITY, (Release) mem_dealloc);
}

