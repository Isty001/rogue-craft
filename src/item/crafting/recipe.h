#ifndef ROGUE_CRAFT_RECIPE_H
#define ROGUE_CRAFT_RECIPE_H


#include <stdint.h>
#include <list.h>
#include "level/material.h"
#include "util/object.h"


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
    uint16_t result_count;
    const ItemPrototype *result;
    bool is_default;
    uint16_t ingredient_count;
    Ingredient ingredients[];
} Recipe;


#endif
