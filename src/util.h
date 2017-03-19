#ifndef ROGUECRAFT_UTIL_H
#define ROGUECRAFT_UTIL_H


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <worker.h>
#include <memory.h>
#include <errno.h>
#include <mem_pool.h>
#include "debug.h"


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
    fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE);

#define styled(win, style, ...) \
    wattron(win, style);        \
    __VA_ARGS__;                \
    wattroff(win, style)        \

#define styled_if(win, style, cond, ...) \
    if (cond) { styled(win, style, __VA_ARGS__); } \
    else { __VA_ARGS__; }

#define has_flag(mask, flag) (flag == (flag & mask))

#define range_new(f, t) (Range) {.from = f, .to = t}

#define pool_release(pool, ptr) \
    if (0 != pool_free(pool, ptr)) { fatal("[%s] unable to release", #pool) }

#define safe_alloc(...)             \
    if (0 >= size) return NULL;     \
    __VA_ARGS__                     \
    if (NULL == ptr) {              \
        fatal("[%s] failed to allocate [%u]byte memory ", __FUNCTION__, size);  \
    } profile_allocate(size, ptr);


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

static inline void *allocate(int size)
{
    safe_alloc(
        void *ptr = malloc(size);
    )

    return ptr;
}

static inline void *callocate(int nmemb, int size)
{
    safe_alloc(
        void *ptr = calloc(nmemb, size);
    )

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

/**
 * @return COLOR_PAIR_* constants will be returned already wrapped in COLOR_PAIR()
 */
uint64_t constant(const char *search);

void list_node_pool_init(void);

void list_node_pool_cleanup(void);


#endif
