#pragma once

#include "player.h"
#include "chase_camera.h"

#include "flame.h"

#include "framework64/engine.h"
#include "framework64/scene.h"

typedef struct {
    Player player;
    ChaseCamera chase_camera;
    fw64Scene* scene;
    fw64Engine* engine;
    Flame flames[2];
} Level1;

#ifdef __cplusplus
extern "C" {
#endif

void level1_init(Level1* level1, fw64Engine* engine);
void level1_update(Level1* level1);
void level1_draw(Level1* level1);

#ifdef __cplusplus
}
#endif