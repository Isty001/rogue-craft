#ifndef ROGUE_CRAFT_MESSAGES_H
#define ROGUE_CRAFT_MESSAGES_H


#include <worker.h>


#define MESSAGE_TYPE_NUM 0


typedef enum {
    EVOLVE
} MessageType;

typedef struct {
    int integer;
    void *ptr;
    MessageType type;
} Message;


void worker_init(void);

void worker_cleanup(void);

void message_pool_init(void);

void message_pool_cleanup(void);

void message_free(Message *message);


#ifdef DEBUG_MODE

uint16_t worker_remaining(void);

#endif


#endif
