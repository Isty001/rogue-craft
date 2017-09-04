#include <player/player.h>


EventStatus level_interact(ClickEvent *click)
{
    if (WINDOW_MAIN != click->window) {
        return ES_CONTINUE;
    }

    Player *player = click->player;
    Point on_level = camera_to_level_point(&player->camera, click->point);
    Cell *cell = player->level->cells[on_level.y][on_level.x];

    LevelInteractionEvent event = {
        .player = player,
        .cell = cell,
        .point = on_level,
        .player_distance = point_distance(on_level, player->position.current)
    };

    event_dispatch(EVENT_INTERACTION, &event);

    return ES_BREAK;
}
