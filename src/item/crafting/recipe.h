#ifndef ROGUE_CRAFT_RECIPE_H
#define ROGUE_CRAFT_RECIPE_H


#include <stdint.h>
#include <list.h>
#include "item/item.h"


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
    uint16_t ingredient_count;
    Ingredient *ingredients;
    uint16_t result_count;
    const ItemPrototype *result;
} Recipe;


void receipe_registry_load(void);

const List *receipe_registry_defaults(void);

const Recipe *receipe_registry_random(void);

void receipe_registry_unload(void);


#endif
