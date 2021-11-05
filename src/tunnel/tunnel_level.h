#pragma once

#include "level_base.h"
#include "enemy.h"

#include "hallway.h"
#include "atrium.h"

typedef union {
    Hallway hallway;
    Atrium atrium;
} SceneData;

typedef struct {
    LevelBase base;
    SceneData sceneData[2];
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

#ifdef __cplusplus
}
#endif
