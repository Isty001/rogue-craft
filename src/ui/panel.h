#ifndef ROGUE_CRAFT_PANEL_H
#define ROGUE_CRAFT_PANEL_H


#include <panel.h>
#include <cursed_tools/cursed_tools.h>


typedef struct Inventory Inventory;

typedef struct Player Player;

#define PANEL_TYPE_NUM 2 + 1

/**
 * Must start from 1, beacuse of PanelInputListener in event.c
 * If there are less supported types than MAX_SUPPORTED_TYPE, the rest of the array
 * would 0s, and match the fisrt enum value.
 */
typedef enum {
    PANEL_INVENTORY = 1,
    PANEL_CRAFT = 2
} PanelType;

typedef struct {
    PanelType type;
    union {
        Inventory *inventory;
        CTabList *tabs;
    };
} PanelInfo;

typedef struct {
    int input;
    Player *player;
    const PanelInfo *info;
} PanelInputEvent;


void panel_init(void);

PANEL *panel_push_new(WINDOW *win, const PanelInfo info);

bool panel_is_open(void);

void panel_hide(void);

void panel_show(void);

const PanelInfo *panel_describe_top(void);

void panel_close_top(int input, Player *player);

void panel_cleanup(void);


#endif
