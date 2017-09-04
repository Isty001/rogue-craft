#include <level/level.h>
#include "ncurses.h"


void mouse_init(void)
{
    mousemask(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(100);
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

EventStatus mouse_handler(InputEvent *event)
{
    if (KEY_MOUSE != event->input) {
        return ES_CONTINUE;
    }

    MEVENT mevent;
    getmouse(&mevent);

    int i = 0;
    WINDOW *window;

    while (i < NCURSES_WINDOW_NUM && (window = *WINDOW_LIST[i++])) {
        if (wmouse_trafo(window, &mevent.y, &mevent.x, false)) {
            dispatch_click_event(&mevent, window, event->player);

            return ES_BREAK;
        }
    }

    return ES_CONTINUE;
}
