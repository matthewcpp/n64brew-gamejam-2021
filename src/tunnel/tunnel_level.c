#include "tunnel_level.h"

#include "hallway.h"

#include "assets.h"
#include "scene_hallway.h"

static void tunnel_level_scene_activated(void* level_arg, fw64Scene* scene);

void tunnel_level_init(TunnelLevel* level, fw64Engine* engine) {
    level->engine = engine;
    scene_manager_init(&level->scene_manager, engine, level, sizeof(SceneData));

    player_init(&level->player, level->engine, NULL);
    vec3_set_all(&level->player.node.transform.scale, 0.01f);
    level->player.jump_impulse = 16.0f;
    level->player.gravity = -38.0f;
    level->player.max_speed = 15.0f;
    level->player.acceleration = 14.0f;
    fw64_node_update(&level->player.node);
    player_calculate_size(&level->player);
    
    chase_camera_init(&level->chase_cam, engine);
    level->chase_cam.target = &level->player.node.transform;
    level->chase_cam.target_follow_height = 5.0f;
    level->chase_cam.target_forward_height = 6.0f;
    level->chase_cam.target_follow_dist = 13.0f;

    ui_init(&level->ui, engine, &level->player);

    SceneDescription desc;
    tunnel_hallway_description(&desc);
    level_base_load_current_scene(&level->scene_manager, &desc);

    fw64_renderer_set_light_enabled(engine->renderer, 1, 1);

    level->debug = 1;
}



void tunnel_level_update(TunnelLevel* level){
    if (level->debug) {
        level->debug = 0;
        return;
    }

    player_update(&level->player);
    scene_manager_update(&level->scene_manager);
    chase_camera_update(&level->chase_cam);
    ui_update(&level->ui);
}

void tunnel_level_draw(TunnelLevel* level) {
    fw64Renderer* renderer = level->engine->renderer;
    fw64_renderer_begin(renderer, &level->chase_cam.camera, FW64_RENDERER_MODE_TRIANGLES, FW64_RENDERER_FLAG_CLEAR);
    player_draw(&level->player);
    scene_manager_draw(&level->scene_manager);
    fw64_renderer_end(renderer, FW64_RENDERER_FLAG_NOSWAP);

    fw64_renderer_begin(renderer, &level->chase_cam.camera, FW64_RENDERER_MODE_ORTHO2D, FW64_RENDERER_FLAG_NOCLEAR);
    ui_draw(&level->ui);
    fw64_renderer_end(renderer, FW64_RENDERER_FLAG_SWAP);
}

void tunnel_level_uninit(TunnelLevel* level) {
    fw64_renderer_set_light_enabled(level->base.engine->renderer, 1, 0);
}

void tunnel_level_load_next(TunnelLevel* level, uint32_t current_index) {
    switch (current_index)
    {
        case FW64_ASSET_scene_hallway: {
            SceneDescription desc;
            tunnel_atrium_description(&desc);
            level_base_load_current_scene(&level->scene_manager, &desc);
            break;
        }
        
    
    default:
        break;
    }
}