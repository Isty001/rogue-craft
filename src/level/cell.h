#ifndef ROGUECRAFT_CELL_H
#define ROGUECRAFT_CELL_H


#include "../color.h"


#define cell_registry_rand(level, type) \
    level->registry.type.cells[rand_in(0, level->registry.type.size)]


typedef enum {
    SOLID,
    HOLLOW,
    PLAYER,
    ITEM
} CellType;

typedef struct {
    char chr;
    bool is_prototype;
    Color color;
    CellType type;
    void *data;
} Cell;

typedef struct {
    size_t size;
    Cell *cells;
} CellRegistry;

typedef const struct {
    char chr;
    CellType type;
    bool has_color_schema;
    union {
        ColorSchema *schema;
        Color single;
    } color;
} CellConfig;


CellRegistry cell_registry_new(CellConfig cfg);

void cell_free_custom(Cell *cell);


#endif
