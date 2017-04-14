#include "../player/player.h"


void input_process(int input, Player *player)
{
    if (ERR == input) {
        return;
    }

    InputEvent event_data = {
        .input = input,
        .player = player
    };

    Event event = !panel_is_open() ? EVENT_INPUT : EVENT_PANEL_INPUT;

    event_dispatch(event, &event_data);
    flushinp();
}
