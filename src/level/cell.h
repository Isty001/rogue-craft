#ifndef ROGUECRAFT_CELL_H
#define ROGUECRAFT_CELL_H


#include "../color.h"


#define cell_registry_rand(level, type) \
    level->registry.type.cells[rand_in(0, level->registry.type.size)]


typedef enum {
    SOLID,
    HOLLOW,
    PLAYER,
    ITEM_ /** Conflicts with ncurses' ITEM */
} CellType;

typedef struct {
    wchar_t chr;
    bool in_registry;
    Style style;
    CellType type;
    uint16_t state;
    Material *material;
    void *data;
} Cell;

typedef struct {
    size_t size;
    Cell *cells;
} CellRegistry;


typedef struct {
    struct {
        uint16_t id;
        Color fore;
        Color back;
    } color;
    Material *material;
} CellPrototype;

typedef const struct {
    char chr;
    CellType type;
    size_t count;
    CellPrototype cells[];
} CellRegistryConfig;


void cell_pool_init(void);

Cell *cell_random_item(void);

CellRegistry cell_registry_new(CellRegistryConfig *cfg);

void cell_free_custom(Cell *cell);

Cell *cell_clone(Cell *cell);

void cell_pool_cleanup(void);


#endif
