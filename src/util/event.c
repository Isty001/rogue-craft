#include "ui/panel.h"
#include "inventory/inventory_player.h"
#include "inventory/inventory.h"


#define MAX_EVENT_LISTENER_PER_EVENT 20
#define MAX_INPUT_LISTENER 20
#define MAX_SUPPORTED_TYPE 10


typedef EventStatus (*Listener)(void *data);

typedef struct {
    Listener executor;
    int inputs[MAX_SUPPORTED_TYPE];
} InputListener;

typedef struct {
    InputListener base;
    PanelType types[MAX_SUPPORTED_TYPE];
} PanelInputListener;


static Listener LISTENERS[EVENT_TYPE_NUM][MAX_EVENT_LISTENER_PER_EVENT] = {
    [EVENT_CLICK] = {
        (Listener) level_interact
    },
    [EVENT_LEVEL_INTERACTION] = {
        (Listener) player_hit,
        (Listener) item_pickup,
        (Listener) item_light_source_place
    },
    [EVENT_PANEL_CLOSE] = {
        (Listener) inventory_close
    }
};

static InputListener INPUT_LISTENERS[MAX_INPUT_LISTENER] = {
    {
        .executor = (Listener) player_move,
        .inputs = {KEY_NORTH, KEY_EAST, KEY_SOUTH, KEY_WEST}
    },
    {
        .executor = (Listener) mouse_handler,
        .inputs = {KEY_MOUSE}
    },
    {
        .executor = (Listener) inventory_player_use_selected,
        .inputs = {KEY_USE_SELECTED}
    },
    {
        .executor = (Listener) inventory_player_shortcut_select,
        .inputs = {KEY_INVENTORY_SHORTCUTS}
    },
    {
        .executor = (Listener) inventory_player_display,
        .inputs = {KEY_PLAYER_INVENTORY}
    },
    {
        .executor = (Listener) ncurses_resize,
        .inputs = {KEY_RESIZE}
    }
};

static PanelInputListener PANEL_INPUT_LISTENERS[MAX_INPUT_LISTENER] = {
    {
        .base = {
            .executor = (Listener) inventory_navigate,
            .inputs = {KEY_NORTH, KEY_EAST, KEY_SOUTH, KEY_WEST, KEY_USE_SELECTED}
        },
        .types = {PANEL_INVENTORY}
    },
    {
        .base = {
            .executor = (Listener) inventory_player_set_shortcut,
            .inputs = {KEY_INVENTORY_SHORTCUTS}
        },
        .types = {PANEL_INVENTORY}
    }
};


void event_dispatch(Event event, void *data)
{
    int i = 0;
    Listener listener;

    while ((listener = LISTENERS[event][i++])) {
        listener(data);
    }
}

static inline bool supports_input(InputListener *listener, int input)
{
    repeat(MAX_SUPPORTED_TYPE,
           if (input == listener->inputs[i]) {
               return true;
           }
    )

    return false;
}

void event_dispatch_input(int input, Player *player)
{
    InputEvent event = {
        .input = input,
        .player = player
    };
    InputListener listener;

    repeat(MAX_INPUT_LISTENER, listener = INPUT_LISTENERS[i];
        if (supports_input(&listener, input)) {
            listener.executor(&event);
        }
    )
}

static inline bool supports_panel(PanelType required, PanelType *supported)
{
    repeat(MAX_SUPPORTED_TYPE,
           if (required == supported[i]) {
               return true;
           }
    )

    return false;
}

void event_dispatch_panel_input(int input, Player *player, const PanelInfo *info)
{
    PanelInputEvent event = {
        .input = input,
        .player = player,
        .info = info
    };
    PanelInputListener listener;
    PanelType required_type = event.info->type;

    repeat(MAX_INPUT_LISTENER, listener = PANEL_INPUT_LISTENERS[i];
        if (supports_panel(required_type, listener.types), supports_input(&listener.base, input)) {
            listener.base.executor(&event);
        }
    )
}
