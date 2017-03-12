#include "../../config/config.h"
#include "../player/inventory.h"


static int KEY_MAIN_DIRECTION_MAP[4][2] = {
    {KEY_NORTH, NORTH},
    {KEY_EAST,  EAST},
    {KEY_SOUTH, SOUTH},
    {KEY_WEST,  WEST},
};


static inline Direction direction_lookup(int input)
{
    for (int i = 0; i < 4; ++i) {
        if (input == KEY_MAIN_DIRECTION_MAP[i][0]) {
            return (Direction) KEY_MAIN_DIRECTION_MAP[i][1];
        }
    }
    return -1;
}

static void process_main(int input, Player *player)
{
    switch (input) {
        case KEY_MOUSE:
            break;
        default: {
            Direction direction;

            if ((direction = direction_lookup(input))) {
                player_move(player, direction);
            }
        }
    }
}

static void process_inventory(int input, Player *player)
{
    switch (input) {
        case KEY_USE:
            inventory_use_selected(player);
            break;
        default:
            return;
    }
}

static void (*INPUT_PROCESSORS[3])(int, Player *) = {
    process_inventory, process_main
};

static uint16_t SELECTED = 1;


void input_process(int input, Player *player)
{
    if (ERR == input) {
        return;
    }

    InputEvent event = {
        .input = input,
        .player = player
    };

    event_dispatch(EVENT_INPUT, &event);

    switch (input) {
        default:
            INPUT_PROCESSORS[SELECTED](input, player);
    }
}
