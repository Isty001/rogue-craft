#ifndef ROGUE_CRAFT_MAIN_MENU_H
#define ROGUE_CRAFT_MAIN_MENU_H


#include "gamestate/gamestate.h"


typedef enum {
    MAIN_MENU_RESUME,
    MAIN_MENU_NEW_GAME,
    MAIN_MENU_QUIT,
} MainMenuAction;


MainMenuAction menu_main_display(GameState *state);

void menu_main_cleanup(void);


#endif
