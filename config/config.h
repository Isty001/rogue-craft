#ifndef ROGUECRAFT_DEFAULTS_H
#define ROGUECRAFT_DEFAULTS_H


#include "../src/color.h"
#include "../src/level/cell.h"
#include "../src/level/level.h"


/**
 * USER
 */
#define PLAYER_CHAR '@'
#define PLAYER_DEFAULT_EYESIGHT 4


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
extern ColorSchema COLOR_EMPTY;

extern CellConfig CELL_WALL_STONE;

extern CellConfig CELL_HOLLOW_SIMPLE;

extern CellConfig CELL_WATER;


/**
 * SINGLE COLOR PAIRS
 */
#define COLOR_PAIR_NONE 1
#define COLOR_PAIR_RED_CELL 2

/**
 * COLOR SCHEMAS
 */
extern ColorSchema COLOR_WATER;

extern ColorSchema COLOR_STONE_WALL;



#endif
