#include "util/event.h"
#include "item/item.h"
#include "inventory/inventory.h"


Listener LISTENERS[EVENT_TYPE_NUM][EVENT_LISTENER_MAX] = {
    [EVENT_CLICK] = {
        (Listener) level_interact
    },
    [EVENT_INTERACTION] = {
        (Listener) player_hit,
        (Listener) item_pickup,
        (Listener) item_light_source_place
    },
    [EVENT_INPUT] = {
        (Listener) player_move,
        (Listener) mouse_handler,
        (Listener) inventory_player_use_selected,
        (Listener) inventory_player_shortcut_select,
        (Listener) inventory_player_display,
        (Listener) ncurses_resize
    },
    [EVENT_PANEL_INPUT] = {
        (Listener) inventory_navigate,
        (Listener) inventory_player_set_shortcut
    },
    [EVENT_PANEL_CLOSE] = {
        (Listener) inventory_close
    }
};
