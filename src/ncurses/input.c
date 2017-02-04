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
            mouse_interact(player);
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
    switch (input){
        case KEY_NORTH:
        case KEY_SOUTH:
            inventory_select(player->inventory, direction_lookup(input));
            break;
        case '\n':
        case KEY_ENTER:
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
    switch (input) {
        case ERR:
            return;
        case 'q':
            if (0 < SELECTED) SELECTED--;
            break;
        case 'e':
            if (3 > SELECTED) SELECTED++;
            break;
        default:
            INPUT_PROCESSORS[SELECTED](input, player);
    }
}
