#include "atrium.h"

#include "assets.h"
#include "scene_atrium.h"

#include "tunnel_level.h"

void tunnel_atrium_description(SceneDescription* desc) {
    desc->index = FW64_ASSET_scene_atrium;
    desc->init_func = tunnel_atrium_init;
    desc->activated_func = tunnel_atrium_activate;
    desc->update_func = tunnel_atrium_update;
    desc->draw_func = tunnel_atrium_draw;
    desc->uninit_func = tunnel_atrium_uninit;
}

void tunnel_atrium_init(fw64Scene* scene, void* level_arg, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    SceneData* data = (SceneData*)data_arg;

    key_init(&data->atrium.key, fw64_scene_get_node(scene, FW64_scene_atrium_node_Key));
}

void tunnel_atrium_activate(fw64Scene* scene,void* level_arg, void* data_arg) {

}

void tunnel_atrium_update(fw64Scene* scene,void* level_arg, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    SceneData* data = (SceneData*)data_arg;

    key_update(&data->atrium.key, level->base.engine->time->time_delta);
}

void tunnel_atrium_draw(fw64Scene* scene,void* level_arg, void* data_arg) {

}

void tunnel_atrium_uninit(fw64Scene* scene,void* level_arg, void* data_arg) {

}
