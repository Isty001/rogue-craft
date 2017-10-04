#include "item/crafting/recipe.h"
#include "item/item_registry.h"
#include "../unit_test.h"


MU_TEST(test_load)
{
    item_registry_load();
    receipe_registry_load();

    const List *loaded = receipe_registry_defaults();
    const Recipe *recipe = loaded->head((List *) loaded);

    mu_assert_int_eq(2, recipe->result_count);
    mu_assert_int_eq(2, recipe->ingredient_count);

    Ingredient material = recipe->ingredients[0];
    Ingredient item = recipe->ingredients[1];

    mu_assert_int_eq(INGREDIENT_MATERIAL, material.type);
    mu_assert_int_eq(MATERIAL_WOOD, material.material);
    mu_assert_int_eq(5, material.count);

    mu_assert_int_eq(INGREDIENT_ITEM, item.type);
    assert_string("Torch", item.item_name);
    mu_assert_int_eq(2, item.count);

    receipe_registry_unload();
    item_registry_unload();
}

void run_recipe_loader_test(void)
{
    TEST_NAME("Recipe Loader");

    MU_RUN_TEST(test_load);

    MU_REPORT();
}
