#include <notifier.h>
#include <stdbool.h>
#include <dlfcn.h>
#include "memory/memory.h"
#include "main_menu.h"
#include "util/environment.h"
#include "util/logger.h"


static bool LOADED = false;
static void *HANDLE = NULL;

static void (*notifications_init)(void *(*allocator)(int), void*(reallocator)(void *, int), void (*deallocator)(void *));
static void (*notifications_load)(NotifierCallback callback, void *arg);
static void (*notifications_cleanup)(void);


static void load(void)
{
    notifications_init = dlsym(HANDLE, "notifications_init");
    notifications_load = dlsym(HANDLE, "notifications_load");
    notifications_cleanup = dlsym(HANDLE, "notifications_cleanup");

    notifications_init(mem_alloc, mem_realloc, mem_dealloc);
}

static void display(const Notifications notifications, WINDOW *win)
{
    wprintw(win, "%p -- %p", win,  WINDOW_MAIN);
    wrefresh(WINDOW_MAIN);
}

void menu_main_add_notifications(WINDOW *window)
{
    const char *path = getenv(ENV_PATH_LIB_NOTIFIER) ?: "/usr/local/lib/librogue-craft-notifier.so";

    if (!LOADED) {
        if ((HANDLE = dlopen(path, RTLD_NOW))) {
            load();
        } else {
            log_alert("Unable to load notifier lib at [%s] : [%s]", path, dlerror());
        }
        LOADED = true;
    }

    if (HANDLE) {
        notifications_load((NotifierCallback) display, WINDOW_MAIN);
    }
}

