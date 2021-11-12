#pragma once

#include "scene_manager.h"
#include "enemy.h"

#include "hallway.h"
#include "atrium.h"
#include "lavapit.h"

typedef union {
    Hallway hallway;
    LavaPit lava_pit;
    Atrium atrium;
} SceneData;

typedef struct {
    fw64Engine* engine;
    Player player;
    UI ui;
    ChaseCamera chase_cam;
    SceneManager scene_manager;
    int debug;
} TunnelLevel;

#ifdef __cplusplus
extern "C" {
#endif

void tunnel_level_init(TunnelLevel* level, fw64Engine* engine);
void tunnel_level_uninit(TunnelLevel* level);
void tunnel_level_update(TunnelLevel* level);
void tunnel_level_draw(TunnelLevel* level);

void tunnel_level_load_next(TunnelLevel* level, uint32_t current_index);
void tunnel_level_kill_player(TunnelLevel* level);
#ifdef __cplusplus
}
#endif
