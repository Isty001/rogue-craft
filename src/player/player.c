#include <memory.h>
#include "player.h"
#include "../../config/config.h"
#include "inventory.h"


#define CHAR_BAR '|'


#define attribute(player, attr_name, def_curr, def_limit, type, disp_name, attr_color) \
    player->attr.attr_name.limit = def_limit;                       \
    player->attr.attr_name.current = def_curr;                      \
    strcpy(player->attr.attr_name.name, disp_name);                 \
    player->attr.attr_name.color = attr_color;                      \
    player->attr.type_map[type] = &player->attr.attr_name;          \


static void find_starting_point(Player *player)
{
    Level *level = player->level;
    Cell *cell;
    Point point;

    do {
        point = level_rand_point(level);

        cell = level->cells[point.y][point.x];
    } while (HOLLOW != cell->type);

    player->cell.previous = cell;
    player->position.previous = point;
    player->position.current = point;
}

static void add_attributes(Player *player)
{
    attribute(player, hp, PLAYER_DEFAULT_HP, PLAYER_DEFAULT_HP, HEALTH, "Health", COLOR_HEALTH);
    attribute(player, stamina, PLAYER_DEFAULT_STAMINA, PLAYER_DEFAULT_STAMINA, STAMINA, "Stamina", COLOR_STAMINA);
    attribute(player, hunger, PLAYER_DEFAULT_HUNGER, PLAYER_LIMIT_HUNGER, HUNGER, "Hunger", COLOR_FOOD);
    attribute(player, thirst, PLAYER_DEFAULT_THIRST, PLAYER_LIMIT_THIRST, THIRST, "Thirst", COLOR_WATER);
}

Player *player_new(Level *level, Camera *camera)
{
    Player *player = alloc(sizeof(Player));
    player->level = level;
    player->cell.prototype.type = PLAYER;
    player->eyesight = PLAYER_DEFAULT_EYESIGHT;
    player->camera = camera;

    add_attributes(player);

    player->cell.prototype.style = COLOR_PAIR(COLOR_PAIR_RED_F);
    player->cell.prototype.chr = PLAYER_CHAR;

    find_starting_point(player);

    player->inventory = inventory_new(PLAYER_DEFAULT_INVENTORY_SIZE);

    return player;
}

static void display_attribute_bar(int width, Attribute *attr, WINDOW *win)
{
    double bar_width = width * ((double) attr->current / attr->limit);

    styled(win, COLOR_PAIR(attr->color),
       for (int i = 0; i < bar_width; i++) {
           waddch(win, CHAR_BAR);
       }
    );
}

void player_attributes_display(Player *player)
{
    Attribute *attr;
    Attribute **attributes = player->attr.type_map;
    WINDOW *win = WINDOW_PLAYER_ATTRIBUTES;
    int width = getmaxx(win) - 2 * PADDING;
    int line = 0;

    for (int i = 0; i < PLAYER_ATTR_NUM; i++) {
        attr = attributes[i];

        styled(win, COLOR_PAIR(attr->color),
               mvwprintw(win, ++line, PADDING, attr->name);
        );
        wprintw(win, ": %d/%d", attr->current, attr->limit);
        wmove(win, ++line, PADDING);
        display_attribute_bar(width, attr, win);
        line++;
    }
    refresh_boxed(win);
}

void player_free(Player *player)
{
    inventory_free(player->inventory);
    free(player);
}
