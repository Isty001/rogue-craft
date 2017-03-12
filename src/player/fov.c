#include "player.h"
#include "../../config/config.h"


void player_update_sight(Player *player)
{
    sight_update(player->sight, player->level, player->position.current, PLAYER_DEFAULT_EYESIGHT);
}

bool player_can_see(Player *player, Point point)
{
    Point player_pos = player->position.current;
    bool in_distance = PLAYER_DEFAULT_EYESIGHT >= point_distance(point, player_pos);

    if (!in_distance) return false;

    return sight_has(player->sight, point);
}
