#ifndef ROGUE_CRAFT_INVENTORY_PLAYER_H
#define ROGUE_CRAFT_INVENTORY_PLAYER_H


#include "ui/panel.h"
#include "ui/ncurses.h"
#include "util/event.h"


void inventory_player_shortcut_select(InputEvent *event);

void inventory_player_use_selected(InputEvent *event);

void inventory_player_display(InputEvent *event);

void inventory_player_set_shortcut(PanelInputEvent *event);

void inventory_player_drop_shortcut(InputEvent *event);


#endif
