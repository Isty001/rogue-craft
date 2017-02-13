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

/**
 * The increase_max field indicates the number until the Attribute's value can increase,
 * but the modification_limit tells the bound of the modifications.
 * It's important if the Attribute is type of decreasing (ie. Hunger, Thirst).
 * For example the Player's Hunger can be 100 at max, but it can be decreased to 0.
 */
typedef struct {
    /**
     * This field marks if the Attribute's value is increased as the time passes
     * (ie. Hunger, Thirst)
     */
    bool increasing;
    char name[PLAYER_ATTR_NAME_MAX];
    Style style;
    uint16_t increase_max;
    uint16_t modification_limit;
    uint16_t current;
} Attribute;

typedef struct Player {
    Level *level;
    Camera *camera;
    Inventory *inventory;
    Attribute attributes[PLAYER_ATTR_NUM];
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


Player *player_new(Level *level, Camera *camera);

void player_free(Player *player);

void player_move(Player *player, Direction direction);

void player_calculate_sight(Player *player);

bool player_can_see(Player *player, Point point);

PlayerError player_hit(Player *player, Point at);

void player_attributes_display(Player *player);

void player_position_on_level(Player *player);


#endif
