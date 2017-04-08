#include "../src/event.h"
#include "../src/player/item.h"
#include "../src/player/inventory.h"


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
        (Listener) inventory_use_selected,
        (Listener) inventory_shortcut_select,
        (Listener) inventory_display
    }
};
