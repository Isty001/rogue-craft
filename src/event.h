#ifndef EVENT_EVENT_H
#define EVENT_EVENT_H


#include <stdint.h>
#include <stdlib.h>


#define EVENT_LISTENER_MAX 100


#define event_new(t, d) (Event) {.type = t, .data = d}


typedef enum {
    LEVEL_INTERACT
} EventType;

typedef struct {
    EventType type;
    void *data;
} Event;

typedef enum {
    EE_OK,
    EE_BREAK
} EventError;

typedef EventError (*EventHandler)(Event *data);

typedef struct {
    EventType listening;
    EventHandler handle;
} Listener;



void event_register_listener(Listener *listener);

/**
 * @param listeners Array of Listeners, where the last, terminating items's
 * .handle member must be NULL
 */
void event_register_listeners(Listener *listeners);

void event_dispatch(Event *event);


#endif
