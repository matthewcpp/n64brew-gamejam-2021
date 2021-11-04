#pragma once

#include "framework64/engine.h"
#include "framework64/scene.h"
#include "player.h"
#include "chase_camera.h"
#include "ui.h"

typedef struct {
    fw64Engine* engine;

    fw64Scene* scene;
    UI ui;

    Player player;
    ChaseCamera chase_cam;
} LevelBase;


#ifdef __cplusplus
extern "C" {
#endif

void level_base_init(LevelBase* level, fw64Engine* engine, uint32_t level_idex);
void level_base_update(LevelBase* level);
void level_base_draw(LevelBase* level);
void level_base_setup_player(LevelBase* level, uint32_t start_node_index);

#ifdef __cplusplus
}
#endif
