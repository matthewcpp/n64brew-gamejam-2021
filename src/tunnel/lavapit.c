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
    fw64Allocator* allocator = fw64_scene_get_allocator(scene);


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

    fw64Image* lava_sprite = fw64_image_load_with_options(level->engine->assets, FW64_ASSET_image_lava_sprite, FW64_IMAGE_FLAG_DMA_MODE, allocator);
    lava_pit->lava_texture = fw64_texture_create_from_image(lava_sprite, allocator);
    fw64_texture_set_wrap_mode(lava_pit->lava_texture, FW64_TEXTURE_WRAP_REPEAT, FW64_TEXTURE_WRAP_REPEAT);
    fw64Node* walls = fw64_scene_get_node(scene, FW64_scene_lavapit_node_Walls);
    fw64Material* material = fw64_mesh_get_material_for_primitive(walls->mesh, 1);
    fw64_material_set_texture(material, lava_pit->lava_texture, 0);

    animated_material_texture_init(&lava_pit->animated_lava, material, 1.0f / 10.0f);
}

void tunnel_lavapit_update(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    LavaPit* lava_pit = (LavaPit*)data_arg;

    for (int i = 0; i < LAVAPIT_PLATFORM_COUNT; i++) {
        moving_platform_update(&lava_pit->platforms[i], level->engine->time->time_delta);
    }

    animated_material_texture_update(&lava_pit->animated_lava, level->engine->time->time_delta);

    // TODO: this should be more general
    if (level->player.node.transform.position.y < -30.0f && !tunnel_level_player_is_dying(level)) {
        tunnel_level_kill_player(level);
    }
}