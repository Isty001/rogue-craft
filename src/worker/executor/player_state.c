#include "../worker.h"


#define DAMAGE_FATIGUE 10


static void calculate_changes(Modifiers *modifiers, int16_t *changes)
{
    if (DAMAGE_FATIGUE < modifiers->dealt_damage) {
        changes[THIRST] += rand_in(2, 6);
        changes[HUNGER] += rand_in(1, 5);
    }

    if (2 < difftime(time(NULL), modifiers->synchronized)) {
        changes[THIRST] += rand_in(1, 3);
        changes[HUNGER] += rand_in(1, 2);
        modifiers->synchronized = time(NULL);
    }
}

static bool has_any_changes(int16_t *changes)
{
    return changes[HUNGER] || changes[THIRST] || changes[STAMINA];
}

static void copy_changes(Player *player, Modifiers *modifiers, int16_t *changes)
{
    if (has_any_changes(changes)) {
        lock(&player->attributes.mutex);
        player->attributes.modifiers.dealt_damage -= modifiers->dealt_damage;
        player->attributes.modifiers.traveled -= modifiers->traveled;
        player->attributes.modifiers.synchronized = modifiers->synchronized;
        player->attributes.state[HUNGER].current += changes[HUNGER];
        player->attributes.state[THIRST].current += changes[THIRST];
        unlock(&player->attributes.mutex);
    }
}

int execute_player_state(Message *message)
{
    Player *player = message->ptr;
    int16_t changes[PLAYER_ATTR_NUM] = {0};

    lock(&player->attributes.mutex);
    Modifiers modifiers = player->attributes.modifiers;
    unlock(&player->attributes.mutex);

    if (0 == modifiers.synchronized) {
        modifiers.synchronized = time(NULL);
    }

    calculate_changes(&modifiers, changes);
    copy_changes(player, &modifiers, changes);
    message_free(message);

    return 0;
}
