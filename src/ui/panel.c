#include <list.h>
#include "memory/memory.h"
#include "ncurses.h"
#include "panel.h"
#include "config.h"


static List *PANELS = NULL;


void panel_init(void)
{
    PANELS = list_new();
}

void panel_cleanup(void)
{
    PANELS->free(PANELS);
}

static PanelInfo *copy_to_heap(PanelInfo info)
{
    PanelInfo *copy = mem_alloc(sizeof(PanelInfo));
    *copy = info;

    return copy;
}

PANEL *panel_push_new(WINDOW *win, PanelInfo info)
{
    PANEL *panel = new_panel(win);

    set_panel_userptr(panel, copy_to_heap(info));
    PANELS->append(PANELS, panel);

    return panel;
}

static void dispatch_panel_event(Event name, int input, Player *player, PANEL *panel)
{
    PanelInputEvent event = {
        .input = input,
        .player = player,
        .info = panel_userptr(panel)
    };

    event_dispatch(name, &event);
}

void panel_dispatch_input_event(int input, Player *player)
{
    PANEL *top = PANELS->last(PANELS);

    dispatch_panel_event(EVENT_PANEL_INPUT, input, player, top);
}

void panel_close_top(int input, Player *player)
{
    PANEL *top = PANELS->pop(PANELS);

    dispatch_panel_event(EVENT_PANEL_CLOSE, input, player, top);
    /** @see grid_free  */
    del_panel(top);
    delwin(panel_window(top));

    mem_dealloc((void *) panel_userptr(top));
}

bool panel_is_open(void)
{
    return 0 != PANELS->count;
}

void panel_hide(void)
{
    PANELS->foreach_r(PANELS, (Foreach) hide_panel);
}

void panel_show(void)
{
    PANELS->foreach_l(PANELS, (Foreach) show_panel);
    update_panels();
    doupdate();
}
