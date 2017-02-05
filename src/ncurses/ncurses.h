#ifndef ROGUECRAFT_NCURSES_H
#define ROGUECRAFT_NCURSES_H


#include <ncurses.h>
#include "../player/player.h"


#define PADDING 2


#define refresh_boxed(w) box(w, 0, 0); wrefresh(w);

#ifndef UNIT_TEST
#define event(msg, ...)                                     \
    wprintw(WINDOW_EVENT, " " msg "\n", ##__VA_ARGS__);     \
    refresh_boxed(WINDOW_EVENT);
#else
#define event(msg, ...)
#endif


extern WINDOW *WINDOW_MAIN, *WINDOW_INVENTORY, *WINDOW_EVENT, *WINDOW_PLAYER_ATTRIBUTES;


void ncurses_init(void);

void mouse_init(void);

void mouse_interact(Player *player);

void input_process(int input, Player *player);

void ncurses_cleanup(void);


#endif
