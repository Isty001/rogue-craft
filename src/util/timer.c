#include <time.h>
#include <list.h>
#include "memory/memory.h"
#include "timer.h"
#include "util.h"


struct Timer {
    uint16_t timeout;
    TimerTask task;
    TimerArgs args;
    time_t next_execution;
};


static List *TIMERS;


static void set_next_execution(Timer *timer, time_t now_ms)
{
    timer->next_execution = now_ms + timer->timeout;
}

void timer_init(void)
{
    TIMERS = list_new();
    TIMERS->release_item = mem_dealloc;
}

void timer_cleanup(void)
{
    TIMERS->free(TIMERS);
}

Timer *timer_new(uint16_t timeout_ms, TimerTask task, TimerArgs args)
{
    Timer *timer = mem_alloc(sizeof(Timer));
    timer->task = task;
    timer->args = args;
    timer->timeout = timeout_ms;
    set_next_execution(timer, time_now_ms());

    TIMERS->append(TIMERS, timer);

    return timer;
}

static void tick(Timer *timer)
{
    time_t now = time_now_ms();

    if (now >= timer->next_execution) {
        timer->task(&timer->args);
        set_next_execution(timer, now);
    }
}

void timer_tick(void)
{
    TIMERS->foreach_l(TIMERS, tick);
}

