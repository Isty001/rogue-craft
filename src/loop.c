#include <ncurses.h>
#include <ui/panel.h>
#include <player/inventory/inventory.h>


#define MAX_TIMEOUT 85


static void render(Player *player)
{
    panel_hide();
    level_display(player);
    inventory_shortcut_display(player->inventory);
    player_state_display(player);
    profiler_display();
    panel_show();
}

static void update(int input, Player *player)
{
    input_process(input, player);

    camera_update(&player->camera, player->position.current, player->level, WINDOW_MAIN);
    player_sight_update(player);

    render(player);
}

void loop_run(Player *player)
{
    int input;
    struct timespec start;
    struct timespec end;

    TimerArgs args = {.ptr = {player, &PLAYER_STATE_CONFIG}};
    timer_new(1000, player_state_update, args);

    while (1) {
        clock_gettime(CLOCK_MONOTONIC, &start);

        if (-1 != (input = wgetch(WINDOW_MAIN))) {
            if (KEY_ESCAPE == input) {
                if (panel_is_open()) {
                    panel_close_top(input, player);
                } else {
                    break;
                }
            }
        }

        update(input, player);
        render(player);

        clock_gettime(CLOCK_MONOTONIC, &end);

        timer_tick(end);
        napms(max(0, MAX_TIMEOUT - time_diff_ms(end, start)));
    }
}
