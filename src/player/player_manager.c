#include "player.h"


static void calculate_starting_point(Player *player)
{
    Level *level = player->level;
    Cell *cell;
    Point p;

    do {
        p = point_new(rand_in_range(level->bounds.y), rand_in_range(level->bounds.x));

        cell = level->cells[p.y][p.x];
    } while (HOLLOW != cell->type);

    player->cell.occupied = cell;
    player->position.previous = p;
    player->position.current = p;
}

Player *player_new(Level *level)
{
    Player *player = alloc(sizeof(Player));
    player->level = level;
    player->cell.prototype.type = PLAYER;
    player->eyesight = PLAYER_DEFAULT_EYESIGHT;

    init_pair(99, COLOR_RED, -1);
    player->cell.prototype.color = 99;


    player->cell.prototype.chr = PLAYER_CHAR;

    calculate_starting_point(player);

    return player;
}

void player_free(Player *player)
{
    free(player);
}
