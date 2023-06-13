#ifndef ROGUE_CRAFT_CRAFTING_H
#define ROGUE_CRAFT_CRAFTING_H


#include "recipe.h"
#include "ui/ncurses.h"


typedef struct Inventory Inventory;


typedef struct {
    Ingredient *ingredient;
    List *items;
} IngredientItems;

typedef struct {
    bool fulfillable;
    IngredientItems *items; //Recipe.ingredient_count
} CraftRequirements;


void craft_panel_display(InputEvent *event);

void craft_panel_navigate(PanelInputEvent *event);

void craft_panel_close(PanelInputEvent *event);

CraftRequirements craft_collect_requiements(const Recipe *recipe, const Inventory *inventory);


#endif
