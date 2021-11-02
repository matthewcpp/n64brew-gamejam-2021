#pragma once

#include "level_base.h"
#include "enemy.h"

typedef struct {
    LevelBase base;

    fw64Mesh* boo_mesh;
    fw64Mesh* pumpkin_mesh;
    Enemy enemy[3];

    int triggered;
    int debug;

} TunnelLevel;

#ifdef __cplusplus
extern "C" {
#endif

void tunnel_level_init(TunnelLevel* level, fw64Engine* engine);
void tunnel_level_uninit(TunnelLevel* level);
void tunnel_level_update(TunnelLevel* level);
void tunnel_level_draw(TunnelLevel* level);

#ifdef __cplusplus
}
#endif
