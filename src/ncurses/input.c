#include "../player/player.h"
#include "panel.h"


static void dispatch_input_event(int input, Player *player)
{
    InputEvent event = {
        .input = input,
        .player = player
    };
    event_dispatch(EVENT_INPUT, &event);
}

void input_process(int input, Player *player)
{
    if (ERR == input) {
        return;
    }

    if (!panel_is_open()) {
        dispatch_input_event(input, player);
    } else {
        panel_dispatch_input_event(input, player);
    }

    flushinp();
}
