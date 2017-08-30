#include <time.h>
#include <list.h>
#include <ui/ncurses.h>
#include "memory/memory.h"
#include "timer.h"


struct Timer {
    uint16_t timeout;
    TimerTask task;
    TimerArgs args;
    intmax_t last_execution;
};


static List *TIMERS;


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
    timer->last_execution = 0;

    TIMERS->append(TIMERS, timer);

    return timer;
}

static void tick(Timer *timer)
{
    intmax_t now = time_now_ms();

    if (now - timer->last_execution >= timer->timeout) {
        timer->task(&timer->args);
        timer->last_execution = now;
    }
}

void timer_tick(void)
{
    TIMERS->foreach_l(TIMERS, tick);
}

