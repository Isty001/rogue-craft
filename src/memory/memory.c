#include <malloc.h>
#include "profiler.h"
#include "util/util.h"


#define safe_alloc(...)             \
    if (0 >= size) return NULL;     \
    __VA_ARGS__                     \
    if (NULL == ptr) {              \
        fatal("[%s] failed to allocate [%u] byte memory ", __FUNCTION__, size);  \
    }


static MemPool *LIST_NODE_POOL;


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
    profile_allocate(nmemb * size, ptr);

    return ptr;
}

void *mem_realloc(void *old, int size)
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

static void *alloc_node(size_t size)
{
    (void)size;
    profile_list_node(++);

    return pool_alloc(LIST_NODE_POOL);
}

static void release_node(void *node)
{
    profile_list_node(--);

    pool_release(LIST_NODE_POOL, node);
}

void list_node_pool_init(void)
{
    LIST_NODE_POOL = pool_init(list_node_size(), 100);

    list_set_allocators(alloc_node, release_node, NULL);
}

void list_node_pool_cleanup(void)
{
    pool_destroy(LIST_NODE_POOL);
}
