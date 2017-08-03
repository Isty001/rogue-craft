#ifndef ROGUECRAFT_FIXTURE_H
#define ROGUECRAFT_FIXTURE_H


#include <stdbool.h>


#define FIXTURE_SOLID_STYLE 900
#define FIXTURE_HOLLOW_STYLE 1900


typedef struct Level Level;

typedef struct Item Item;


/**
 * Level cells will look like this:
 *
 * S H S
 * H H H
 * H H H
 * S S S
 */
Level *fixture_level(void);

void fixture_level_free(Level *level);

Item fixture_consumable(bool permanent);


#endif
