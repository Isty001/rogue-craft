#include "debug.h"


#ifdef DEBUG_MODE

#include "player/item.h"
#include "worker/worker.h"


void display_profiler()
{
    mvwprintw(WINDOW_MAIN, 1, 2, "CELL POOL: [%d][%d byte]", PROFILER.cell, PROFILER.cell * sizeof(Cell));
    mvwprintw(WINDOW_MAIN, 2, 2, "ITEM POOL: [%d][%d byte]", PROFILER.item, PROFILER.item * sizeof(Item));

    mvwprintw(WINDOW_MAIN, 1, 40, "WORKER: Messages: Pool: [%d][%d byte] In Queue: [%d]",
              PROFILER.message, PROFILER.message * sizeof(Message),  worker_remaining()
    );

    wrefresh(WINDOW_MAIN);
}


#endif
