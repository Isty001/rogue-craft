#ifndef ROGUE_CRAFT_CRAFTING_H
#define ROGUE_CRAFT_CRAFTING_H


#include <list.h>
#include "recipe.h"
#include "ui/ncurses.h"


typedef struct {
    bool successful;
    List *requirements;
} CraftResult;

typedef struct Inventory Inventory;


void craft_panel_display(InputEvent *event);

void craft_panel_navigate(PanelInputEvent *event);

void craft_panel_close(PanelInputEvent *event);

const List *craft_collect_requirements(const Recipe *recipe, const Inventory *inventory);


#endif
