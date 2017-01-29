#include <ncurses.h>
#include <assert.h>
#include "ncurses.h"


static int HEIGHT, WIDTH;
WINDOW *WINDOW_MAIN, *WINDOW_INVENTORY, *WINDOW_EVENT, *WINDOW_PLAYER_STATS;


WINDOW *ncurses_subwin(int height, int width, int y, int x)
{
    WINDOW *sub_window = derwin(stdscr, height, width, y, x);
    scrollok(sub_window, true);
    keypad(sub_window, true);

    assert(NULL != sub_window);
    nodelay(sub_window, true);

    return sub_window;
}

void ncurses_init(void)
{
    initscr();
    color_init();
    curs_set(false);
    noecho();
    cbreak();
    nodelay(stdscr, true);

    getmaxyx(stdscr, HEIGHT, WIDTH);

    WINDOW_MAIN = ncurses_subwin(HEIGHT * 0.75, WIDTH * 0.6, 0, WIDTH * 0.2);
    WINDOW_EVENT = ncurses_subwin(HEIGHT * 0.25, WIDTH * 0.3, HEIGHT * 0.75, WIDTH * 0.2);

    WINDOW_INVENTORY = ncurses_subwin(HEIGHT * 0.75, (WIDTH - 1) * 0.2, 0, 0);
    WINDOW_PLAYER_STATS = ncurses_subwin(HEIGHT * 0.25, (WIDTH - 1) * 0.2, HEIGHT * 0.75, 0);

    box(WINDOW_INVENTORY, 0, 0);
    box(WINDOW_EVENT, 0, 0);
    box(WINDOW_PLAYER_STATS, 0, 0);

    refresh();
}

void ncurses_cleanup(void)
{
    delwin(WINDOW_MAIN);
    delwin(WINDOW_INVENTORY);
    delwin(WINDOW_EVENT);
    delwin(WINDOW_PLAYER_STATS);
    endwin();
}

void ncurses_event(char *msg)
{
#ifndef UNIT_TEST
    wprintw(WINDOW_EVENT, "%s", msg);
    wrefresh(WINDOW_EVENT);
#endif
}
