#ifndef ROGUECRAFT_UTIL_H
#define ROGUECRAFT_UTIL_H


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <errno.h>
#include <mem_pool.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include "memory/profiler.h"


#define repeat(times, body)             \
    for (int i = 0; i < times; i++) {   \
        body;                           \
    }

#define rand_in(min, max) (int16_t)(min == max ? min : (rand() % (max - min)) + min)

#define rand_in_range(r) rand_in(r.from, r.to)

#define rand_bool(probability) ((rand() / (double)RAND_MAX) < probability)

#define in_range(i, range) \
    i >= range.from && i <= range.to

#define fatal(...) fprintf(stderr, "[%s:%d:%s]", __FILE__, __LINE__, __FUNCTION__);\
    fprintf(stderr, __VA_ARGS__); fprintf(stderr, " - [errno] - %s\n", strerror(errno)); exit(EXIT_FAILURE);

#define styled(win, style, ...) \
    wattron(win, style);        \
    __VA_ARGS__;                \
    wattroff(win, style);       \

#define underline(win, ...)     \
    wattron(win, A_UNDERLINE);  \
    __VA_ARGS__;                \
    wattroff(win, A_UNDERLINE);       \

#define styled_if(win, style, cond, ...) \
    if (cond) { styled(win, style, __VA_ARGS__); } \
    else { __VA_ARGS__; }

#define has_flag(mask, flag) (flag == (flag & mask))

#define range_new(f, t) (Range) {.from = f, .to = t}

#define pool_release(pool, ptr) \
    if (0 != pool_free(pool, ptr)) { fatal("[%s] unable to release", #pool) }

#define time_diff_ms(now, since) \
    ((now.tv_sec * 1000 + now.tv_nsec / 1000000) - (since.tv_sec * 1000 + since.tv_nsec / 1000000))


typedef struct {
    char *name;
    int value;
} ConstLookup;

extern ConstLookup CONST_LOOKUP[];


static inline uint16_t sqr(uint16_t x)
{
    return x * x;
}

static inline long max(long a, long b)
{
    return a > b ? a : b;
}

static inline long min(long a, long b)
{
    return a < b ? a : b;
}

typedef struct {
    int from;
    int to;
} Range;

/**
 * @return COLOR_PAIR_* constants will be returned already wrapped in COLOR_PAIR()
 */
uint64_t constant(const char *search);

uint64_t constant_prefixed(const char *prefix, const char *name);


#endif
