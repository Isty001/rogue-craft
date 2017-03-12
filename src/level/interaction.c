#include "../ncurses/ncurses.h"
#include "camera.h"


EventError level_interact(ClickEvent *click)
{
    if (WINDOW_MAIN != click->window) {
    }

    Player *player = click->player;
    Point on_level = camera_to_level_point(player->camera, click->point);
    Cell ***cells = player->level->cells;
    Cell *cell = cells[on_level.y][on_level.x];

    InteractionEvent event = {
        .player = player, .cell = cell, .point = on_level
    };

    event_dispatch(EVENT_INTERACTION, &event);

    return EE_BREAK;
}
