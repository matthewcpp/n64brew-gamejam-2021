#include "gamestates.h"

void game_state_data_init(GameStateData* game_state_data) {
    game_state_data->transition_state = GAME_STATE_NONE;
    game_state_data->transition_level = LEVEL_NONE;
}