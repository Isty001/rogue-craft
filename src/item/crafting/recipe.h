#ifndef ROGUE_CRAFT_RECIPE_H
#define ROGUE_CRAFT_RECIPE_H


#include <stdint.h>
#include "level/material.h"


typedef struct ItemPrototype ItemPrototype;


typedef enum {
    INGREDIENT_MATERIAL,
    INGREDIENT_ITEM
} IngredientType;

typedef struct {
    IngredientType type;
    uint16_t count;
    union {
        const char *item_name;
        Material material;
    };
} Ingredient;

typedef struct {
    bool known_by_default;
    const ItemPrototype *result;
    uint16_t ingredient_count;
    Ingredient ingredients[];
} Recipe;


static inline const char *ingredient_to_str(const Ingredient *ingredient)
{
    return INGREDIENT_ITEM == ingredient->type
        ? ingredient->item_name
        : MATERIAL_NAMES[ingredient->material];
}

List *recipe_known_by_default(void);


#endif
