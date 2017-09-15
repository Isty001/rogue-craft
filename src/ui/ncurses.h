#ifndef ROGUECRAFT_NCURSES_H
#define ROGUECRAFT_NCURSES_H


#include <ncurses.h>
#include "level/point.h"
#include "util/environment.h"
#include "util/event.h"


/** Characters in the terminal appear higher than wider */
#define NCURSES_WIDTH_MULTIPLIER 2

#define NCURSES_BOX_PADDING 3
#define NCURSES_SIDE_NONE NCURSES_ACS(' ')
#define NCURSES_EVENT_MAX 200


#define refresh_boxed(w) wborder(w,                                                                     \
     NCURSES_SIDE_NONE, NCURSES_SIDE_NONE, NCURSES_SIDE_NONE, NCURSES_SIDE_NONE,    \
     ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);                                           \
     wrefresh(w);


#define NCURSES_WINDOW_NUM 4

extern WINDOW *WINDOW_MAIN, *WINDOW_INVENTORY_SHORTCUT, *WINDOW_EVENT, *WINDOW_PLAYER_ATTRIBUTES;

extern WINDOW **WINDOW_LIST[NCURSES_WINDOW_NUM];


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

WINDOW *ncurses_newwin_adjust(Size size, WINDOW *adjust_to);

void ncurses_resize(InputEvent *event);

void ncurses_event(char *msg, ...);

void ncurses_cleanup(void);

void input_process(int input, Player *player);

void mouse_init(void);

void mouse_handler(InputEvent *event);


#endif
