#include <ncurses.h>
#include <assert.h>
#include "../color.h"
#include "../level/level.h"


static Size SIZE;
WINDOW *WINDOW_MAIN, *WINDOW_INVENTORY, *WINDOW_EVENT;


WINDOW *ncurses_subwin(double height, double width, double y, double x)
{
    WINDOW *sub_window = derwin(stdscr, SIZE.height * height, SIZE.width * width, SIZE.height * y, SIZE.width * x);
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

    getmaxyx(stdscr, SIZE.height, SIZE.width);

    WINDOW_MAIN = ncurses_subwin(0.6, 0.6, 0.1, 0.2);
    WINDOW_INVENTORY = ncurses_subwin(0.9, 0.19, 0.1, 0);
    WINDOW_EVENT = ncurses_subwin(0.3, 0.3, 0.7, 0.19);

    box(WINDOW_INVENTORY, 0, 0);
    box(WINDOW_EVENT, 0, 0);

    refresh();
}

void ncurses_cleanup(void)
{
    delwin(WINDOW_MAIN);
    delwin(WINDOW_INVENTORY);
    delwin(WINDOW_EVENT);
    endwin();
}

void ncurses_event(char *msg)
{
#ifndef UNIT_TEST
    wprintw(WINDOW_EVENT, "%s", msg);
    wrefresh(WINDOW_EVENT);
#endif
}
