#pragma once

#include "framework64/engine.h"

typedef struct {
    fw64Engine* engine;
    fw64Camera camera;
    fw64Font* title_font;
    fw64Font* menu_font;
    fw64Image* indicators;
    fw64Texture* indicator_texture;
    int menu_selection ;
    IVec2 measurements[4];
} TitleScreen;

#ifdef __cplusplus
extern "C" {
#endif

void title_screen_init(TitleScreen* title_screen, fw64Engine* engine);
void title_screen_update(TitleScreen* title_screen);
void title_screen_draw(TitleScreen* title_screen);
void title_screen_uninit(TitleScreen* title_screen);

#ifdef __cplusplus
}
#endif