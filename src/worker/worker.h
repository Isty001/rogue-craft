#ifndef ROGUE_CRAFT_MESSAGES_H
#define ROGUE_CRAFT_MESSAGES_H


#include <worker.h>
#include "../player/player.h"


#define MESSAGE_TYPE_NUM 1


typedef enum {
    PLAYER_STATE
} MessageType;

typedef struct {
    void *ptr;
    int integer;
    MessageType type;
} Message;


void worker_init(void);

void worker_cleanup(void);

void message_pool_init(void);

void message_pool_cleanup(void);

void message_send_player_state(Player *player);

int execute_player_state(Message *message);

void message_free(Message *message);


#ifdef DEBUG_MODE

uint16_t worker_remaining(void);

#endif


#endif
