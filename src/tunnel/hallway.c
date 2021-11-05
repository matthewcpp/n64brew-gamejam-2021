#include "hallway.h"

#include "tunnel_level.h"
#include "scene_hallway.h"
#include "assets.h"

void tunnel_hallway_init(fw64Scene* scene, void* level_arg, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    SceneData* data = (SceneData*)data_arg;

    fw64Node* trigger_node = fw64_scene_get_node(scene, FW64_scene_hallway_node_Load_Next_Scene);
    trigger_box_init(&data->hallway.load_next_scene, trigger_node, &level->base.player.collider);
}

void tunnel_hallway_update(void* level_arg, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    SceneData* data = (SceneData*)data_arg;

    trigger_box_update(&data->hallway.load_next_scene);

    if (trigger_box_was_triggered(&data->hallway.load_next_scene)) {
        tunnel_level_load_next(level, FW64_ASSET_scene_hallway);
    }
}

void tunnel_hallway_uninit(void* level_arg, void* data_arg) {

}