#include "lavapit.h"

#include "scene_lavapit.h"
#include "tunnel_level.h"

#include "assets.h"

#include <string.h>

void tunnel_lavapit_description(SceneDescription* desc) {
    memset(desc, 0, sizeof(SceneDescription));

    desc->index = FW64_ASSET_scene_lavapit;
    desc->init_func = tunnel_lavapit_init;
    desc->update_func = tunnel_lavapit_update;
}

void tunnel_lavapit_init(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    LavaPit* lava_pit = (LavaPit*)data_arg;


    const float speed = 15.0f;

    fw64Node* platform_node = fw64_scene_get_node(scene, FW64_scene_lavapit_node_Moving_Platform1);
    fw64Node* platform_target_node = fw64_scene_get_node(scene, FW64_scene_lavapit_node_Platform1_Target);
    moving_platform_init(&lava_pit->platforms[0], platform_node, platform_target_node, speed, &level->player);

    platform_node = fw64_scene_get_node(scene, FW64_scene_lavapit_node_Moving_Platform2);
    platform_target_node = fw64_scene_get_node(scene, FW64_scene_lavapit_node_Platform2_Target);
    moving_platform_init(&lava_pit->platforms[1], platform_node, platform_target_node, speed, &level->player);

    platform_node = fw64_scene_get_node(scene, FW64_scene_lavapit_node_Moving_Platform3);
    platform_target_node = fw64_scene_get_node(scene, FW64_scene_lavapit_node_Platform3_Target);
    moving_platform_init(&lava_pit->platforms[2], platform_node, platform_target_node, speed, &level->player);

    platform_node = fw64_scene_get_node(scene, FW64_scene_lavapit_node_Vertical_Moving_Platform_1);
    platform_target_node = fw64_scene_get_node(scene, FW64_scene_lavapit_node_Vertical_Platform_Target);
    moving_platform_init(&lava_pit->platforms[3], platform_node, platform_target_node, speed, &level->player);
}

void tunnel_lavapit_update(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    LavaPit* lava_pit = (LavaPit*)data_arg;

    for (int i = 0; i < LAVAPIT_PLATFORM_COUNT; i++) {
        moving_platform_update(&lava_pit->platforms[i], level->engine->time->time_delta);
    }

    if (level->player.node.transform.position.y < -30.0f) {
        tunnel_level_kill_player(level);
    }
}