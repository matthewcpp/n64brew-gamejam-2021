#include "tunnel_level.h"

#include "hallway.h"

#include "assets.h"
#include "scene_hallway.h"

float spawn_x[3] = {-5.0, 0.0, 5.0};

static void respawn_enemies(TunnelLevel* level);

void tunnel_level_init(TunnelLevel* level, fw64Engine* engine) {
    level_base_init(&level->base, engine);
    level->base.chase_cam.target_follow_height = 5.0f;
    level->base.chase_cam.target_forward_height = 6.0f;
    level->base.chase_cam.target_follow_dist = 13.0f;
    level->base.player.jump_impulse = 16.0f;
    level->base.player.gravity = -38.0f;
    level->base.player.max_speed = 15.0f;
    level->base.player.acceleration = 14.0f;

    // todo: see if we can fix these three calls somehow i really dont like this
    level_base_set_level_arg(&level->base, (void*)level);
    level_base_set_scene_data(&level->base, 0, &level->sceneData[0]);
    level_base_set_scene_data(&level->base, 1, &level->sceneData[1]);


    level_base_load_current_scene(&level->base, FW64_ASSET_scene_hallway, 
        tunnel_hallway_init, tunnel_hallway_update, tunnel_hallway_uninit, level);

    level_base_setup_player(&level->base, FW64_scene_hallway_node_Player_Start);

    level->debug = 1;
}


void tunnel_level_update(TunnelLevel* level){
    if (level->debug) {
        level->debug = 0;
        return;
    }

    level_base_update(&level->base);
}

void tunnel_level_draw(TunnelLevel* level) {
    level_base_draw(&level->base);
}

void tunnel_level_uninit(TunnelLevel* level) {
    
}

void tunnel_level_load_next(TunnelLevel* level, uint32_t current_index) {
    switch (current_index)
    {
    case FW64_ASSET_scene_hallway:
        level_base_load_next_scene(&level->base, FW64_ASSET_scene_atrium, 
            tunnel_atrium_init, tunnel_atrium_update, tunnel_atrium_uninit, level);
        break;
    
    default:
        break;
    }
}