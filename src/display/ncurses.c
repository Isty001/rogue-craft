#include <ncurses.h>
#include <assert.h>
#include "../color.h"
#include "../level/level.h"


static Size SIZE;
WINDOW *WINDOW_MAIN;


WINDOW *ncurses_subwin(double height, double width, double y, double x)
{
    WINDOW *sub_window = derwin(stdscr, SIZE.height * height, SIZE.width * width, y, x);
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

    WINDOW_MAIN = ncurses_subwin(0.6, 0.6, SIZE.height * 0.1, SIZE.width * 0.2);

    refresh();
}

void ncurses_cleanup(void)
{
    delwin(WINDOW_MAIN);
    endwin();
}
