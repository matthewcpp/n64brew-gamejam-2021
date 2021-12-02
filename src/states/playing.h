#pragma once

#include "states/gamestates.h"

#include "framework64/engine.h"

#include "tunnel/tunnel_level.h"

typedef union {
    TunnelLevel tunnel_level;
} Levels;

typedef struct {
    GameStateData* game_state;
    fw64Engine* engine;
    Levels levels;
    LevelId current_level;
} PlayingState;

#ifdef __cplusplus
extern "C" {
#endif

void playing_state_init(PlayingState* playing, fw64Engine* engine, GameStateData* game_state);
void playing_state_update(PlayingState* playing);
void playing_state_draw(PlayingState* playing);
void playing_state_uninit(PlayingState* playing);

void playing_set_current_level(PlayingState* playing, LevelId level);

#ifdef __cplusplus
}
#endif