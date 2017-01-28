#include "ncurses.h"
#include "../player/inventory.h"


#define CLICKABLE_NUM 2


typedef struct {
    WINDOW **window;
    void (*interact)(Player *, Click *);
} Clickable;

Clickable CLICKABLE_WINDOWS[CLICKABLE_NUM] = {
    {.window = &WINDOW_MAIN, .interact = level_interact},
    {.window = &WINDOW_INVENTORY, .interact = inventory_interact}
};


void mouse_init(void)
{
    mousemask(ALL_MOUSE_EVENTS, NULL);
}

void mouse_interact(Player *player)
{
    WINDOW *win;
    MEVENT event;
    Clickable clickable;

    repeat(CLICKABLE_NUM,
           getmouse(&event);
           clickable = CLICKABLE_WINDOWS[i];

           if (wmouse_trafo(win = *clickable.window, &event.y, &event.x, false)) {
               Click click;
               click.window = win;
               click.point.y = (uint16_t) event.y;
               click.point.x = (uint16_t) event.x;

               clickable.interact(player, &click);
           }
    )
}
