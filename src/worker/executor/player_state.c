#include "../worker.h"


#define calculate_fatigue_damage(changes, limit, type, damage)                     \
    if (damage->limit >= changes[type] + attributes[type].current) {               \
        changes[HEALTH] += rand_in_range(damage->health);                          \
        changes[STAMINA] += rand_in_range(damage->stamina);                        \
    }


static void calculate_fatigue_damages(FatigueDamage *damage, int16_t *changes, Attribute *attributes)
{
    calculate_fatigue_damage(changes, hunger_limit, HUNGER, damage);
    calculate_fatigue_damage(changes, thirst_limit, THIRST, damage);
}

static void calculate_fatigue_change(int16_t *changes, AttributeType type, Range value, Attribute *attribute)
{
    int16_t change = changes[type] + rand_in_range(value);
    int16_t new = attribute->current + change;

    if (new <= attribute->max) {
        changes[type] = change;
    }
}

static void calculate_fatigue_changes(FatigueChanges *change, int16_t *changes, Attribute *attributes)
{
    calculate_fatigue_change(changes, THIRST, change->thirst, &attributes[THIRST]);
    calculate_fatigue_change(changes, HUNGER, change->hunger, &attributes[HUNGER]);
}

static void calculate_changes(AttributeConfig *cfg, Attribute *attributes, Modifiers *modifiers, int16_t *changes)
{
    if (cfg->dealt_damage.limit < modifiers->dealt_damage) {
        calculate_fatigue_changes(&cfg->dealt_damage, changes, attributes);
    }

    if (cfg->traveled.limit < modifiers->traveled) {
        calculate_fatigue_changes(&cfg->traveled, changes, attributes);
    }

    if (cfg->time.limit < difftime(time(NULL), modifiers->updated)) {
        calculate_fatigue_changes(&cfg->traveled, changes, attributes);
        modifiers->updated = time(NULL);
    }

    calculate_fatigue_damages(&cfg->damage, changes, attributes);
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
        player->attributes.modifiers.updated = modifiers->updated;

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
