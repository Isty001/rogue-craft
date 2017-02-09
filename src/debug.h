#ifndef ROGUECRAFT_DEBUGGER_H
#define ROGUECRAFT_DEBUGGER_H


#ifdef DEBUG_MODE


#include <stdint.h>


struct {
    uint16_t cell;
    uint16_t item;
} PROFILER;


#define dbg_cell(op) PROFILER.cell op
#define dbg_item(op) PROFILER.item op
#define dbg_display() display_debugger();


void display_debugger(void);


#else

#define dbg_cell(op)
#define dbg_item(op)
#define dbg_display

#endif
#endif
