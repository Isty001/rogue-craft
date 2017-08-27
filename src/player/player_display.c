#include "../player/player.h"


#define SEPARATOR '|'



static void display_attribute_bar(int width, State *attr, WINDOW *win)
{
    int bar_width = width * ((double) attr->current / attr->max);

    for (int i = 0; i < width; i++) {
        if (i < bar_width) {
            styled(win, attr->style,
                   waddch(win, SEPARATOR);
            );
        } else {
            waddch(win, ' ');
        }
    }
}

void player_state_display(Player *player)
{
    State *state;
    WINDOW *win = WINDOW_PLAYER_ATTRIBUTES;
    int width = getmaxx(win) - 2 * NCURSES_BOX_PADDING;
    int line = NCURSES_BOX_PADDING;

    for (int i = 0; i < PLAYER_STATE_NUM; i++) {
        state = &player->state.map[i];

        styled(win, state->style,
               mvwprintw(win, ++line, NCURSES_BOX_PADDING, state->name);
        );
        wprintw(win, ": %d/%d", state->current, state->max);
        wmove(win, ++line, NCURSES_BOX_PADDING);

        display_attribute_bar(width, state, win);
        line++;
    }

    refresh_boxed(win);
}

