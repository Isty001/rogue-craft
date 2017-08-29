#include "item/item.h"


typedef struct Chunk {
    void *ptr;
    unsigned size;
} Chunk;


void profiler_init(void)
{
    PROFILER.chunks = list_new();
    PROFILER.chunks->release_item = free;
    PROFILER.chunks->release_node = free;
    PROFILER.chunks->alloc_node = malloc;
}

static int sum_chunks(void)
{
    int sum = 0;
    List *chunks = PROFILER.chunks;

    return *(int *) chunks->fold_l(chunks, &sum, (Fold) function(int *, (int *folding, Chunk *chunk) {
        *folding += chunk->size;
        return folding;
    }));
}

void profiler_display(void)
{
    if (!env_is_debug_mode()) {
        return;
    }
    mvwprintw(WINDOW_MAIN, 1, 2, "CELL POOL: [%d][%d byte]", PROFILER.cell, PROFILER.cell * sizeof(Cell));

    mvwprintw(WINDOW_MAIN, 2, 2, "ITEM POOL: [%d][%d byte]", PROFILER.item, PROFILER.item * sizeof(Item));

    mvwprintw(WINDOW_MAIN, 3, 2, "LIST NODE POOL: [%d][%d byte]", PROFILER.list_node,
              PROFILER.list_node * list_node_size());

    mvwprintw(WINDOW_MAIN, 4, 2,
              "MALLOC: [%d][%d byte]", PROFILER.chunks->count, sum_chunks());

    wrefresh(WINDOW_MAIN);
}

void profile_allocate(unsigned size, void *ptr)
{
    Chunk *chunk = malloc(sizeof(Chunk));
    chunk->size = size;
    chunk->ptr = ptr;

    PROFILER.chunks->append(PROFILER.chunks, chunk);
}

void profile_reallocate(void *old, void *new, unsigned size)
{
    PROFILER.chunks->delete(PROFILER.chunks, old);

    profile_allocate(size, new);
}

void profile_release(void *ptr)
{
    PROFILER.chunks->filter(PROFILER.chunks, (Predicate) function(bool, (Chunk * chunk) {
        return ptr != chunk->ptr;
    }));
}

void profiler_cleanup(void)
{
    if (env_is_debug_mode() && 0 != PROFILER.chunks->count) {
        printf("WARNING: [%d byte] in [%d] chunks, still in use!",
               sum_chunks(), PROFILER.chunks->count
        );
    }
    PROFILER.chunks->free(PROFILER.chunks);
}


