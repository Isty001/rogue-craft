#include "player.h"
#include "../../data/config.h"


#define attribute(player, name, def_curr, def_max, type)     \
    player->attr.name.limit = def_curr;                      \
    player->attr.name.current = def_max;                     \
    player->attr.type_map[type] = &player->attr.name;        \


static void calculate_starting_point(Player *player)
{
    Level *level = player->level;
    Cell *cell;
    Point point;

    do {
        point = point_new(rand_in_range(level->bounds.y), rand_in_range(level->bounds.x));

        cell = level->cells[point.y][point.x];
    } while (HOLLOW != cell->type);

    player->cell.occupied = cell;
    player->position.previous = point;
    player->position.current = point;
}

static void add_attributes(Player *player)
{
    attribute(player, hp, PLAYER_DEFAULT_HP, PLAYER_DEFAULT_HP, HEALTH);
    attribute(player, stamina, PLAYER_DEFAULT_STAMINA, PLAYER_DEFAULT_STAMINA, STAMINA);
    attribute(player, hunger, PLAYER_DEFAULT_HUNGER, PLAYER_MAX_HUNGER, HUNGER);
    attribute(player, thirst, PLAYER_DEFAULT_THIRST, PLAYER_MAX_THIRST, THIRST);
}

Player *player_new(Level *level)
{
    Player *player = alloc(sizeof(Player));
    player->level = level;
    player->cell.prototype.type = PLAYER;
    player->eyesight = PLAYER_DEFAULT_EYESIGHT;

    add_attributes(player);

    player->cell.prototype.color = COLOR_PAIR_RED_F;
    player->cell.prototype.chr = PLAYER_CHAR;

    calculate_starting_point(player);

    return player;
}

void player_free(Player *player)
{
    free(player);
}
