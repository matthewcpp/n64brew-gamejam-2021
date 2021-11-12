#include "hallway.h"

#include "tunnel_level.h"
#include "scene_hallway.h"
#include "assets.h"

#include <string.h>

void tunnel_hallway_init(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    SceneData* data = (SceneData*)data_arg;

    fw64Node* trigger_node = fw64_scene_get_node(scene, FW64_scene_hallway_node_Load_Next_Scene);
    trigger_box_init(&data->hallway.load_next_scene, trigger_node, &level->player.collider);
}

void tunnel_hallway_update(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    SceneData* data = (SceneData*)data_arg;

    trigger_box_update(&data->hallway.load_next_scene);

    if (trigger_box_was_triggered(&data->hallway.load_next_scene)) {
        tunnel_level_load_next(level, FW64_ASSET_scene_hallway);
    }
}

void tunnel_hallway_description(SceneDescription* desc) {
    memset(desc, 0, sizeof(SceneDescription));

    desc->index = FW64_ASSET_scene_hallway;
    desc->init_func = tunnel_hallway_init;
    desc->update_func = tunnel_hallway_update;
}
