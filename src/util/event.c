#include "config.h"


void event_dispatch(Event event, void *data)
{
    int i = 0;
    Listener listener;

    while ((listener = LISTENERS[event][i++])) {
        if (ES_BREAK == listener(data)) {
            break;
        }
    }
}
