#pragma once

#include "scene_manager.h"

#include "moving_platform.h"

#define LAVAPIT_PLATFORM_COUNT 4

typedef struct {
    MovingPlatform platforms[LAVAPIT_PLATFORM_COUNT];
} LavaPit;

#ifdef __cplusplus
extern "C" {
#endif

void tunnel_lavapit_description(SceneDescription* desc);
void tunnel_lavapit_init(void* level_arg, fw64Scene* scene, void* data_arg);
void tunnel_lavapit_update(void* level_arg, fw64Scene* scene, void* data_arg);

#ifdef __cplusplus
}
#endif