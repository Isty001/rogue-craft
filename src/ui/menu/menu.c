#include "ui/ncurses.h"
#include "keymap.h"
#include "menu.h"
#include "memory/memory.h"


static void create_items(va_list choices, uint16_t count, Menu *menu)
{
    char *choice;
    repeat(count,
           choice = va_arg(choices, char *);
           menu->items[i] = new_item(choice, NULL);
    )
    menu->items[count] = NULL;

    va_end(choices);
}

Menu *menu_new(WINDOW *window, uint16_t count, ...)
{
    Menu *menu = menu_new_empty(window, count);
    va_list choices;
    va_start(choices, count);
    create_items(choices, count, menu);

    menu->base = new_menu(menu->items);
    menu_setup(menu);

    return menu;
}

Menu *menu_new_empty(WINDOW *window, uint16_t count)
{
    Menu *menu = mem_alloc(sizeof(Menu));
    menu->items = mem_alloc(count * sizeof(ITEM *) + 1);
    menu->window = window;
    menu->count = count;

    return menu; 
}

void menu_set_item(Menu *menu, uint16_t offset, char *name, void *data)
{
    menu->items[offset] = new_item(name, NULL);
    set_item_userptr(menu->items[offset], data);
}

void menu_setup(Menu *menu)
{
    set_menu_win(menu->base, menu->window);
    set_menu_sub(menu->base, menu->window);
    set_menu_mark(menu->base, "=> ");
}

void menu_set_item_data(Menu *menu, ...)
{
    va_list args;
    va_start(args, menu);

    repeat(menu->count,
           set_item_userptr(menu->items[i], va_arg(args, void *));
    )
    va_end(args);
}

void menu_process_defaults(Menu *menu, int input)
{
    MENU *base = menu->base;

    switch (input) {
        case KEY_NORTH:
            menu_driver(base, REQ_UP_ITEM);
            break;
        case KEY_SOUTH:
            menu_driver(base, REQ_DOWN_ITEM);
            break;
        case KEY_EAST:
            menu_driver(base, REQ_DOWN_ITEM);
            break;
        case KEY_WEST:
            menu_driver(base, REQ_DOWN_ITEM);
            break;
    }
}

void menu_free(Menu *menu)
{
    // Items must be freed after unpost
    unpost_menu(menu->base);
    free_menu(menu->base);

    repeat(menu->count,
           free_item(menu->items[i])
    )
    mem_dealloc(menu->items);
    mem_dealloc(menu);
}
