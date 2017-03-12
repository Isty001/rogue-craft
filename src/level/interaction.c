#include "../ncurses/ncurses.h"
#include "camera.h"


void level_interact(Player *player, Point click)
{
    Point on_level = camera_to_level_point(player->camera, click);
    Cell ***cells = player->level->cells;
    Cell *cell = cells[on_level.y][on_level.x];

    InteractionEvent event = {
        .player = player, .cell = cell, .point = on_level
    };

    event_dispatch(EVENT_INTERACTION, &event);
}
