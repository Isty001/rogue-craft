#ifndef ROGUECRAFT_PLAYER_H
#define ROGUECRAFT_PLAYER_H


#include <stdint.h>
#include <memory.h>
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

typedef enum {
    PE_OUT_OF_RANGE,
    PE_INVALID_CELL,
    PE_DEALT_DAMAGE
} PlayerError;

typedef struct {
    char name[PLAYER_ATTR_NAME_MAX];
    Style style;
    uint16_t max;
    uint16_t current;
} Attribute;

typedef struct {
    double dealt_damage;
    uint32_t traveled;
    time_t updated;
} Modifiers;

typedef struct {
    pthread_mutex_t mutex;
    Modifiers modifiers;
    Attribute state[PLAYER_ATTR_NUM];
} PlayerAttributes;

typedef struct Player {
    Level *level;
    Camera *camera;
    Inventory *inventory;
    PlayerAttributes attributes;
    struct {
        uint16_t radius;
        uint16_t visible_count;
        Point *visible;
    } sight;
    struct {
        Point current;
        Point previous;
    } position;
    struct {
        Cell prototype;
        Cell *previous;
    } cell;
} Player;

typedef const struct {
    uint16_t limit;
    Range hunger;
    Range thirst;
    Range stamina;
} FatigueChanges;

typedef const struct {
    uint16_t hunger_limit;
    uint16_t thirst_limit;
    Range health;
    Range stamina;
} FatigueDamage;

typedef const struct {
    FatigueChanges dealt_damage;
    FatigueChanges time;
    FatigueChanges traveled;
    FatigueDamage damage;
} AttributeConfig;


Player *player_new(Level *level, Camera *camera);

void player_free(Player *player);

void player_move(Player *player, Direction direction);

void player_calculate_sight(Player *player);

bool player_can_see(Player *player, Point point);

PlayerError player_hit(Player *player, Point at);

void player_attributes_display(Player *player);

void player_position_on_level(Player *player);


#endif
