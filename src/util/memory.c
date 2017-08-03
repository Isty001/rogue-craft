#include <malloc.h>
#include "profiler.h"
#include "util.h"


#define safe_alloc(...)             \
    if (0 >= size) return NULL;     \
    __VA_ARGS__                     \
    if (NULL == ptr) {              \
        fatal("[%s] failed to allocate [%u] byte memory ", __FUNCTION__, size);  \
    }


void *mem_alloc(int size)
{
    safe_alloc(
        void *ptr = malloc(size);
    )
    profile_allocate(size, ptr);

    return ptr;
}

void *mem_calloc(int nmemb, int size)
{
    safe_alloc(
        void *ptr = calloc(nmemb, size);
    )
    profile_allocate(size, ptr);

    return ptr;
}

void *mem_realloc(void *old, unsigned size)
{
    safe_alloc(
        void *ptr = realloc(old, size);
    );
    profile_reallocate(old, ptr, size);

    return ptr;
}

void mem_dealloc(void *ptr)
{
    if (NULL == ptr) {
        return;
    }

    free(ptr);
    profile_release(ptr);
}
