#include "player.h"
#include "item.h"
#include "inventory.h"


typedef struct {
    double damage;
    uint16_t allowed_range;
} Hit;


static double tool_damage_multiplier(Tool *tool, Cell *cell)
{
    double *materials = tool->multipliers.materials;

    if (materials[cell->material]) {
        return materials[cell->material];
    }
    DefaultDamage defaults = tool->multipliers.defaults;

    return SOLID == cell->type ? defaults.solid : defaults.creature;
}

static double calculate_damage(Player *player, Tool *tool, Cell *target)
{
    Attribute *state = player->attributes.state;

    double positive = state[STAMINA].current * 1.2;
    double negative = state[HUNGER].current + state[THIRST].current;

    double damage = (positive - (negative / 2)) / 20;

    if (tool) {
        damage *= tool_damage_multiplier(tool, target);
    }

    return damage;
}

static Hit calculate_hit(Player *player, Item *selected_item, Cell *target)
{
    uint16_t range = 1;
    Tool *tool = NULL;

    if (selected_item && TOOL == selected_item->type) {
        tool = &selected_item->tool;
        range = tool->range;
    }

    return (Hit) {
        .damage = calculate_damage(player, tool, target),
        .allowed_range = range
    };
}

static PlayerError apply_hit(Hit hit, Player *player, Cell *target, Point point)
{
    if (target->in_registry) {
        target = level_replace_cell_with_new(player->level, point);
    }

    target->state -= hit.damage;
    player->attributes.modifiers.dealt_damage += hit.damage;

    if (target->state <= 0) {
        level_set_hollow(player->level, point);
    }
    ncurses_event("Dealt %d damage", (int) round(hit.damage));

    return PE_DEALT_DAMAGE;
}

PlayerError player_hit(Player *player, Point at)
{
    Cell *target = player->level->cells[at.y][at.x];

    if (!cell_damageable(target)) {
        return PE_INVALID_CELL;
    }

    Inventory *inventory = player->inventory;
    Item *selected_item = inventory->items[inventory->selected];
    PlayerError err;
    lock(&player->attributes.mutex);

    Hit hit = calculate_hit(player, selected_item, target);

    if (hit.allowed_range < point_distance(player->position.current, at)) {
        err = PE_OUT_OF_RANGE;
    } else {
        err = apply_hit(hit, player, target, at);
    }
    unlock(&player->attributes.mutex);

    return err;
}
