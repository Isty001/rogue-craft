#include <ncurses.h>
#include <assert.h>
#include "ncurses.h"
#include "../color.h"


static int HEIGHT, WIDTH;
WINDOW *WINDOW_MAIN, *WINDOW_INVENTORY_SHORTCUT, *WINDOW_EVENT, *WINDOW_PLAYER_ATTRIBUTES;

WINDOW **WINDOW_LIST[] = {
    &WINDOW_MAIN, &WINDOW_INVENTORY_SHORTCUT,
    &WINDOW_EVENT, &WINDOW_PLAYER_ATTRIBUTES,
    NULL
};


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

    double left_side_bar_height = HEIGHT / 2;
    double side_bar_width = WIDTH * 0.2;

    double inventory_height = 3;
    double main_height = HEIGHT - inventory_height - 1;
    double main_width = WIDTH * 0.6;

    WINDOW_MAIN = ncurses_subwin(main_height, main_width, 0, side_bar_width);
    WINDOW_EVENT = ncurses_subwin(left_side_bar_height, side_bar_width, 0, 0);
    WINDOW_PLAYER_ATTRIBUTES = ncurses_subwin(left_side_bar_height, side_bar_width, left_side_bar_height, 0);
    WINDOW_INVENTORY_SHORTCUT = ncurses_subwin(inventory_height, main_width, main_height, side_bar_width);

    box(WINDOW_INVENTORY_SHORTCUT, 0, 0);
    box(WINDOW_EVENT, 0, 0);
    box(WINDOW_PLAYER_ATTRIBUTES, 0, 0);

    refresh();
}

void ncurses_cleanup(void)
{
    delwin(WINDOW_MAIN);
    delwin(WINDOW_INVENTORY_SHORTCUT);
    delwin(WINDOW_EVENT);
    delwin(WINDOW_PLAYER_ATTRIBUTES);
    endwin();
}
