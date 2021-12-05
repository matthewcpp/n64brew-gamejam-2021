#pragma once

#include "gamestates.h"

#include "ui_navigation.h"

#include "framework64/engine.h"
#include "framework64/util/bump_allocator.h"

typedef struct {
    fw64Engine* engine;
    GameStateData* game_state;
    fw64Camera camera;
    UiNavigation ui_navigation;
    fw64BumpAllocator allocator;
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