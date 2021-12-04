#pragma once

#include "scene_manager.h"
#include "trigger_box.h"

#include "framework64/scene.h"

typedef struct {
    TriggerBox load_next_scene;
} Hallway;

#ifdef __cplusplus
extern "C" {
#endif

void tunnel_hallway_init(void* level_arg, fw64Scene* scene, void* data_arg);
void tunnel_hallway_update(void* level_arg, fw64Scene* scene, void* data_arg);

#ifdef __cplusplus
}
#endif