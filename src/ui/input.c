#include "player/player.h"
#include "panel.h"


void input_process(int input, Player *player)
{
    if (ERR == input) {
        return;
    }

    if (panel_is_open()) {
        event_dispatch_panel_input(input, player, panel_describe_top());
    } else {
        event_dispatch_input(input, player);
    }

    flushinp();
}
