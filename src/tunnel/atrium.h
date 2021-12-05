#pragma once


#include "scene_manager.h"
#include "girl.h"
#include "trigger_box.h"
#include "cutscene.h"
#include "fade_effect.h"

#include "framework64/scene.h"


typedef struct {
    Girl girl;
    Cutscene cutscene;
} Atrium;

#ifdef __cplusplus
extern "C" {
#endif


void tunnel_atrium_init(void* level_arg, fw64Scene* scene, void* data_arg);
void tunnel_atrium_uninit(void* level_arg, fw64Scene* scene, void* data_arg);
void tunnel_atrium_update(void* level_arg, fw64Scene* scene, void* data_arg);
void tunnel_atrium_draw(void* level_arg, fw64Scene* scene, void* data_arg);
void tunnel_atrium_ui_draw(void* level_arg, fw64Scene* scene, void* data_arg);

#ifdef __cplusplus
}
#endif