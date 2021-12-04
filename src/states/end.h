#pragma once

#include "gamestates.h"

#include "framework64/engine.h"

typedef struct {
    fw64Engine* engine;
    GameStateData* game_state;
} EndScreen;

#ifdef __cplusplus
extern "C" {
#endif

void end_screen_init(EndScreen* end_screen, fw64Engine* engine, GameStateData* game_state);
void end_screen_update(EndScreen* end_screen);
void end_screen_draw(EndScreen* end_screen);
void end_screen_uninit(EndScreen* end_screen);

#ifdef __cplusplus
}
#endif