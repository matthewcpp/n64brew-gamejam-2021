#include "hallway.h"

#include "tunnel_level.h"
#include "scene_hallway.h"
#include "assets.h"

void tunnel_hallway_init(fw64Scene* scene, void* level_arg, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    SceneData* data = (SceneData*)data_arg;

    fw64Node* trigger_node = fw64_scene_get_node(scene, FW64_scene_hallway_node_Load_Next_Scene);
    trigger_box_init(&data->hallway.load_next_scene, trigger_node, &level->player.collider);
}

void tunnel_hallway_activate(fw64Scene* scene, void* level_arg, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    SceneData* data = (SceneData*)data_arg;

    // Player just spawned
    if (level->player.scene == NULL) {
        fw64Node *node = fw64_scene_get_node(scene, FW64_scene_hallway_node_Player_Start);
        level->player.node.transform.position = node->transform.position;
    }
}

void tunnel_hallway_update(void* level_arg, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    SceneData* data = (SceneData*)data_arg;

    trigger_box_update(&data->hallway.load_next_scene);

    if (trigger_box_was_triggered(&data->hallway.load_next_scene)) {
        tunnel_level_load_next(level, FW64_ASSET_scene_hallway);
    }
}

void tunnel_hallway_draw(void* level_arg, void* data_arg) {

}

void tunnel_hallway_uninit(void* level_arg, void* data_arg) {

}

void tunnel_hallway_description(SceneDescription* desc) {
    desc->index = FW64_ASSET_scene_hallway;
    desc->init_func = tunnel_hallway_init;
    desc->activated_func tunnel_hallway_activate;
    desc->update_func = tunnel_level_update;
    desc->draw_func = tunnel_hallway_draw;
    desc->uninit_func = tunnel_hallway_uninit;
}
