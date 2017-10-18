#include <ncurses.h>
#include <assert.h>
#include "memory/memory.h"
#include "ncurses.h"


#define clear_window(win) wclear(win); wrefresh(win);


#define EVENT_MAX 20


WINDOW *WINDOW_MAIN, *WINDOW_INVENTORY_SHORTCUT, *WINDOW_EVENT, *WINDOW_PLAYER_ATTRIBUTES;

WINDOW **WINDOW_LIST[NCURSES_WINDOW_NUM] = {
    &WINDOW_MAIN, &WINDOW_INVENTORY_SHORTCUT,
    &WINDOW_EVENT, &WINDOW_PLAYER_ATTRIBUTES
};

static List *EVENTS;


WINDOW *ncurses_subwin(WINDOW *win, int height, int width, int y, int x)
{
    WINDOW *sub_window = derwin(win, height, width, y, x);
    scrollok(sub_window, true);
    keypad(sub_window, true);

    assert(NULL != sub_window);
    nodelay(sub_window, true);

    return sub_window;
}

static void clear_all(void)
{
    clear_window(WINDOW_MAIN);
    clear_window(WINDOW_EVENT);
    clear_window(WINDOW_PLAYER_ATTRIBUTES);
    clear_window(WINDOW_INVENTORY_SHORTCUT);
}

void ncurses_init(void)
{
    EVENTS = list_new();
    EVENTS->release_item = mem_dealloc;

    initscr();
    start_color();
    use_default_colors();
    curs_set(false);
    noecho();
    cbreak();
    nodelay(stdscr, true);
    keypad(stdscr, true);
}

void ncurses_display_menu_windows(void)
{
    clear_all();

    int height, width;
    getmaxyx(stdscr, height, width);

    WINDOW_MAIN = ncurses_subwin(stdscr, height, width, 0, 0);
    refresh_boxed(WINDOW_MAIN);
}

void ncurses_display_game_windows(void)
{
    int height, width;

    getmaxyx(stdscr, height, width);

    double left_side_bar_height = height / 2;
    double side_bar_width = width * 0.2;

    double inventory_height = 3;
    double main_height = height - inventory_height - 1;
    double main_width = width * 0.6;

    WINDOW_MAIN = ncurses_subwin(stdscr, main_height, main_width, 0, side_bar_width);
    WINDOW_EVENT = ncurses_subwin(stdscr, left_side_bar_height, side_bar_width, 0, 0);
    WINDOW_PLAYER_ATTRIBUTES = ncurses_subwin(stdscr, left_side_bar_height, side_bar_width, left_side_bar_height, 0);
    WINDOW_INVENTORY_SHORTCUT = ncurses_subwin(stdscr, inventory_height, main_width, main_height + 1, side_bar_width);

    refresh_boxed(WINDOW_EVENT);

    refresh();
}

void ncurses_resize(InputEvent *event)
{
    (void) event;

    ncurses_cleanup();
    ncurses_init();
}

WINDOW *ncurses_newwin_adjust(Size size, WINDOW *adjust_to)
{
    Size adjust_size;
    getmaxyx(WINDOW_MAIN, adjust_size.height, adjust_size.width);

    if (size.height > adjust_size.height || size.width > adjust_size.width) {
        fatal("Inner WINDOW can't be bigger");
    }

    int y = getbegy(adjust_to) + (adjust_size.height - size.height) / 2;
    int x = getbegx(adjust_to) + (adjust_size.width - size.width) / 2;

    return newwin(size.height, size.width, y, x);
}

static void print_events(void)
{
    wclear(WINDOW_EVENT);

    uint16_t y = NCURSES_BOX_PADDING;

    EVENTS->foreach_l(EVENTS, (Foreach) function(void, (char *msg) {
        mvwprintw(WINDOW_EVENT, y++, NCURSES_BOX_PADDING, msg);
    }));

    refresh_boxed(WINDOW_EVENT);
}

void ncurses_event(char *msg, ...)
{
    if (getenv(ENV_NCURSES_DISABLED)) {
        return;
    }

    char *buff = mem_alloc(NCURSES_EVENT_MAX);

    va_list args;
    va_start(args, msg);
    vsprintf(buff, msg, args);
    va_end(args);

    EVENTS->prepend(EVENTS, buff);

    if (EVENT_MAX < EVENTS->count) {
        EVENTS->pop(EVENTS);
    }

    print_events();
}

void ncurses_cleanup(void)
{
    delwin(WINDOW_MAIN);
    delwin(WINDOW_INVENTORY_SHORTCUT);
    delwin(WINDOW_EVENT);
    delwin(WINDOW_PLAYER_ATTRIBUTES);
    endwin();

    EVENTS->free(EVENTS);
}
