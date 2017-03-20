#include "debug.h"


#ifdef DEBUG_MODE

#include "player/item.h"
#include "worker/worker.h"
#include "level/lighting.h"


typedef struct Chunk {
    unsigned size;
    void *ptr;
} Chunk;


static int sum_chunks(void)
{
    int sum = 0;
    List *chunks = PROFILER.chunks;

    return *(int *) chunks->fold_l(chunks, &sum, (Fold) function(int *, (int *folding, Chunk *chunk) {
        *folding += chunk->size;
        return folding;
    }));
}

void _profiler_display(void)
{
    mvwprintw(WINDOW_MAIN, 1, 2, "CELL POOL: [%d][%d byte]", PROFILER.cell, PROFILER.cell * sizeof(Cell));

    mvwprintw(WINDOW_MAIN, 2, 2, "LIGHTED CELL POOL: [%d][%d byte]", PROFILER.lighted_cell,
              PROFILER.lighted_cell * lighted_cell_size());

    mvwprintw(WINDOW_MAIN, 3, 2, "ITEM POOL: [%d][%d byte]", PROFILER.item, PROFILER.item * sizeof(Item));

    mvwprintw(WINDOW_MAIN, 4, 2, "LIST NODE POOL: [%d][%d byte]", PROFILER.list_node, PROFILER.list_node * list_node_size());

    mvwprintw(WINDOW_MAIN, 5, 2,
              "MALLOC: [%d][%d byte]", PROFILER.chunks->count, sum_chunks());

    mvwprintw(WINDOW_MAIN, 1, 40, "WORKER: Messages: Pool: [%d][%d byte] In Queue: [%d]",
              PROFILER.message, PROFILER.message * sizeof(Message), worker_remaining()
    );

    wrefresh(WINDOW_MAIN);
}

void _profile_allocate(unsigned size, void *ptr)
{
    Chunk *chunk = malloc(sizeof(Chunk));
    chunk->size = size;
    chunk->ptr = ptr;

    PROFILER.chunks->append(PROFILER.chunks, chunk);
}

void _profile_reallocate(void *old, void *new, unsigned size)
{
    PROFILER.chunks->delete(PROFILER.chunks, old);

    _profile_allocate(size, new);
}

void _profile_release(void *ptr)
{
    PROFILER.chunks->filter(PROFILER.chunks, (Predicate) function(bool, (Chunk * chunk) {
        return ptr != chunk->ptr;
    }));
}

void _profiler_cleanup(void)
{
    if (0 != PROFILER.chunks->count) {
        printf("WARNING: [%d byte] in [%d] chunks, still in use!",
               sum_chunks(), PROFILER.chunks->count
        );
    }
    PROFILER.chunks->free(PROFILER.chunks);
}

#endif
