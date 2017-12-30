#include "crafting.h"
#include "util/util.h"
#include "inventory/inventory.h"


typedef enum {
    INGREDIENT_FOUND,
    INGREDIENT_NOT_FOUND
} IngredientStatus;


static List *find_materials(const Ingredient ingredient, const Inventory *inventory)
{
    List *materials = list_new();
    const Item *item;
    uint16_t collected = 0;

    repeat(inventory->count,
        item = inventory->items[i];

        if (item && ITEM_MATERIAL == item->type && ingredient.material == item->material) {
            materials->append(materials, (void *)item);
            collected += item->value;
        }
        if (ingredient.value == collected) {
            return materials;
        }
    )
    materials->free(materials);

    return NULL;
}

static const Item *find_single_item(const Ingredient ingredient, const Inventory *inventory)
{
    const Item *item;

    repeat(inventory->count,
        item = inventory->items[i];

        if (item && 0 == strcmp(ingredient.item_name, item->name) && item->value >= ingredient.value) {
            return item;
        };
    );

    return NULL;
}

static IngredientStatus add_required_item(List *requirements, const Ingredient ingredient, const Inventory *inventory)
{
    const Item *item = find_single_item(ingredient, inventory);

    if (item) {
        requirements->append(requirements, (void *)item);
        return INGREDIENT_FOUND;
    }

    return INGREDIENT_NOT_FOUND;
}

static IngredientStatus add_required_material(List *requirements, const Ingredient ingredient, const Inventory *inventory)
{
    List *materials = find_materials(ingredient, inventory);

    if (materials) {
        requirements->merge_f(requirements, materials);

        return INGREDIENT_FOUND;
    }
    return INGREDIENT_NOT_FOUND;
}

static IngredientStatus add_requirements_of(List *requirements, const Ingredient ingredient, const Inventory *inventory)
{
    switch (ingredient.type) {
        case INGREDIENT_ITEM:
            return add_required_item(requirements, ingredient, inventory);
        case INGREDIENT_MATERIAL:
            return add_required_material(requirements, ingredient, inventory);
    }

    return INGREDIENT_NOT_FOUND;
}

const List *craft_collect_requirements(const Recipe *recipe, const Inventory *inventory)
{
    IngredientStatus status;
    List *requirements = list_new();

    repeat(recipe->ingredient_count,
        status = add_requirements_of(requirements, recipe->ingredients[i], inventory);

        if (INGREDIENT_NOT_FOUND == status) {
            requirements->free(requirements);
            return NULL;
        }
    )

    return requirements;
}

