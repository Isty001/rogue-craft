#include "../worker.h"


#define add_fatigue_damage(changes, damage)             \
    changes[HEALTH] -= rand_in_range(damage->health);   \
    changes[STAMINA] -= rand_in_range(damage->stamina);


#define calculate_fatigue_damage(changes, limit, type, damage)           \
    if (damage->limit <= changes[type] + attributes[type].current) {     \
        add_fatigue_damage(changes, damage);                             \
    }


static void assure_unsigned(int16_t *change, Attribute *attribute)
{
    int16_t new = *change + attribute->current;

    if (new < 0) {
        *change += abs(new);
    }
}

static void calculate_fatigue_damages(FatigueDamage *damage, int16_t *changes, Attribute *attributes, Modifiers *modifiers)
{
    calculate_fatigue_damage(changes, hunger, HUNGER, damage);
    calculate_fatigue_damage(changes, thirst, THIRST, damage);

    if (damage->time < difftime(time(NULL), modifiers->updated)) {
        add_fatigue_damage(changes, damage);
    }

    assure_unsigned(&changes[HEALTH], &attributes[HEALTH]);
    assure_unsigned(&changes[STAMINA], &attributes[STAMINA]);
}

static void calculate_fatigue_change(int16_t *changes, AttributeType type, Range value, Attribute *attribute)
{
    int16_t change = changes[type] + rand_in_range(value);
    int16_t new = attribute->current + change;

    if (new <= attribute->max) {
        changes[type] = change;
    }
}

static void calculate_fatigue_changes(Fatigue *fatigue, int16_t *changes, Attribute *attributes)
{
    calculate_fatigue_change(changes, THIRST, fatigue->thirst, &attributes[THIRST]);
    calculate_fatigue_change(changes, HUNGER, fatigue->hunger, &attributes[HUNGER]);
    calculate_fatigue_change(changes, STAMINA, fatigue->stamina, &attributes[STAMINA]);
}

static void calculate_changes(AttributeConfig *cfg, Attribute *attributes, Modifiers *modifiers, int16_t *changes)
{
    if (cfg->fatigue.dealt_damage < modifiers->dealt_damage) {
        calculate_fatigue_changes(&cfg->fatigue, changes, attributes);
    }

    if (cfg->fatigue.traveled < modifiers->traveled) {
        calculate_fatigue_changes(&cfg->fatigue, changes, attributes);
    }

    if (cfg->fatigue.time < difftime(time(NULL), modifiers->updated)) {
        calculate_fatigue_changes(&cfg->fatigue, changes, attributes);
    }

    calculate_fatigue_damages(&cfg->damage, changes, attributes, modifiers);
}

static bool has_any_changes(int16_t *changes)
{
    return changes[HUNGER] || changes[THIRST] || changes[STAMINA] || changes[HEALTH];
}

static void apply_changes(Player *player, Modifiers *modifiers, int16_t *changes)
{
    if (has_any_changes(changes)) {
        lock(&player->attributes.mutex);
        player->attributes.modifiers.dealt_damage -= modifiers->dealt_damage;
        player->attributes.modifiers.traveled -= modifiers->traveled;
        player->attributes.modifiers.updated = time(NULL);

        repeat(PLAYER_ATTR_NUM,
               player->attributes.state[i].current += changes[i];
        )
        unlock(&player->attributes.mutex);
    }
}

int message_player_state_execute(Message *message)
{
    AttributeConfig *cfg = message->player_state.cfg;
    Player *player = message->player_state.player;
    message_free(message);

    Attribute current[PLAYER_ATTR_NUM];
    int16_t changes[PLAYER_ATTR_NUM] = {0};

    lock(&player->attributes.mutex);
    Modifiers modifiers = player->attributes.modifiers;
    memcpy(current, player->attributes.state, PLAYER_ATTR_NUM * sizeof(Attribute));
    unlock(&player->attributes.mutex);

    if (0 == modifiers.updated) {
        modifiers.updated = time(NULL);
    }

    calculate_changes(cfg, current, &modifiers, changes);
    apply_changes(player, &modifiers, changes);

    return 0;
}
