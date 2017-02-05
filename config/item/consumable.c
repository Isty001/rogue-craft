#include "../config.h"


#define CHAR_POTION     L'ð'
#define CHAR_FOOD       'f'
#define CHAR_WATER      'w'


#define CONSUMABLE_PROTOTYPE(var_name, nam, stl, c_permanent, c, c_type, ...)  \
    static ItemPrototype var_name = {                                       \
        .randomize = item_randomize_value,                                  \
        .value_range = __VA_ARGS__,                                         \
        .item = {                                                           \
            .name = nam,                                                    \
            .chr = c,                                                       \
            .style = stl,                                                   \
            .type = CONSUMABLE,                                             \
            .consumable = {                                                 \
                .permanent = c_permanent,                                   \
                .type = c_type                                              \
            }                                                               \
        }                                                                   \
};

CONSUMABLE_PROTOTYPE(
    ITEM_CONSUMABLE_HP,
    "Health Potion",
    COLOR_PAIR(COLOR_HEALTH),
    false,
    CHAR_POTION,
    HEALTH,
    {10, 50}
);

CONSUMABLE_PROTOTYPE(
    ITEM_CONSUMABLE_MAX_HP,
    "Max Health Potion",
    COLOR_PAIR(COLOR_HEALTH) | A_BOLD | A_UNDERLINE,
    true,
    CHAR_POTION,
    HEALTH,
    {1, 6}
)

CONSUMABLE_PROTOTYPE(
    ITEM_CONSUMABLE_MAX_STAMINA,
    "Stamina Potion",
    COLOR_PAIR(COLOR_STAMINA) | A_BOLD | A_UNDERLINE,
    true,
    CHAR_POTION,
    STAMINA,
    {1, 6}
)

CONSUMABLE_PROTOTYPE(
    ITEM_CONSUMABLE_FOOD,
    "Om nom nom",
    COLOR_PAIR(COLOR_FOOD) | A_BOLD,
    false,
    CHAR_FOOD,
    HUNGER,
    {-20, -5}
)

CONSUMABLE_PROTOTYPE(
    ITEM_CONSUMABLE_WATER,
    "Drinking water",
    COLOR_PAIR(COLOR_WATER) | A_BOLD,
    false,
    CHAR_WATER,
    THIRST,
    {-20, -5}
)

Probability ITEM_CONSUMABLE_PROBABILITY = {
    .count = 5,
    .sum = 68,
    .items = {
        {.chance = 30, .value = &ITEM_CONSUMABLE_WATER},
        {.chance = 20, .value = &ITEM_CONSUMABLE_FOOD},
        {.chance = 10, .value = &ITEM_CONSUMABLE_HP},
        {.chance = 4, .value = &ITEM_CONSUMABLE_MAX_HP},
        {.chance = 4, .value = &ITEM_CONSUMABLE_MAX_STAMINA},
    }
};
