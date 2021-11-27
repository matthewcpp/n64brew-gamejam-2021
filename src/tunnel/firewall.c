#include "firewall.h"

#include "tunnel_level.h"

#include "assets.h"
#include "scene_firewall.h"

#include <string.h>

void tunnel_firewall_description(SceneDescription* desc) {
    memset(desc, 0, sizeof(SceneDescription));

    desc->index = FW64_ASSET_scene_firewall;
    desc->init_func = tunnel_firewall_init;
    desc->update_func = tunnel_firewall_update;
    desc->draw_func = tunnel_firewall_draw;
}

void tunnel_firewall_init(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* tunnel_level = (TunnelLevel*)level_arg;
    Firewall* firewall = (Firewall*)data_arg;
    fw64Allocator* allocator = fw64_scene_get_allocator(scene);

    firewall->flame_image = fw64_image_load_with_options(tunnel_level->engine->assets, FW64_ASSET_image_fire_sprite, FW64_IMAGE_FLAG_DMA_MODE, allocator);
    
    fw64TexturedQuadParams params;
    fw64_textured_quad_params_init(&params);
    params.image = firewall->flame_image;
    params.is_animated = 1;
    params.min_s = 0.265f;
    params.max_s = 0.710f;

    firewall->flame_mesh = fw64_textured_quad_create_with_params(tunnel_level->engine, &params, allocator);

    fw64Material* material = fw64_mesh_get_material_for_primitive(firewall->flame_mesh, 0);
    animated_material_texture_init(&firewall->animated_material, material, 1.0f / 10.0f);

    fw64Node* roll_node = fw64_scene_get_node(scene, FW64_scene_firewall_node_Roll1);
    fire_rolling_obstacle_init(&firewall->roll_obstacles[0], roll_node, &tunnel_level->player, firewall->flame_mesh);

    fw64Node* jump_node = fw64_scene_get_node(scene, FW64_scene_firewall_node_Jump1);
    fire_jumping_obstacle_init(&firewall->jump_obstacles[0], jump_node, &tunnel_level->player, firewall->flame_mesh);
}

void tunnel_firewall_update(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* tunnel_level = (TunnelLevel*)level_arg;
    Firewall* firewall = (Firewall*)data_arg;

    animated_material_texture_update(&firewall->animated_material, tunnel_level->engine->time->time_delta);

    for (int i = 0; i < ROLL_OBSTACLE_COUNT; i++) {
        if (fire_rolling_obstacle_update(&firewall->roll_obstacles[i], tunnel_level->engine->time->time_delta)) {
            tunnel_level_kill_player(tunnel_level);
            return;
        }
    }

        for (int i = 0; i < JUMP_OBSTACLE_COUNT; i++) {
        if (fire_jumping_obstacle_update(&firewall->jump_obstacles[i], tunnel_level->engine->time->time_delta)) {
            tunnel_level_kill_player(tunnel_level);
            return;
        }
    }
}

void tunnel_firewall_draw(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* tunnel_level = (TunnelLevel*)level_arg;
    Firewall* firewall = (Firewall*)data_arg;

    draw_static_fire_obstacles(tunnel_level->engine->renderer, &firewall->roll_obstacles[0], ROLL_OBSTACLE_COUNT);
    draw_static_fire_obstacles(tunnel_level->engine->renderer, &firewall->jump_obstacles[0], JUMP_OBSTACLE_COUNT);
}