#pragma once

#include "game_settings.h"

typedef enum {
    GAME_STATE_NONE,
    GAME_STATE_TITLE,
    GAME_STATE_PLAYING
} GameState;

typedef enum {
    LEVEL_NONE,
    LEVEL_TUNNEL
} LevelId;

typedef struct {
    /** set this to the game state you want to transition to on the next update frame */
    GameState transition_state;

    /** set this to the level you want to load on the next update frame */
    LevelId transition_level;

    GameSettings settings;
} GameStateData;

#ifdef __cplusplus
extern "C" {
#endif

/** use this method to set inital game wide data */
void game_state_data_init(GameStateData* game_state_data);

#ifdef __cplusplus
}
#endif