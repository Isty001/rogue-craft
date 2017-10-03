#ifndef ROGUE_CRAFT_RECIPE_H
#define ROGUE_CRAFT_RECIPE_H


#include <stdint.h>
#include <list.h>
#include "item/item.h"


typedef enum {
    INGREDIENT_MATERIAL
} IngredientType;

typedef struct {
    IngredientType type;
    union {
        char *item;
        Material material;
    };
} Ingredient;

typedef struct {
    uint16_t ingredient_count;
    Ingredient *Ingredients;
    const ItemPrototype *result;
} Recipe;


void receipe_registry_load(void);

const List * receipe_registry_defaults(void);

void receipe_registry_unload(void);


#endif
