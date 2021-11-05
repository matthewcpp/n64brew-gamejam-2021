#include "atrium.h"
#include "scene_atrium.h"

#include "tunnel_level.h"

void tunnel_atrium_init(fw64Scene* scene, void* level_arg, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    SceneData* data = (SceneData*)data_arg;

    key_init(&data->atrium.key, fw64_scene_get_node(scene, FW64_scene_atrium_node_Key));
}

void tunnel_atrium_update(void* level_arg, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    SceneData* data = (SceneData*)data_arg;

    key_update(&data->atrium.key, level->base.engine->time->time_delta);
}

void tunnel_atrium_uninit(void* level_arg, void* data_arg) {

}
