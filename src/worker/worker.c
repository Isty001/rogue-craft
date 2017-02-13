#include <mem_pool.h>
#include "worker.h"
#include "../debug.h"


static MemPool *MESSAGE_POOL;
static Queue *QUEUE;
static Worker *WORKER;


static Executor EXECUTOR_MAP[MESSAGE_TYPE_NUM] = {
};


static int execute_message(Message *message)
{
    return EXECUTOR_MAP[message->type](message);
}

static void error_handler(int err)
{
}

void worker_init(void)
{
    QUEUE = queue_new();
    WORKER = worker_new(QUEUE, (Executor) execute_message, error_handler, 100);

    worker_run(WORKER);
}

void worker_cleanup(void)
{
    worker_free(WORKER);
    queue_free(QUEUE);
}

static inline Message *alloc_message(void)
{
    profile_message(++);

    return pool_alloc(MESSAGE_POOL);
}

void message_pool_init(void)
{
    MESSAGE_POOL = pool_init(sizeof(Message), 100);
}

void message_pool_cleanup(void)
{
    pool_destroy(MESSAGE_POOL);
}

void message_free(Message *message)
{
    profile_message(--);
    pool_free(MESSAGE_POOL, message);
}

#ifdef DEBUG_MODE

uint16_t worker_remaining(void)
{
    return queue_count(QUEUE);
}

#endif
