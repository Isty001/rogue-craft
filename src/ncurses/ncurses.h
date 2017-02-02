#ifndef ROGUECRAFT_NCURSES_H
#define ROGUECRAFT_NCURSES_H


#include <ncurses.h>
#include "../player/player.h"


#define PADDING 2


#define refresh_boxed(w) box(w, 0, 0); wrefresh(w);


extern WINDOW *WINDOW_MAIN, *WINDOW_INVENTORY, *WINDOW_EVENT, *WINDOW_PLAYER_ATTRIBUTES;


void ncurses_init(void);

void ncurses_event(char *msg);

void mouse_init(void);

void mouse_interact(Player *player);

void ncurses_cleanup(void);


#endif
