#ifndef ROGUECRAFT_CELL_H
#define ROGUECRAFT_CELL_H


#include <list.h>
#include "material.h"
#include "../color.h"
#include "../storage/storage.h"


#define CELL_NAME_MAX 20


#define cell_is_damageable(cell) \
    (SOLID == cell->type || CREATURE == cell->type)


typedef struct Item Item;


typedef enum {
    SOLID,
    HOLLOW,
    CREATURE,
    PLAYER,
    LIQUID,
    ITEM_ /** Conflicts with ncurses' ITEM */
} CellType;

typedef struct {
    wchar_t chr;
    bool in_registry;
    Style style;
    CellType type;
    double state;
    Material material;
    bool lighted;
    union {
        Item *item;
    };
} Cell;

typedef struct {
    char name[CELL_NAME_MAX];
    Cell cell;
} CellPrototype;


void cell_load(void);

CacheError cell_cache_load(List *prototypes);

void cell_cache_save(List *prototypes);

void cell_unload(void);

void cell_pool_init(void);

Cell *cell_allocate(void);

Cell *cell_with_item(Item *item);

Cell *cell_with_random_item(void);

const Cell *cell_get(char *name);

void cell_free_custom(Cell *cell);

Cell *cell_clone(const Cell *cell);

void cell_pool_cleanup(void);


#endif
