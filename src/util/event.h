#ifndef EVENT_EVENT_H
#define EVENT_EVENT_H


#include <stdint.h>
#include <stdlib.h>


#define EVENT_LISTENER_MAX 20
#define EVENT_TYPE_NUM 5


typedef enum {
    EVENT_INTERACTION,
    EVENT_INPUT,
    EVENT_PANEL_INPUT,
    EVENT_PANEL_CLOSE,
    EVENT_CLICK
} Event;

typedef enum {
    EE_CONTINUE,
    EE_BREAK
} EventError;

typedef EventError (*Listener)(void *data);


void event_dispatch(Event event, void *data);


#endif
