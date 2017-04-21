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
#define profile_lighted_cell(op) PROFILER.lighted_cell op
#define profile_item(op) PROFILER.item op
#define profile_list_node(op) PROFILER.list_node op

#define size_dump(s) printf("Height: %d Width: %d\n", s.height, s.width)
#define point_dump(p) printf("Y: %d X: %d\n", p.y, p.x)
#define ptr_dump(p) printf("%p\n", p)
#define range_dump(r) printf("From: %d To: %d\n", r.from, r.to)
#define rectangle_dump(r)                               \
    printf("Size: [H:%d W:%d] Point: [Y:%d X:%d]\n",      \
        r.size.height, r.size.width, r.left_upper.y, r.left_upper.x)


void profiler_init();

void profiler_display(void);

void profile_allocate(unsigned size, void *ptr);

void profile_reallocate(void *old, void *new, unsigned size);

void profile_release(void *ptr);

void profiler_cleanup(void);

#endif
