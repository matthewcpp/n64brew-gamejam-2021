#pragma once


#include "key.h"
#include "trigger_box.h"

#include "framework64/scene.h"

typedef struct {
    Key key;
} Atrium;

#ifdef __cplusplus
extern "C" {
#endif

void tunnel_atrium_description(SceneDescription* desc);
void tunnel_atrium_init(fw64Scene* scene,void* level_arg, void* data_arg);
void tunnel_atrium_update(fw64Scene* scene,void* level_arg, void* data_arg));
void tunnel_atrium_draw(fw64Scene* scene,void* level_arg, void* data_arg));
void tunnel_atrium_uninit(fw64Scene* scene,void* level_arg, void* data_arg));
#ifdef __cplusplus
}
#endif