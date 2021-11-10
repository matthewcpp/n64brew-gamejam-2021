#include "atrium.h"

#include "assets.h"
#include "scene_atrium.h"

#include "tunnel_level.h"

void tunnel_atrium_description(SceneDescription* desc) {
    desc->index = FW64_ASSET_scene_atrium;
    desc->init_func = tunnel_atrium_init;
    desc->update_func = tunnel_atrium_update;
    desc->draw_func = NULL;
    desc->uninit_func = NULL;
}

void tunnel_atrium_init(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    SceneData* data = (SceneData*)data_arg;

    key_init(&data->atrium.key, fw64_scene_get_node(scene, FW64_scene_atrium_node_Key));
}


void tunnel_atrium_update(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    SceneData* data = (SceneData*)data_arg;

    key_update(&data->atrium.key, level->engine->time->time_delta);
}