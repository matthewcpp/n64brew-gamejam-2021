#pragma once 

#include "framework64/animation_data.h"
#include "framework64/animation_controller.h"
#include "framework64/engine.h"
#include "player_palette.h"

typedef struct {
    fw64Engine* engine;
    fw64Mesh* mesh;
    fw64AnimationData* animation_data;
    fw64AnimationController animation_controller;
    fw64Transform transforms[2];
    PlayerPalette palette;
} TitleAnimation;

#ifdef __cplusplus
extern "C" {
#endif

void title_animation_init(TitleAnimation* animation, fw64Engine* engine, int inital_animation, fw64Allocator* allocator);
void title_animation_uninit(TitleAnimation* animation, fw64Allocator* allocator);

void title_animation_update(TitleAnimation* animation);

#ifdef __cplusplus
}
#endif