#include <time.h>
#include "player.h"


static inline void change(Attribute *attr, int16_t change)
{
    int16_t new = attr->current + change;

    if (new >= 0 && new <= attr->max) {
        attr->current = new;
    }
}

static inline void apply_fatigue_damage(Attribute *attributes, FatigueDamage *damage)
{
    change(&attributes[HEALTH], rand_in_range(damage->health));
}

static inline void apply_fatigue(Attribute *attributes, Fatigue *fatigue)
{
    change(&attributes[HUNGER], rand_in_range(fatigue->hunger));
    change(&attributes[THIRST], rand_in_range(fatigue->thirst));
    change(&attributes[STAMINA], rand_in_range(fatigue->stamina));
}

static void apply_fatigue_by_modifiers(Modifiers *modifiers, Fatigue *fatigue, Attribute *attributes)
{
    if (modifiers->traveled >= fatigue->traveled) {
        apply_fatigue(attributes, fatigue);
        modifiers->traveled = 0;
    }
    if (modifiers->dealt_damage >= fatigue->dealt_damage) {
        apply_fatigue(attributes, fatigue);
        modifiers->dealt_damage = 0;
    }
    time_t now = time(NULL);

    if (difftime(now, modifiers->timestamp.fatigue) >= fatigue->elapsed_time) {
        apply_fatigue(attributes, fatigue);
        modifiers->timestamp.fatigue = now;
    }
}

static void apply_fatigue_damages(Modifiers *modifiers, FatigueDamage *damage, Attribute *attributes)
{
    if (attributes[HUNGER].current >= damage->hunger) {
        apply_fatigue_damage(attributes, damage);
    }
    if (attributes[THIRST].current >= damage->thirst) {
        apply_fatigue_damage(attributes, damage);
    }
    time_t now = time(NULL);

    if (difftime(now, modifiers->timestamp.fatigue_damage) >= damage->elapsed_time) {
        apply_fatigue_damage(attributes, damage);
        modifiers->timestamp.fatigue_damage = now;
    }
}

void player_state_update(Player *player, PlayerStateConfig *cfg)
{
    Attribute *attributes = player->state.attributes;
    Modifiers *modifiers = &player->state.modifiers;

    apply_fatigue_by_modifiers(modifiers, &cfg->fatigue, attributes);
    apply_fatigue_damages(modifiers, &cfg->fatigue_damage, attributes);
}
