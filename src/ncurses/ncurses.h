#ifndef ROGUECRAFT_NCURSES_H
#define ROGUECRAFT_NCURSES_H


#include <ncurses.h>
#include "level/point.h"
#include "util/environment.h"
#include "util/event.h"


/** Characters in the terminal appear higher than wider */
#define NCURSES_WIDTH_MULTIPLIER 2


#define refresh_boxed_ch(w, ch) box(w, ch, ch); wrefresh(w);

#define refresh_boxed(w) refresh_boxed_ch(w, 0);

#define ncurses_event(msg, ...)                              \
    if (!getenv(ENV_NCURSES_INACTIVE)) {                    \
        wprintw(WINDOW_EVENT, "\n " msg, ##__VA_ARGS__);     \
        refresh_boxed(WINDOW_EVENT)                          \
    };


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

WINDOW *ncurses_newwin_adjust(Size size, WINDOW *adjust);

EventError ncurses_resize(InputEvent *event);

void ncurses_cleanup(void);

void input_process(int input, Player *player);

void mouse_init(void);

EventError mouse_handler(InputEvent *event);


#endif
