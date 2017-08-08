#ifndef ROGUE_CRAFT_TIMER_H
#define ROGUE_CRAFT_TIMER_H


#include <stdint.h>
#include <stdbool.h>


typedef struct Timer Timer;


typedef void (*TimerTask)(void *arg);


Timer *timer_new(uint16_t timeout_ms, TimerTask task, void *arg);

void timer_cleanup(void);

void timer_init(void);

void timer_tick(void);


#endif
