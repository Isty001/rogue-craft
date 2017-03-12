#ifndef EVENT_EVENT_H
#define EVENT_EVENT_H


#include <stdint.h>
#include <stdlib.h>
#include "level/level.h"


#define EVENT_LISTENER_MAX 20


typedef enum {
    EVENT_INTERACTION
} Event;

typedef struct {
    Player *player;
    Cell *cell;
    Point point;
} InteractionEvent;

typedef enum {
    EE_OK,
    EE_CONTINUE,
    EE_BREAK
} EventError;

typedef EventError (*EventHandler)(void *data);

typedef struct {
    Event listening;
    EventHandler handle;
} Listener;


void event_register_listener(Listener *listener);

/**
 * @param listeners Array of Listeners, where the last, terminating items's
 * .handle member must be NULL
 */
void event_register_listeners(Listener *listeners);

void event_dispatch(Event event, void *data);


#endif
