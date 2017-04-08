#ifndef ROGUECRAFT_NCURSES_H
#define ROGUECRAFT_NCURSES_H


#include <ncurses.h>
#include <panel.h>
#include "../level/point.h"
#include "../event.h"


#define GRID_CELL_SIZE 2
#define GRID_BOX_CHAR_ADJUST 1
#define GRID_PADDING 1


/** Characters in the terminal appear higher than wider */
#define NCURSES_WIDTH_MULTIPLIER 2


#define refresh_boxed(w) box(w, 0, 0); wrefresh(w);

#ifndef UNIT_TEST
#define ncurses_event(msg, ...)                             \
    wprintw(WINDOW_EVENT, "\n " msg, ##__VA_ARGS__);     \
    refresh_boxed(WINDOW_EVENT);
#else
#define ncurses_event(msg, ...)
#endif


extern WINDOW *WINDOW_MAIN, *WINDOW_INVENTORY_SHORTCUT, *WINDOW_EVENT, *WINDOW_PLAYER_ATTRIBUTES;

extern WINDOW **WINDOW_LIST[];


typedef struct Player Player;

typedef struct Panel Panel;

typedef struct {
    WINDOW *window;
    Size size;
    Point left_upper;
    Point selected;
} Grid;

typedef void (*GridForeach)(Point current);

typedef struct {
    int input;
    Player *player;
} InputEvent;

typedef struct {
    Point point;
    Player *player;
    WINDOW *window;
} ClickEvent;


void ncurses_init(void);

WINDOW *ncurses_newwin_adjust(Size size, WINDOW *adjust);

void input_process(int input, Player *player);

void mouse_init(void);

EventError mouse_handler(InputEvent *event);

PANEL *panel_push_new(WINDOW *win);

bool panel_is_open(void);

void panel_hide(void);

void panel_show(void);

void panel_close_top(void);

void grid_new(Grid *grid, WINDOW *window, Size size, Point left_upper);

void grid_display(Grid *grid);

void grid_foreach(Grid *grid, GridForeach visit);

void ncurses_cleanup(void);


#endif
