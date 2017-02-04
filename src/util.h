#ifndef ROGUECRAFT_UTIL_H
#define ROGUECRAFT_UTIL_H


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#define repeat(times, body)             \
    for (int i = 0; i < times; i++) {   \
        body;                           \
    }

#define rand_in(min, max) (uint16_t)(min == max ? min : (rand() % (max - min)) + min)

#define rand_in_range(r) rand_in(r.from, r.to)

#define rand_true(probability) ((rand() / (double)RAND_MAX) < probability)

#define in_range(i, range) \
    i >= range.from && i <= range.to

#define fatal(...) fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE);

#define styled(win, style, ...) \
    wattron(win, style);        \
    __VA_ARGS__;                \
    wattroff(win, style)        \

#define has_flag(mask, flag) (flag == (flag & mask))

#ifdef DEBUG_MODE

#define size_dump(s) printf("Height: %d Width: %d\n", s.height, s.width)

#define point_dump(p) printf("Y: %d X: %d\n", p.y, p.x)

#define range_dump(r)printf("Min: %d Max: %d\n", r.min, r.max)

#endif


static inline uint16_t square(uint16_t x)
{
    return x * x;
}

static inline int16_t min(int16_t a, int16_t b)
{
    return a < b ? a : b;
}

static inline int16_t max(int16_t a, int16_t b)
{
    return a > b ? a : b;
}

static inline void *alloc(unsigned size)
{
    void *ptr = malloc(size);

    if (!ptr) {
        fatal("Unable to allocate memory count of [%u]", size);
    }

    return ptr;
}

typedef struct {
    int from;
    int to;
} Range;


#endif
