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

#define range_new(f, t) (Range) {.from = f, .to = t}


static inline uint16_t sqr(uint16_t x)
{
    return x * x;
}

static inline uint16_t max(uint16_t a, uint16_t b)
{
    return a < b ? a : b;
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
