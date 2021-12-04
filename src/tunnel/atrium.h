#pragma once


#include "scene_manager.h"
#include "girl.h"
#include "trigger_box.h"
#include "dialogue_window.h"
#include "fade_effect.h"

#include "framework64/scene.h"

typedef enum {
    ATRIUM_STATE_PLAYING,
    ATRIUM_STATE_CUTSCENE
} AtriumState;

typedef struct {
    Girl girl;
    DialogueWindow dialog_window;
    AtriumState state;
} Atrium;

#ifdef __cplusplus
extern "C" {
#endif

void tunnel_atrium_description(SceneDescription* desc);
void tunnel_atrium_init(void* level_arg, fw64Scene* scene, void* data_arg);
void tunnel_atrium_uninit(void* level_arg, fw64Scene* scene, void* data_arg);
void tunnel_atrium_update(void* level_arg, fw64Scene* scene, void* data_arg);
void tunnel_atrium_draw(void* level_arg, fw64Scene* scene, void* data_arg);
void tunnel_atrium_ui_draw(void* level_arg, fw64Scene* scene, void* data_arg);

#ifdef __cplusplus
}
#endif