#ifndef ROGUECRAFT_DEFAULTS_H
#define ROGUECRAFT_DEFAULTS_H


#include "../src/color.h"
#include "../src/level/cell.h"
#include "../src/level/level.h"
#include "../src/player/player.h"
#include "../src/player/item.h"

/**
 * USER
 */
#define PLAYER_CHAR                 '@'
#define PLAYER_DEFAULT_EYESIGHT     4
#define PLAYER_DEFAULT_HP           100
#define PLAYER_DEFAULT_STAMINA      100
#define PLAYER_DEFAULT_HUNGER       0
#define PLAYER_MAX_HUNGER           100
#define PLAYER_DEFAULT_THIRST       0
#define PLAYER_MAX_THIRST           100


/**
 * KEYBINDINGS
 */
#define KEY_NORTH   'w'
#define KEY_EAST    'd'
#define KEY_SOUTH   's'
#define KEY_WEST    'a'


/**
 * LEVEL
 */
extern LevelConfig LEVEL_STONE_CAVE;


/**
 * CELL
 */
extern CellConfig CELL_WALL_STONE;

extern CellConfig CELL_HOLLOW_SIMPLE;

extern CellConfig CELL_WATER;


/**
 * SINGLE COLOR PAIRS
 */
#define COLOR_PAIR_NONE 1
#define COLOR_PAIR_RED_F 2

/**
 * COLOR SCHEMAS
 */
extern ColorSchema COLOR_WATER;

extern ColorSchema COLOR_STONE_WALL;



#endif
