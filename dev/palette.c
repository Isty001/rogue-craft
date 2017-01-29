#include <ncurses.h>


int main(void)
{
    initscr();
    start_color();
    use_default_colors();

    for (short i = 0; i < COLORS; ++i) {

        attron(COLOR_PAIR(COLOR_BLACK));
        printw("|%d", i);
        attroff(COLOR_PAIR(COLOR_BLACK));

        init_pair(i, i, i);
        attron(COLOR_PAIR(i));
        printw("|  ");
        attroff(COLOR_PAIR(i));

    }
    refresh();

    return 0;
}
