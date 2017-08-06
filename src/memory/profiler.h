#ifndef ROGUECRAFT_PROFILER_H
#define ROGUECRAFT_PROFILER_H


#include <stdint.h>
#include <list.h>


struct {
    uint16_t cell;
    uint16_t lighted_cell;
    uint16_t item;
    uint16_t list_node;
    List *chunks;
} PROFILER;


#define profile_cell(op) PROFILER.cell op
#define profile_item(op) PROFILER.item op
#define profile_list_node(op) PROFILER.list_node op


void profiler_init(void);

void profiler_display(void);

void profile_allocate(unsigned size, void *ptr);

void profile_reallocate(void *old, void *new, unsigned size);

void profile_release(void *ptr);

void profiler_cleanup(void);


#endif
