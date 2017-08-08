
#include "player.h"
#include "item/item.h"
#include "inventory/inventory.h"
#include "sfx/sfx.h"


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
    State *states = player->state.map;

    double positive = states[STAMINA].current * 1.15;
    double negative = states[HUNGER].current + states[THIRST].current;

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

static void apply_hit(Hit hit, Player *player, Cell *target, Point point)
{
    if (target->in_registry) {
        target = level_replace_cell_with_new(player->level, point);
    }

    target->state -= hit.damage;
    player->state.modifiers.dealt_damage += hit.damage;

    if (target->state <= 0) {
        sfx_play_break(target);
        level_set_hollow(player->level, point);
    }
}

EventError player_hit(InteractionEvent *event)
{
    Cell *target = event->cell;
    Player *player = event->player;
    Point point = event->point;

    if (!cell_is_damageable(target)) {
        return EE_CONTINUE;
    }

    Inventory *inventory = player->inventory;
    Item *selected_item = inventory_selected(inventory);

    Hit hit = calculate_hit(player, selected_item, target);

    if (hit.allowed_range >= point_distance(player->position.current, point)) {
        sfx_play_hit(target);
        apply_hit(hit, player, target, point);
    }

    return EE_CONTINUE;
}
