#include "debug.h"


#ifdef DEBUG_MODE

#include "player/item.h"


void display_debugger(void)
{
    mvwprintw(WINDOW_MAIN, 1, 2, "CELL POOL: [%d][%d byte]", PROFILER.cell, PROFILER.cell * sizeof(Cell));
    mvwprintw(WINDOW_MAIN, 2, 2, "ITEM POOL: [%d][%d byte]", PROFILER.item, PROFILER.item * sizeof(Item));

    wrefresh(WINDOW_MAIN);
}


#endif
