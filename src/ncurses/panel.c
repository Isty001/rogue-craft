#include <list.h>
#include "ncurses.h"


static PANEL *TOP = NULL;


PANEL *panel_push_new(WINDOW *win)
{
    PANEL *panel = new_panel(win);
    set_panel_userptr(panel, TOP);
    TOP = panel;

    return panel;
}

bool panel_is_open(void)
{
    return NULL != TOP;
}

void panel_hide(void)
{
    hide_panel(TOP);
}

void panel_show(void)
{
    show_panel(TOP);
    update_panels();
    doupdate();
}

void panel_close_top(void)
{
    PANEL *prev = (void *) panel_userptr(TOP);
    delwin(panel_window(TOP));
    del_panel(TOP);

    TOP = prev;
}
