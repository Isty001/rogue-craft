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
 * MATERIAL
 */
extern int16_t MATERIAL_STRENGTH[MATERIAL_NUM];


/**
 * LEVEL
 */
#define LEVEL_ITEM_COUNT(level) \
    (uint16_t) ((level->size.height * level->size.width) / 200);


extern Probability LEVEL_PROBABILITY;


#endif
