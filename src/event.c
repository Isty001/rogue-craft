#include "event.h"


static uint16_t COUNT = 0;
static Listener *LISTENERS[EVENT_LISTENER_MAX];


void event_register_listener(Listener *listener)
{
    LISTENERS[COUNT++] = listener;
}

void event_register_listeners(Listener *listeners)
{
    int i = 0;

    while (NULL != listeners[i].handle) {
        event_register_listener(&listeners[i]);
        i++;
    }
}

void event_dispatch(Event event, void *data)
{
    Listener *listener;

    for (uint16_t i = 0; i < COUNT; i++) {
        listener = LISTENERS[i];

        if (event == listener->listening) {
            if (EE_BREAK == listener->handle(data)) {
                break;
            }
        }
    }
}
