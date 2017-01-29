#include <ncurses.h>

/**
 * @see http://melvilletheatre.com/articles/ncurses-extended-characters/index.html
 */
int main(void)
{
    int counter, line = 0;
    initscr();

    for (counter = 43; counter < 256; counter++) {
        printw("%3d = ", counter);
        addch(counter | A_ALTCHARSET);
        if (line < 7) {
            addch(' ');
            addch(ACS_VLINE);
            printw(" ");
        }
        line++;
        if (line > 7) {
            addch('\n');
            line = 0;
        }
        switch (counter) {
            case 46:
                counter = 47;
                break;
            case 48:
                counter = 95;
                break;
            case 97:
                counter = 101;
                break;
            case 126:
                counter = 127;
                break;
            case 128:
                counter = 160;
                break;
            case 172:
                counter = 173;
        }
    }
    refresh();

    return 0;
}
