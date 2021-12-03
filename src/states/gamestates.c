#include "gamestates.h"

void game_state_data_init(GameStateData* game_state_data) {
    game_state_data->transition_state = GAME_STATE_NONE;
    game_state_data->transition_level = LEVEL_NONE;
    game_state_data->settings.control_mode = CONTROL_MODE_SINGLE;
    game_state_data->settings.control_transfer_time = CONTROL_TRANSFER_TIME_MED;
}