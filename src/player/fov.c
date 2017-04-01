#include "player.h"
#include "../../config/config.h"


void player_sight_update(Player *player)
{
    sight_update(player->sight, player->level, player->position.current, PLAYER_DEFAULT_EYESIGHT);
}

bool player_can_see(Player *player, Point point)
{
    bool in_player_sight = sight_has(player->sight, point);
    List *lightings = player->level->lightings;

    if (in_player_sight) return true;

    return lightings->exists(lightings, (Predicate )function(bool, (Lighting *lighting) {
        return lighting_has(lighting, point);
    }));
}
