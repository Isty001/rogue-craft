#ifndef ROGUECRAFT_UTIL_H
#define ROGUECRAFT_UTIL_H


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <worker.h>
#include <memory.h>
#include <errno.h>
#include "debug.h"


#define repeat(times, body)             \
    for (int i = 0; i < times; i++) {   \
        body;                           \
    }

#define rand_in(min, max) (int16_t)(min == max ? min : (rand() % (max - min)) + min)

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

#ifndef UNIT_TEST

#define lock(mutex) pthread_check(pthread_mutex_lock(mutex))

#define unlock(mutex) pthread_check(pthread_mutex_unlock(mutex))

#else

#define lock(mutex)

#define unlock(mutex)

#endif


typedef struct {
    char *name;
    int value;
} ConstLookup;

extern ConstLookup CONST_LOOKUP[];

int constant(const char *search);


static inline uint16_t sqr(uint16_t x)
{
    return x * x;
}

static inline uint16_t max(uint16_t a, uint16_t b)
{
    return a < b ? a : b;
}

static inline void *allocate(unsigned size)
{
    if (0 == size) return NULL;

    void *ptr = malloc(size);

    if (!ptr) {
        fatal("Unable to allocate memory count of [%u]", size);
    }
    profile_allocate(size, ptr);

    return ptr;
}

static inline void release(void *ptr)
{
    if (NULL == ptr) {
        return;
    }

    free(ptr);
    profile_release(ptr);
}

typedef struct {
    int from;
    int to;
} Range;


#endif
