#include "ncurses.h"


void mouse_init(void)
{
    mousemask(ALL_MOUSE_EVENTS, NULL);
    mouseinterval(70);
}

static void dispatch_click_event(MEVENT *mevent, WINDOW *window, Player *player)
{
    ClickEvent event = {
        .point = point_new(mevent->y, mevent->x),
        .window = window,
        .player = player
    };

    event_dispatch(EVENT_CLICK, &event);
}

EventError mouse_handler(InputEvent *event)
{
    MEVENT mevent;
    getmouse(&mevent);

    int i = 0;
    WINDOW *window;

    while ((window = *WINDOW_LIST[i++])) {
        if (wmouse_trafo(window, &mevent.y, &mevent.x, false)) {
            dispatch_click_event(&mevent, window, event->player);

            return EE_BREAK;
        }
    }

    return EE_OK;
}
