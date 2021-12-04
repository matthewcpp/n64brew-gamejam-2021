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
    desc->ui_draw_func = tunnel_atrium_ui_draw;
    desc->uninit_func = tunnel_atrium_uninit;
}

void tunnel_atrium_init(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    Atrium* atrium = (Atrium*)data_arg;
    fw64Allocator* allocator = fw64_scene_get_allocator(scene);

    atrium->state = ATRIUM_STATE_PLAYING;

    fw64Node* girl_node = fw64_scene_get_node(scene, FW64_scene_atrium_node_Girl);

    girl_init(&atrium->girl, girl_node, &level->player, level->engine, fw64_scene_get_allocator(scene));

    IVec2 pos = {20, 181};
    IVec2 size = {280, 40};
    dialogue_window_init(&atrium->dialog_window, level->engine, FW64_ASSET_font_Dialogue, FW64_ASSET_raw_dialogue, &pos, &size, allocator);
}

void tunnel_atrium_uninit(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    Atrium* atrium = (Atrium*)data_arg;
    fw64Allocator* allocator = fw64_scene_get_allocator(scene);

    dialogue_window_uninit(&atrium->dialog_window, allocator);
    girl_uninit(&atrium->girl);
}

void atrium_start_cutscene(FadeDirection direction, void* arg) {
    Atrium* atrium = (Atrium*)arg;

    atrium->state = ATRIUM_STATE_CUTSCENE;
    dialogue_window_start(&atrium->dialog_window);
}

void tunnel_atrium_update(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    Atrium* atrium = (Atrium*)data_arg;

    girl_update(&atrium->girl);

    if (atrium->state == ATRIUM_STATE_PLAYING  & !fade_effect_is_active(&level->fade_effect)) {
        float dist = vec3_distance_squared(&level->player.node.transform.position, &atrium->girl.node->transform.position);

        if (dist < 100.0f) {
            fw64ColorRGBA8 black = {0, 0, 0, 255};
            level->fade_effect.color = black;
            fade_effect_set_callback(&level->fade_effect, atrium_start_cutscene, atrium);
            fade_effect_start(&level->fade_effect, FADE_IN, 2.0f);
            level->player.process_input = 0;
        }
    }
    else if (atrium->state == ATRIUM_STATE_CUTSCENE) {
        dialogue_window_update(&atrium->dialog_window);
    }
}

void tunnel_atrium_draw(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    Atrium* atrium = (Atrium*)data_arg;

    girl_draw(&atrium->girl);
}

void tunnel_atrium_ui_draw(void* level_arg, fw64Scene* scene, void* data_arg) {
    Atrium* atrium = (Atrium*)data_arg;

    if (atrium->state == ATRIUM_STATE_CUTSCENE) {
        dialogue_window_draw(&atrium->dialog_window);
    }
}
