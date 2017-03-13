#include "../../config/config.h"


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
}
