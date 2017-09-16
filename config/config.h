#ifndef ROGUECRAFT_DEFAULTS_H
#define ROGUECRAFT_DEFAULTS_H


#include "util/color.h"
#include "level/cell/cell.h"
#include "level/level.h"
#include "player/player.h"
#include "item/item.h"
#include "util/randomization.h"
#include "util/event.h"


/**
 * CONFIGS
 */
#define RESOURCE_LEVELS "levels"
#define RESOURCE_ITEMS "items"
#define RESOURCE_CELLS "cells"

/**
 * PLAYER
 */
extern const PlayerStateConfig PLAYER_STATE_CONFIG;


/**
 * INVENTORY
 */
#define INVENTORY_SHORTCUT_NUM 5
#define INVENTORY_SELECTED_STYLE COLOR_PAIR(COLOR_RED_F)


/**
 * KEYBINDINGS
 */
#define KEY_NORTH       'w'
#define KEY_EAST        'd'
#define KEY_SOUTH       's'
#define KEY_WEST        'a'
#define KEY_ESCAPE       KEY_F(2)
#define KEY_PLAYER_INVENTORY   'i'
#define KEY_USE_SELECTED         'e'
#define KEY_INVENTORY_SHORTCUT_FIRST '1'
#define KEY_INVENTORY_SHORTCUTS \
    KEY_INVENTORY_SHORTCUT_FIRST, KEY_INVENTORY_SHORTCUT_FIRST + 1, KEY_INVENTORY_SHORTCUT_FIRST + 2, \
    KEY_INVENTORY_SHORTCUT_FIRST + 3, KEY_INVENTORY_SHORTCUT_FIRST + 4
#define KEY_DROP_ITEM        'q'

/**
 * MATERIAL
 */
extern int16_t MATERIAL_STRENGTH[MATERIAL_NUM];


/**
 * LEVEL
 */
#define LEVEL_ITEM_COUNT(level) \
    (uint16_t) ((level->size.height * level->size.width) / 200);


extern Probability LEVEL_PROBABILITY;


/**
 * ITEMS
 */
extern Probability ITEM_TYPE_PROBABILITY;

extern Probability ITEM_CONSUMABLE_PROBABILITY;

extern Probability ITEM_TOOL_PROBABILITY;

extern Probability ITEM_LIGHT_SOURCE_PROBABILITY;


#endif
