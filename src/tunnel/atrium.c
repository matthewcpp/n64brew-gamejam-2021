#include "atrium.h"

#include "assets.h"
#include "scene_atrium.h"

#include "tunnel_level.h"

#include <string.h>

void tunnel_atrium_description(SceneDescription* desc) {
    memset(desc, 0, sizeof(SceneDescription));

    desc->index = FW64_ASSET_scene_atrium;
    desc->data_size = sizeof(Atrium);
    desc->init_func = tunnel_atrium_init;
    desc->update_func = tunnel_atrium_update;
    desc->draw_func = tunnel_atrium_draw;
    desc->ui_draw_func = tunnel_atrium_ui_draw;
    desc->uninit_func = tunnel_atrium_uninit;
}

void tunnel_atrium_init(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    Atrium* atrium = (Atrium*)data_arg;
    fw64Allocator* allocator = fw64_scene_get_allocator(scene);

    cutscene_init(&atrium->cutscene, level->engine, level, scene);

    fw64Node* girl_node = fw64_scene_get_node(scene, FW64_scene_atrium_node_Girl);
    girl_init(&atrium->girl, girl_node, &level->player, level->engine, fw64_scene_get_allocator(scene));
}

void tunnel_atrium_uninit(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    Atrium* atrium = (Atrium*)data_arg;
    fw64Allocator* allocator = fw64_scene_get_allocator(scene);

    //cutscene_uninit(&atrium->cutscene);
    girl_uninit(&atrium->girl);
}


void tunnel_atrium_update(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    Atrium* atrium = (Atrium*)data_arg;

    girl_update(&atrium->girl);

    return;

    if (cutscene_is_active(&atrium->cutscene)) {
        cutscene_update(&atrium->cutscene);
    }
    else {
        if (vec3_distance_squared(&level->player.node.transform.position, &atrium->girl.node->transform.position) <= 100.0f)
            cutscene_start(&atrium->cutscene); 

        if (atrium->cutscene.state == CUTSCENE_COMPLETE)
            level->state_data->transition_state = GAME_STATE_END;
    }
}

void tunnel_atrium_draw(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    Atrium* atrium = (Atrium*)data_arg;

    girl_draw(&atrium->girl);
}

void tunnel_atrium_ui_draw(void* level_arg, fw64Scene* scene, void* data_arg) {
    Atrium* atrium = (Atrium*)data_arg;

    //if (cutscene_is_active(&atrium->cutscene)) {
    //    cutscene_ui_draw(&atrium->cutscene);
    //}
}
