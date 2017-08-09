#ifndef ROGUE_CRAFT_TIMER_H
#define ROGUE_CRAFT_TIMER_H


#include <stdint.h>
#include <stdbool.h>


typedef struct Timer Timer;

typedef struct {
    void *ptr[5];
} TimerArgs;


typedef void (*TimerTask)(TimerArgs args);


Timer *timer_new(uint16_t timeout_ms, TimerTask task, TimerArgs args);

void timer_init(void);

void timer_tick(void);

void timer_cleanup(void);


#endif
