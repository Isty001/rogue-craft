#include "item/crafting/recipe.h"
#include "item/crafting/recipe_registry.h"
#include "item/item_registry.h"
#include "../unit_test.h"


static void assert_recipes(void)
{
    recipe_registry_load();

    const Probability *loaded = recipe_registry_all();

    mu_assert_int_eq(1, loaded->count);

    const Recipe *recipe = loaded->items[0].value;

    mu_assert_int_eq(2, recipe->result_count);
    mu_assert_int_eq(2, recipe->ingredient_count);

    Ingredient material = recipe->ingredients[0];
    Ingredient item = recipe->ingredients[1];

    mu_assert_int_eq(INGREDIENT_MATERIAL, material.type);
    mu_assert_int_eq(MATERIAL_WOOD, material.material);
    mu_assert_int_eq(5, material.count);

    mu_assert_int_eq(INGREDIENT_ITEM, item.type);
    assert_string("Torch", ((const ItemPrototype *) item.item.obj)->name);
    mu_assert_int_eq(2, item.count);

    recipe_registry_unload();
}

MU_TEST(test_load)
{
    mu_assert(cache_is_empty(RESOURCE_RECIPES), "");

    assert_recipes();

    mu_assert(false == cache_is_empty(RESOURCE_RECIPES), "");
}

MU_TEST(test_cache)
{
    char *original = DIR_FIXTURE"/resources/json/recipes/recipes.json";
    char *tmp = DIR_FIXTURE"/resources/json/tmp/recipes.json";

//    rename(original, tmp);

    assert_recipes();

  //  rename(tmp, original);
}

void run_recipe_loader_test(void)
{
    TEST_NAME("Recipe Loader");

    item_registry_load();

    MU_RUN_TEST(test_load);
    MU_RUN_TEST(test_cache);

    item_registry_unload();

    MU_REPORT();
}
