#include <time.h>
#include <list.h>
#include <sys/time.h>
#include <stdio.h>
#include "memory/memory.h"
#include "timer.h"


struct Timer {
    uint16_t timeout;
    TimerTask task;
    void *arg;
    time_t last_execution;
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

static time_t now_ms(void)
{
    struct timeval now;
    gettimeofday(&now, NULL);

    return now.tv_sec * 1000 + now.tv_usec / 1000;
}

Timer *timer_new(uint16_t timeout_ms, TimerTask task, void *arg)
{
    Timer *timer = mem_alloc(sizeof(Timer));
    timer->task = task;
    timer->arg = arg;
    timer->timeout = timeout_ms;

    TIMERS->append(TIMERS, timer);

    return timer;
}


static void tick(Timer *timer)
{
    time_t now = now_ms();

    if (now - timer->last_execution >= timer->timeout) {

        timer->task(timer->arg);
        timer->last_execution = now;
    }
}

void timer_tick(void)
{
    TIMERS->foreach_l(TIMERS, tick);
}

