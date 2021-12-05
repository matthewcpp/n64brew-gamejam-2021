#include "firewall.h"

#include "tunnel_level.h"

#include "assets.h"
#include "scene_firewall.h"

#include <string.h>

void tunnel_firewall_description(SceneDescription* desc) {
    memset(desc, 0, sizeof(SceneDescription));

    desc->index = FW64_ASSET_scene_firewall;
    desc->data_size = sizeof(Firewall);
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

    fw64Node* reference_node = fw64_scene_get_node(scene, FW64_scene_firewall_node_Roll1);
    static_fire_obstacle_init(&firewall->static_obstacles[0], STATIC_FIRE_OBSTACLE_ROLL, reference_node, &tunnel_level->player, firewall->flame_mesh);

    reference_node = fw64_scene_get_node(scene, FW64_scene_firewall_node_Jump1);
    static_fire_obstacle_init(&firewall->static_obstacles[1], STATIC_FIRE_OBSTACLE_JUMP, reference_node, &tunnel_level->player, firewall->flame_mesh);

    reference_node = fw64_scene_get_node(scene, FW64_scene_firewall_node_Dash1);
    static_fire_obstacle_init(&firewall->static_obstacles[2], STATIC_FIRE_OBSTACLE_JUMP, reference_node, &tunnel_level->player, firewall->flame_mesh);
    
    reference_node = fw64_scene_get_node(scene, FW64_scene_firewall_node_Dash2);
    static_fire_obstacle_init(&firewall->static_obstacles[3], STATIC_FIRE_OBSTACLE_JUMP, reference_node, &tunnel_level->player, firewall->flame_mesh);
    
    reference_node = fw64_scene_get_node(scene, FW64_scene_firewall_node_Dash3);
    static_fire_obstacle_init(&firewall->static_obstacles[4], STATIC_FIRE_OBSTACLE_JUMP, reference_node, &tunnel_level->player, firewall->flame_mesh);

    reference_node = fw64_scene_get_node(scene, FW64_scene_firewall_node_Roll2);
    static_fire_obstacle_init(&firewall->static_obstacles[5], STATIC_FIRE_OBSTACLE_ROLL, reference_node, &tunnel_level->player, firewall->flame_mesh);

    reference_node = fw64_scene_get_node(scene, FW64_scene_firewall_node_Jump2);
    static_fire_obstacle_init(&firewall->static_obstacles[6], STATIC_FIRE_OBSTACLE_JUMP, reference_node, &tunnel_level->player, firewall->flame_mesh);
}

void tunnel_firewall_update(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* tunnel_level = (TunnelLevel*)level_arg;
    Firewall* firewall = (Firewall*)data_arg;

    animated_material_texture_update(&firewall->animated_material, tunnel_level->engine->time->time_delta);

    for (int i = 0; i < STATIC_FIRE_OBSTACLE_COUNT; i++) {
        if (static_fire_obstacle_update(&firewall->static_obstacles[i], tunnel_level->engine->time->time_delta)) {
            tunnel_level_kill_player(tunnel_level);
            return;
        }
    }
}

void tunnel_firewall_draw(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* tunnel_level = (TunnelLevel*)level_arg;
    Firewall* firewall = (Firewall*)data_arg;

    for (int i = 0; i < STATIC_FIRE_OBSTACLE_COUNT; i++) {
        static_fire_obstacle_draw(firewall->static_obstacles + i, tunnel_level->engine->renderer);
    }
}