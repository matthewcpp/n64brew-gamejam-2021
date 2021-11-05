#pragma once

#include "trigger_box.h"

#include "framework64/scene.h"

typedef struct {
    TriggerBox load_next_scene;
} Hallway;

#ifdef __cplusplus
extern "C" {
#endif

void tunnel_hallway_init(fw64Scene* scene, void* level_arg, void* data_arg);
void tunnel_hallway_update(void* level_arg, void* data_arg);
void tunnel_hallway_uninit(void* level_arg, void* data_arg);

#ifdef __cplusplus
}
#endif