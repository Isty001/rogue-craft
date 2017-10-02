#ifndef ROGUECRAFT_CELL_H
#define ROGUECRAFT_CELL_H


#include <list.h>
#include "util/color.h"
#include "level/material.h"
#include "storage/storage.h"


#define CELL_NAME_MAX 20



typedef struct Item Item;

typedef struct Player Player;


typedef enum {
    CELL_SOLID,
    CELL_HOLLOW,
    CELL_CREATURE,
    CELL_PLAYER,
    CELL_LIQUID,
    CELL_ITEM
} CellType;

typedef struct Cell {
    wchar_t chr;
    bool in_registry;
    Style style;
    CellType type;
    int16_t state;
    Material material;
    union {
        Item *item;
    };
} Cell;

typedef struct {
    char name[CELL_NAME_MAX];
    Cell cell;
} CellPrototype;


void cell_registry_load(void);

CacheError cell_cache_load(List *prototypes);

void cell_cache_save(List *prototypes);

void cell_registry_unload(void);

void cell_pool_init(void);

Cell *cell_from_item(Item *item);

Cell *cell_with_random_item(void);

const Cell *cell_registry_get(char *name);

void cell_free(Cell *cell);

Cell *cell_clone(const Cell *cell);

void cell_pool_cleanup(void);


#endif
