#pragma once

#include "framework64/engine.h"

#include "tunnel/tunnel_level.h"

typedef union {
    TunnelLevel tunnel_level;
} Levels;

typedef enum {
    LEVEL_NONE,
    LEVEL_TUNNEL
} LevelId;

typedef struct {
    fw64Engine* engine;
    Levels levels;
    LevelId current_level;
} PlayingState;

#ifdef __cplusplus
extern "C" {
#endif

void playing_state_init(PlayingState* title_screen, fw64Engine* engine);
void playing_state_update(PlayingState* title_screen);
void playing_state_draw(PlayingState* title_screen);
void playing_state_uninit(PlayingState* title_screen);

#ifdef __cplusplus
}
#endif