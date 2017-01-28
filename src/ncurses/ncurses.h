#ifndef ROGUECRAFT_NCURSES_H
#define ROGUECRAFT_NCURSES_H


#include <ncurses.h>
#include "../player/player.h"


extern WINDOW *WINDOW_MAIN, *WINDOW_INVENTORY, *WINDOW_EVENT;


typedef struct {
    WINDOW *window;
    Point point;
} Click;


void ncurses_init(void);

void mouse_init(void);

void mouse_interact(Player *player);

void ncurses_cleanup(void);


#endif
