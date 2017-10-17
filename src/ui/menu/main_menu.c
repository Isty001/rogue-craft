#include <ncurses.h>
#include <wchar.h>
#include <menu.h>
#include "gamestate/gamestate.h"
#include "main_menu.h"
#include "menu.h"


// source: http://patorjk.com/software/taag/#p=display&f=Bloody&t=Rogue%20Craft
static wchar_t *LOGO[10] = {
    L" ██▀███   ▒█████    ▄████  █    ██ ▓█████     ▄████▄   ██▀███   ▄▄▄        █████▒▄▄▄█████▓",
    L"▓██ ▒ ██▒▒██▒  ██▒ ██▒ ▀█▒ ██  ▓██▒▓█   ▀    ▒██▀ ▀█  ▓██ ▒ ██▒▒████▄    ▓██   ▒ ▓  ██▒ ▓▒",
    L"▓██ ░▄█ ▒▒██░  ██▒▒██░▄▄▄░▓██  ▒██░▒███      ▒▓█    ▄ ▓██ ░▄█ ▒▒██  ▀█▄  ▒████ ░ ▒ ▓██░ ▒░",
    L"▒██▀▀█▄  ▒██   ██░░▓█  ██▓▓▓█  ░██░▒▓█  ▄    ▒▓▓▄ ▄██▒▒██▀▀█▄  ░██▄▄▄▄██ ░▓█▒  ░ ░ ▓██▓ ░ ",
    L"░██▓ ▒██▒░ ████▓▒░░▒▓███▀▒▒▒█████▓ ░▒████▒   ▒ ▓███▀ ░░██▓ ▒██▒ ▓█   ▓██▒░▒█░      ▒██▒ ░ ",
    L"░ ▒▓ ░▒▓░░ ▒░▒░▒░  ░▒   ▒ ░▒▓▒ ▒ ▒ ░░ ▒░ ░   ░ ░▒ ▒  ░░ ▒▓ ░▒▓░ ▒▒   ▓▒█░ ▒ ░      ▒ ░░   ",
    L"  ░▒ ░ ▒░  ░ ▒ ▒░   ░   ░ ░░▒░ ░ ░  ░ ░  ░     ░  ▒     ░▒ ░ ▒░  ▒   ▒▒ ░ ░          ░    ",
    L"  ░░   ░ ░ ░ ░ ▒  ░ ░   ░  ░░░ ░ ░    ░      ░          ░░   ░   ░   ▒    ░ ░      ░      ",
    L"   ░         ░ ░        ░    ░        ░  ░   ░ ░         ░           ░  ░                 ",
    L"                                             ░                                            "
};


static MainMenuAction get_action(Menu *menu)
{
    MainMenuAction action = (MainMenuAction) item_userptr(current_item(menu->base));
    menu_free(menu);

    return action;
}

static Menu *create_menu(GameState *state, WINDOW *win)
{
    Menu *menu;

    if (state->in_game) {
        menu = menu_new(win, 3, "Resume", "New Game", "Quit to Main Menu");
        menu_set_item_data(menu, MAIN_MENU_RESUME, MAIN_MENU_NEW_GAME, MAIN_MENU_QUIT);
    } else {
        menu = menu_new(win, 2, "New Game", "Quit Game");
        menu_set_item_data(menu, MAIN_MENU_NEW_GAME, MAIN_MENU_QUIT);
    }
    post_menu(menu->base);

    return menu;
}

MainMenuAction menu_main_display(GameState *state)
{
    ncurses_display_menu_windows();

    uint16_t x = (getmaxx(WINDOW_MAIN) - wcslen(LOGO[0])) / 2;
    uint16_t y = 5;

    styled(WINDOW_MAIN, COLOR_PAIR(COLOR_RED_F),
           repeat(10,
                  mvwaddwstr(WINDOW_MAIN, y++, x, LOGO[i]);
           )
    )
    menu_main_add_notifications(WINDOW_MAIN);
    wrefresh(WINDOW_MAIN);

    WINDOW *win = ncurses_newwin_adjust(size_new(10, 20), WINDOW_MAIN);
    Menu *menu = create_menu(state, win);

    int input;

    while ((input = wgetch(menu->window))) {
        switch (input) {
            case '\n':
            case KEY_ENTER:
                return get_action(menu);
            default:
                menu_process_defaults(menu, input);
        }
    }

    return MAIN_MENU_RESUME;
}
