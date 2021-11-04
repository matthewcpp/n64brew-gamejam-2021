#include "tunnel_level.h"

#include "assets.h"

float spawn_x[3] = {-5.0, 0.0, 5.0};

static void respawn_enemies(TunnelLevel* level);

void tunnel_level_init(TunnelLevel* level, fw64Engine* engine) {
    level_base_init(&level->base, engine, FW64_ASSET_scene_hallway);
    level->base.chase_cam.target_follow_height = 5.0f;
    level->base.chase_cam.target_forward_height = 6.0f;
    level->base.chase_cam.target_follow_dist = 13.0f;
    level->base.player.jump_impulse = 16.0f;
    level->base.player.gravity = -38.0f;
    level->base.player.max_speed = 15.0f;
    level->base.player.acceleration = 14.0f;

    level->boo_mesh = fw64_mesh_load(engine->assets, FW64_ASSET_mesh_boo);

    for (int i = 0; i < 3; i++) {
        Vec3 pos = {spawn_x[i], 1.1f, level->base.player.node.transform.position.z - 30.0f};
        enemy_init(&level->enemy[i], level->base.engine, &level->base.player, level->boo_mesh, &pos);
    }

    level->triggered = 0;
    level->debug = 1;
}

void respawn_enemies(TunnelLevel* level) {
    for (int i = 0; i < 3; i++) {
        Vec3 pos = {spawn_x[i], 1.1f, level->base.player.node.transform.position.z - 30.0f};
        level->enemy[i].node.transform.position = pos;
    }
}

#define SCALE_SPEED 0.25f
#define SCALE_MAX 0.2f

void tunnel_level_update(TunnelLevel* level){
    if (level->debug) {
        level->debug = 0;
        return;
    }
    level_base_update(&level->base);
    
    if (!level->triggered) {
        for (int i = 0; i < 3; i++) {
            enemy_update(&level->enemy[i]);
        }

        if (level->enemy[0].collider.bounding.min.z > level->base.player.collider.bounding.min.z + 10.0f && level->base.player.state == PLAYER_STATE_ON_GROUND) {
            respawn_enemies(level);
        }
    }
    else {
        Enemy* pumpkin = &level->enemy[0];
        float scale = pumpkin->node.transform.scale.x;
        scale += SCALE_SPEED * level->base.engine->time->time_delta;
        if (scale > SCALE_MAX)
            scale = SCALE_MAX;

        vec3_set_all(&pumpkin->node.transform.scale, scale);
        fw64_node_update(&pumpkin->node);
    }


    if (level->base.player.previous_position.z > -100.0f && level->base.player.node.transform.position.z <= -100) {
        level->triggered = 1;
        level->pumpkin_mesh = fw64_mesh_load(level->base.engine->assets, FW64_ASSET_mesh_jack_o_lantern);
        Enemy* pumpkin = &level->enemy[0];
        fw64_node_set_mesh(&pumpkin->node, level->pumpkin_mesh);
        vec3_set(&pumpkin->node.transform.position, 0.0f, 1.0f, -135.0f);
        quat_ident(&pumpkin->node.transform.rotation);
        vec3_set_all(&pumpkin->node.transform.scale, 0.01f);
        
        fw64_node_update(&pumpkin->node);
    }
}

void tunnel_level_draw(TunnelLevel* level) {
    fw64Renderer* renderer = level->base.engine->renderer;

    fw64_renderer_begin(renderer, &level->base.chase_cam.camera, FW64_RENDERER_MODE_TRIANGLES, FW64_RENDERER_FLAG_CLEAR);
    fw64_scene_draw_all(level->base.scene, renderer);
    player_draw(&level->base.player);

    if (!level->triggered) {
        for (int i = 0; i < 3; i++) {
            enemy_draw(&level->enemy[i]);
        }
    }
    else {
        enemy_draw(&level->enemy[0]);
    }


    fw64_renderer_end(renderer, FW64_RENDERER_FLAG_NOSWAP);

    fw64_renderer_begin(renderer, &level->base.chase_cam.camera, FW64_RENDERER_MODE_ORTHO2D, FW64_RENDERER_FLAG_NOCLEAR);
    ui_draw(&level->base.ui);
    fw64_renderer_end(renderer, FW64_RENDERER_FLAG_SWAP);
}

void tunnel_level_uninit(TunnelLevel* level) {
    
}