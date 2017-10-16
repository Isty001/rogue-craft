#ifndef ROGUE_CRAFT_MENU_H
#define ROGUE_CRAFT_MENU_H


#include <menu.h>
#include <stdint.h>


typedef struct {
    MENU *base;
    WINDOW *window;
    uint16_t count;
    ITEM **items;
} Menu;


Menu *menu_new(WINDOW *window, uint16_t count, ...);

Menu *menu_new_empty(WINDOW *window, uint16_t count);

void menu_set_item(Menu *menu, uint16_t offset, char *name, void *data);

void menu_setup(Menu *menu);

void menu_process_defaults(Menu *menu, int input);

void menu_set_item_data(Menu *menu, ...);

void menu_free(Menu *menu);


#endif
