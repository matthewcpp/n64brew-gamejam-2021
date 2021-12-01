#include "atrium.h"

#include "assets.h"
#include "scene_atrium.h"

#include "tunnel_level.h"

#include <string.h>

void tunnel_atrium_description(SceneDescription* desc) {
    memset(desc, 0, sizeof(SceneDescription));

    desc->index = FW64_ASSET_scene_atrium;
    desc->init_func = tunnel_atrium_init;
    desc->update_func = tunnel_atrium_update;
    desc->draw_func = tunnel_atrium_draw;
}

void tunnel_atrium_init(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    Atrium* atrium = (Atrium*)data_arg;

    fw64Node* girl_node = fw64_scene_get_node(scene, FW64_scene_atrium_node_Girl);

    girl_init(&atrium->girl, girl_node, &level->player, level->engine, fw64_scene_get_allocator(scene));
}


void tunnel_atrium_update(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    Atrium* atrium = (Atrium*)data_arg;

    girl_update(&atrium->girl);
}

void tunnel_atrium_draw(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    Atrium* atrium = (Atrium*)data_arg;

    girl_draw(&atrium->girl);
}