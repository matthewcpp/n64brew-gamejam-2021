#pragma once

#include "gamestates.h"
#include "ui_navigation.h"

#include "framework64/util/bump_allocator.h"
#include "framework64/engine.h"

typedef enum MenuChoices {
    MENU_CHOICE_SINGLE_PLAYER,
    MENU_CHOICE_MULTIPLAYER,
    MENU_CHOICE_MULTI_CONTROLLED,
    MENU_CHOICE_MULTI_FIXED,
    MENU_CHOICE_MULTI_FIXED_FAST,
    MENU_CHOICE_MULTI_FIXED_MED,
    MENU_CHOICE_MULTI_FIXED_SLOW
} MenuChoices;

typedef struct {
    GameStateData* game_state;
    UiNavigation ui_navigation;
    fw64Engine* engine;
    fw64Camera camera;
    fw64Font* title_font;
    fw64Font* menu_font;
    fw64Texture* indicator_texture;
    int menu_selection ;
    IVec2 measurements[9];
    fw64BumpAllocator allocator;
} TitleScreen;

#ifdef __cplusplus
extern "C" {
#endif

void title_screen_init(TitleScreen* title_screen, fw64Engine* engine, GameStateData* game_state);
void title_screen_update(TitleScreen* title_screen);
void title_screen_draw(TitleScreen* title_screen);
void title_screen_uninit(TitleScreen* title_screen);

#ifdef __cplusplus
}
#endif