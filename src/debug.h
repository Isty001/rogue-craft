#ifndef ROGUECRAFT_DEBUGGER_H
#define ROGUECRAFT_DEBUGGER_H


#ifdef DEBUG_MODE
#ifndef UNIT_TEST


#include <stdint.h>
#include <list.h>


struct {
    uint16_t cell;
    uint16_t lighted_cell;
    uint16_t item;
    uint16_t list_node;
    uint16_t message;
    List *chunks;
} PROFILER;


#define profile_cell(op) PROFILER.cell op
#define profile_lighted_cell(op) PROFILER.lighted_cell op
#define profile_item(op) PROFILER.item op
#define profile_message(op) PROFILER.message op
#define profile_list_node(op) PROFILER.list_node op

#define profile_allocate(size, ptr) _profile_allocate(size, ptr);
#define profile_release(ptr) _profile_release(ptr);

#define profiler_init()                       \
        PROFILER.chunks = list_new();         \
        PROFILER.chunks->release_item = free; \
        PROFILER.chunks->release_node = free; \
        PROFILER.chunks->alloc_node = malloc; \

#define profiler_cleanup() _profiler_cleanup();
#define profiler_display() _profiler_display();

#define size_dump(s) printf("Height: %d Width: %d\n", s.height, s.width)
#define point_dump(p) printf("Y: %d X: %d\n", p.y, p.x)
#define range_dump(r) printf("From: %d To: %d\n", r.from, r.to)
#define rectangle_dump(r)                               \
    printf("Size: [H:%d W:%d] Point: [Y:%d X:%d]\n",      \
        r.size.height, r.size.width, r.left_upper.y, r.left_upper.x)


void _profiler_display(void);

void _profile_allocate(unsigned size, void *ptr);

void _profile_release(void *ptr);

void _profiler_cleanup(void);


#else


#define profile_cell(op)
#define profile_item(op)
#define profile_lighted_cell(op)
#define profile_message(op)
#define profile_list_node(op)

#define profile_allocate(count, ptr)
#define profile_release(ptr)

#define profiler_init()
#define profiler_cleanup()
#define profiler_display


#endif
#endif
#endif
