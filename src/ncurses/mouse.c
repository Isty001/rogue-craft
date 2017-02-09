#include "ncurses.h"


#define CLICKABLE_NUM 1


typedef struct {
    WINDOW **window;
    void (*interact)(Player *, Point );
} Clickable;

Clickable CLICKABLE_WINDOWS[CLICKABLE_NUM] = {
    {.window = &WINDOW_MAIN, .interact = level_interact}
};


void mouse_init(void)
{
    mousemask(ALL_MOUSE_EVENTS, NULL);
    mouseinterval(70);
}

void mouse_interact(Player *player)
{
    MEVENT event;
    Clickable clickable;
    getmouse(&event);

    repeat(CLICKABLE_NUM,
       clickable = CLICKABLE_WINDOWS[i];

       if (wmouse_trafo(*clickable.window, &event.y, &event.x, false)) {
           Point click = point_new(
               event.y + 1,
               event.x
           );

           clickable.interact(player, click);
       }
    )
}
