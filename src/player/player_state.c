#include <time.h>
#include "config.h"


static inline void change(State *attr, int16_t change)
{
    int16_t new = attr->current + change;

    if (new >= 0 && new <= attr->max) {
        attr->current = new;
    }
}

static inline void apply_fatigue_damage(State *states, FatigueDamage *damage)
{
    change(&states[HEALTH], rand_in_range(damage->health));
}

static inline void apply_fatigue(State *states, Fatigue *fatigue)
{
    change(&states[HUNGER], rand_in_range(fatigue->hunger));
    change(&states[THIRST], rand_in_range(fatigue->thirst));
    change(&states[STAMINA], rand_in_range(fatigue->stamina));
}

static void apply_fatigue_by_modifiers(Modifiers *modifiers, Fatigue *fatigue, State *states)
{
    if (modifiers->traveled >= fatigue->traveled) {
        apply_fatigue(states, fatigue);
        modifiers->traveled = 0;
    }
    if (modifiers->dealt_damage >= fatigue->dealt_damage) {
        apply_fatigue(states, fatigue);
        modifiers->dealt_damage = 0;
    }
    time_t now = time(NULL);

    if (difftime(now, modifiers->timestamp.fatigue) >= fatigue->elapsed_time) {
        apply_fatigue(states, fatigue);
        modifiers->timestamp.fatigue = now;
    }
}

static void apply_fatigue_damages(Modifiers *modifiers, FatigueDamage *damage, State *states)
{
    time_t now = time(NULL);

    if (difftime(now, modifiers->timestamp.fatigue_damage) < damage->elapsed_time) {
        return;
    }

    if (states[HUNGER].current >= damage->hunger) {
        apply_fatigue_damage(states, damage);
    }
    if (states[THIRST].current >= damage->thirst) {
        apply_fatigue_damage(states, damage);
    }

    modifiers->timestamp.fatigue_damage = now;
}

void player_state_update(TimerArgs *args)
{
    Player *player = args->ptr[0];
    PlayerStateConfig *cfg = args->ptr[1];

    State *states = player->state.map;
    Modifiers *modifiers = &player->state.modifiers;

    apply_fatigue_by_modifiers(modifiers, &cfg->fatigue, states);
    apply_fatigue_damages(modifiers, &cfg->fatigue_damage, states);
}
