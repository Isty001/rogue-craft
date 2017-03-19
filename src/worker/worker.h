#ifndef ROGUE_CRAFT_MESSAGES_H
#define ROGUE_CRAFT_MESSAGES_H


#include <worker.h>
#include "../player/player.h"


#define MESSAGE_TYPE_NUM 1


typedef enum {
    PLAYER_STATE
} MessageType;

typedef struct {
    Player *player;
    AttributeConfig *cfg;
} PlayerStateMessage;

typedef struct {
    MessageType type;
    union {
        PlayerStateMessage player_state;
    };
} Message;


void worker_init(void);

void worker_cleanup(void);

void message_pool_init(void);

Message *message_allocate(void);

void message_free(Message *message);

void message_pool_cleanup(void);

void message_player_state_send(Player *player);

int message_player_state_execute(Message *message);

uint16_t worker_remaining(void);


#endif
