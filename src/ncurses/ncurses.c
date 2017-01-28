#include <ncurses.h>
#include <assert.h>
#include "../color.h"
#include "../level/level.h"


static Size SIZE;
WINDOW *WINDOW_MAIN, *WINDOW_INVENTORY;


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
    WINDOW_INVENTORY = ncurses_subwin(0.8, 0.4, 0.1, 0);

    refresh();
}

void ncurses_cleanup(void)
{
    delwin(WINDOW_MAIN);
    delwin(WINDOW_INVENTORY);
    endwin();
}
