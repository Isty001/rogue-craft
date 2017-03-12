#include "../src/event.h"
#include "../src/player/item.h"


Listener LISTENERS[EVENT_LISTENER_MAX] = {
    {.listening = EVENT_INTERACTION, .handle = (EventHandler) item_pickup},
    {.listening = EVENT_INTERACTION, .handle = (EventHandler) player_hit},
    {.handle = NULL}
};
