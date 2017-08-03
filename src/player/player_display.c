#include "../player/player.h"


#define SEPARATOR '|'
#define PADDING 2


static void display_attribute_bar(int width, Attribute *attr, WINDOW *win)
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

void player_attributes_display(Player *player)
{
    Attribute *attr;
    WINDOW *win = WINDOW_PLAYER_ATTRIBUTES;
    int width = getmaxx(win) - 2 * PADDING;
    int line = 0;

    for (int i = 0; i < PLAYER_ATTR_NUM; i++) {
        attr = &player->state.attributes[i];

        styled(win, attr->style,
               mvwprintw(win, ++line, PADDING, attr->name);
        );
        wprintw(win, ": %d/%d", attr->current, attr->max);
        wmove(win, ++line, PADDING);

        display_attribute_bar(width, attr, win);
        line++;
    }

    refresh_boxed(win);
}

