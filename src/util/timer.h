#ifndef ROGUE_CRAFT_TIMER_H
#define ROGUE_CRAFT_TIMER_H


#include <stdint.h>
#include <stdbool.h>
#include <time.h>


typedef struct Timer Timer;

typedef struct {
    void *ptr[5];
} TimerArgs;


typedef void (*TimerTask)(TimerArgs *args);


void timer_init(void);

Timer *timer_new(uint16_t timeout_ms, TimerTask task, TimerArgs args);

void timer_tick(struct timespec now);

void timer_free(Timer *timer);

void timer_cleanup(void);


#endif
