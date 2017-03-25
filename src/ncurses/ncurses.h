#ifndef ROGUECRAFT_NCURSES_H
#define ROGUECRAFT_NCURSES_H


#include <ncurses.h>
#include "../level/point.h"
#include "../event.h"


#define PADDING 2


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

void mouse_init(void);

EventError mouse_handler(InputEvent *event);

void input_process(int input, Player *player);

void ncurses_cleanup(void);


#endif
