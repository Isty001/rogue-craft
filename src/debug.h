#ifndef ROGUECRAFT_DEBUGGER_H
#define ROGUECRAFT_DEBUGGER_H


#ifdef DEBUG_MODE


#include <stdint.h>
#include "level/level.h"


struct {
    uint16_t cell;
    uint16_t item;
    uint16_t message;
} PROFILER;


#define profile_cell(op) PROFILER.cell op
#define profile_item(op) PROFILER.item op
#define profile_message(op) PROFILER.message op
#define profiler_display() display_profiler();

#define size_dump(s) printf("Height: %d Width: %d\n", s.height, s.width)
#define point_dump(p) printf("Y: %d X: %d\n", p.y, p.x)
#define range_dump(r) printf("Min: %d Max: %d\n", r.min, r.max)
#define rectangle_dump(r)                               \
    printf("Size: [H:%d W:%d] Point: [Y:%d X:%d]\n",      \
        r.size.height, r.size.width, r.left_upper.y, r.left_upper.x)


void display_profiler();


#else

#define dbg_cell(op)
#define dbg_item(op)
#define dbg_display

#endif
#endif
