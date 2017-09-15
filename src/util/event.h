#ifndef EVENT_EVENT_H
#define EVENT_EVENT_H


#include <stdint.h>
#include <stdlib.h>
#include "ui/panel.h"


#define EVENT_TYPE_NUM 5


typedef struct Player Player;


typedef enum {
    EVENT_LEVEL_INTERACTION,
    EVENT_PANEL_CLOSE,
    EVENT_CLICK
} Event;

typedef enum {
    ES_BREAK
} EventStatus;


void event_dispatch(Event event, void *data);

void event_dispatch_input(int input, Player *player);

void event_dispatch_panel_input(int input, Player *player, const PanelInfo *info);

#endif
