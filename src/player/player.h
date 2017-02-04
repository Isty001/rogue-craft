#ifndef ROGUECRAFT_PLAYER_H
#define ROGUECRAFT_PLAYER_H


#include <stdint.h>
#include "../level/point.h"
#include "../level/level.h"


#define PLAYER_ATTR_NUM      4
#define PLAYER_ATTR_NAME_MAX 10


typedef struct Inventory Inventory;


typedef enum {
    HEALTH,
    STAMINA,
    HUNGER,
    THIRST
} AttributeType;

typedef struct {
    char name[PLAYER_ATTR_NAME_MAX];
    Color color;
    uint16_t limit;
    uint16_t current;
} Attribute;

typedef struct Player {
    Level *level;
    uint16_t eyesight;
    Camera *camera;
    struct {
        Point current;
        Point previous;
    } position;
    struct {
        Cell prototype;
        Cell *previous;
    } cell;
    struct {
        Attribute hp;
        Attribute stamina;
        Attribute hunger;
        Attribute thirst;
        Attribute *type_map[PLAYER_ATTR_NUM];
    } attr;
    Inventory *inventory;
} Player;


Player *player_new(Level *level, Camera *camera);

void player_free(Player *player);

void player_move(Player *player, Direction direction);

bool player_can_see(Player *player, uint16_t y, uint16_t x);

void player_attributes_display(Player *player);

void player_position_on_level(Player *player);


#endif
