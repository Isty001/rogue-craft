//This include must be the first
#include <ncursesw/ncurses.h>

#include <dlfcn.h>
#include <menu.h>
#include <wchar.h>
#include "util/environment.h"
#include "gamestate/gamestate.h"
#include "loop.h"
#include "main_menu.h"
#include "menu.h"
#include "notifier.h"
#include "memory/memory.h"
#include "ui/ncurses.h"
#include "util/logger.h"


#define RELEASE_WIDTH 80
#define TIME_LINE_OFFSET RELEASE_WIDTH + 2


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

static WINDOW *RELEASE_WINDOW = NULL;
static WINDOW *TIMELINE_WINDOW = NULL;
static const Notifications *NOTIFICATIONS;

static void (*notifications_init)(void *(*allocator)(int), void*(reallocator)(void *, int), void (*deallocator)(void *), const char *cache_dir) = NULL;
static void (*notifications_load)(NotifierCallback callback) = NULL;
static void (*notifications_cleanup)(void) = NULL;


static void clear_notifications()
{
    if (RELEASE_WINDOW) {
        werase(RELEASE_WINDOW);
        wrefresh(RELEASE_WINDOW);
        clear();
    }
}

static void init_notifier(void *handle)
{
    notifications_init = dlsym(handle, "notifications_init");
    notifications_load = dlsym(handle, "notifications_load");
    notifications_cleanup = dlsym(handle, "notifications_cleanup");

    notifications_init(mem_alloc, mem_realloc, mem_dealloc, getenv(ENV_DIR_CACHE));
}

static void save_notifications(const Notifications *notifications)
{
    NOTIFICATIONS = notifications;
}

static void load_notifications(void)
{
    static bool loaded = false;
    static void *handle = NULL;

    if (!loaded) {
        const char *path = getenv(ENV_PATH_LIB_NOTIFIER) ?: "/usr/local/lib/librogue-craft-notifier.so";

        if ((handle = dlopen(path, RTLD_NOW))) {
            init_notifier(handle);
        } else {
            log_info("Unable to load notifier lib at [%s] : [%s]", path, dlerror());
        }
    }

    if (handle && !loaded) {
        notifications_load(save_notifications);
        loaded = true;
    }
}

static MainMenuAction get_action(Menu *menu)
{
    MainMenuAction action = (MainMenuAction) item_userptr(current_item(menu->base));
    menu_free(menu);
    clear_notifications();

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

static void display_release(WINDOW *win)
{
    Content release = NOTIFICATIONS->latest_release;

    if (release.count) {
        underline(win, mvwprintw(win, 2, 2, "Latest Release:");)
        mvwprintw(win, 3, 2, release.messages[0].text);
        underline(win, mvwprintw(win, 5, 2, "Donwload::");)
        mvwprintw(win, 6, 2, release.messages[0].url);
    }
}

static void display_timeline(WINDOW *win)
{
    Content timeline = NOTIFICATIONS->timeline;
    uint16_t max_width = getmaxx(win);

    if (timeline.count && max_width > TIME_LINE_OFFSET) {
        underline(win, mvwprintw(win, 2, 0, "Twitter:");)

        uint16_t j = 3;
        const char *msg;
        size_t len;

        repeat(timeline.count,
                msg = timeline.messages[i].text;
                len = strlen(msg);

                mvwprintw(win, j + i, 0, "%s", msg);

                j += len >= max_width ? 2 : 1;
        );
    }
}

static void display_notifications(WINDOW *parent)
{
    if (!RELEASE_WINDOW && NULL != NOTIFICATIONS) {
        uint16_t height = getmaxy(parent) / 4;
        uint16_t y = getmaxy(parent) - height;
        uint16_t full_width = getmaxx(parent);
        uint16_t release_width = full_width / 3;
        uint16_t timeline_width = full_width - release_width - 2;

        RELEASE_WINDOW = ncurses_subwin(parent, height, release_width, y, 0);
        TIMELINE_WINDOW = ncurses_subwin(parent, height, timeline_width, y, release_width + 2);
    }

    if (RELEASE_WINDOW) {
        display_release(RELEASE_WINDOW);
        wrefresh(RELEASE_WINDOW);
    }
    if (TIMELINE_WINDOW) {
        display_timeline(TIMELINE_WINDOW);
        wrefresh(TIMELINE_WINDOW);
    }
}

MainMenuAction menu_main_display(GameState *state)
{
    load_notifications();
    ncurses_display_menu_windows();

    uint16_t x = (getmaxx(WINDOW_MAIN) - wcslen(LOGO[0])) / 2;
    uint16_t y = 5;

    styled(WINDOW_MAIN, COLOR_PAIR(COLOR_PURPLE_F),
            repeat(10,
                mvwaddwstr(WINDOW_MAIN, y++, x, LOGO[i]);
            )
    )
    wrefresh(WINDOW_MAIN);

    WINDOW *win = ncurses_newwin_adjust(size_new(10, 20), WINDOW_MAIN);
    Menu *menu = create_menu(state, win);
    nodelay(menu->window, true);

    int input;

    while ((input = wgetch(menu->window))) {
        switch (input) {
            case '\n':
            case KEY_ENTER:
                return get_action(menu);
            default:
                menu_process_defaults(menu, input);
        }
        display_notifications(WINDOW_MAIN);
        napms(LOOP_MAX_TIMEOUT);
    }
    clear_notifications();

    return MAIN_MENU_RESUME;
}

void menu_main_cleanup(void)
{
    if (notifications_cleanup) {
        notifications_cleanup();
    }
}
