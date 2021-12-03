#include "tunnel_level.h"

#include "assets.h"
#include "music_bank_music.h"
#include "sound_bank_sound_effects.h"
#include "scene_hallway.h"
#include "typemap.h"

static void tunnel_level_scene_activated(void* level_arg, fw64Scene* scene, void* data);
static void on_fade_effect_complete(FadeDirection direction, void* arg);

void tunnel_level_init(TunnelLevel* level, fw64Engine* engine) {
    level->engine = engine;

    level->game_settings = NULL;

    player_init(&level->player, level->engine, NULL);
    vec3_set_all(&level->player.node.transform.scale, 0.02f);
    fw64_node_update(&level->player.node);
    player_calculate_size(&level->player);
    
    chase_camera_init(&level->chase_cam, engine);
    level->chase_cam.target = &level->player.node.transform;
    level->chase_cam.target_follow_height = 14.5f;
    level->chase_cam.target_forward_height = 5.0f;
    level->chase_cam.target_follow_dist = 26.0f;

    ui_init(&level->ui, engine, &level->player);

    scene_manager_init(&level->scene_manager, engine, level, sizeof(SceneData), tunnel_level_scene_activated, &level->player.node.transform);
    SceneDescription desc;
    tunnel_hallway_description(&desc);
    scene_manager_load_current_scene(&level->scene_manager, &desc); // note this will activate the scene


    fw64_renderer_set_light_enabled(engine->renderer, 1, 1);

    fade_effect_init(&level->fade_effect);
    fade_effect_set_callback(&level->fade_effect, on_fade_effect_complete, level);

    level->sound_bank = fw64_sound_bank_load(engine->assets, FW64_ASSET_soundbank_sound_effects);
    level->music_bank = fw64_music_bank_load(engine->assets, FW64_ASSET_musicbank_music);

    fw64_audio_set_sound_bank(engine->audio, level->sound_bank);
    fw64_audio_set_music_bank(engine->audio, level->music_bank);

    fw64_audio_play_music(engine->audio, music_bank_music_runnyeye);

    level->debug = 1;
}


void tunnel_level_update(TunnelLevel* level){
    if (level->debug) {
        level->debug = 0;
        return;
    }

    trigger_box_update(&level->next_scene_trigger);

    if (trigger_box_was_triggered(&level->next_scene_trigger)) {
        tunnel_level_load_next(level);
    }

    scene_manager_update(&level->scene_manager);

    //this is just a little FOV trick to make dashing look "fast"
    //todo: this is hacky and should go somewhere else for better consistency
    //      but i couldn't figure out how to shove it into player update
    if(level->player.is_dashing)
    {
        level->chase_cam.camera.fovy = ((level->chase_cam.camera.fovy + (45.0f + (2.0f * (level->player.speed / level->player.max_speed)))) / 2.0f );
        fw64_camera_update_projection_matrix(&level->chase_cam.camera);
    }
    else
    {
        level->chase_cam.camera.fovy = 45.0f;
        fw64_camera_update_projection_matrix(&level->chase_cam.camera);
    }
    player_update(&level->player);

    if (level->player.node.transform.position.y < -30.0f) {
        tunnel_level_kill_player(level);
    }

    chase_camera_update(&level->chase_cam);
    fade_effect_update(&level->fade_effect, level->engine->time->time_delta);
    ui_update(&level->ui);
}

void tunnel_level_draw(TunnelLevel* level) {
    fw64Renderer* renderer = level->engine->renderer;
    fw64_renderer_begin(renderer, &level->chase_cam.camera, FW64_RENDERER_MODE_TRIANGLES, FW64_RENDERER_FLAG_CLEAR);
    scene_manager_draw(&level->scene_manager);
    player_draw(&level->player); // player is drawn last due to sparkle effect ignoring depth buffer
    fade_effect_draw(&level->fade_effect, level->engine->renderer);
    fw64_renderer_end(renderer, FW64_RENDERER_FLAG_NOSWAP);

    fw64_renderer_begin(renderer, &level->chase_cam.camera, FW64_RENDERER_MODE_ORTHO2D, FW64_RENDERER_FLAG_NOCLEAR);
    ui_draw(&level->ui);
    fw64_renderer_end(renderer, FW64_RENDERER_FLAG_SWAP);
}

// when the scene manager swaps a scene the player's scene needs to be updated (for collisions)
// if the player's scene is NULL, then we are loading the first level
// We will also update the triggerbox that is responsible for loading the next level
void tunnel_level_scene_activated(void* level_arg, fw64Scene* scene, void* data) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    fw64Node* search_node = NULL;

    if (level->player.scene == NULL) {
        fw64_scene_find_nodes_with_type(scene, NODE_TYPE_START, &search_node, 1);
        level->player.node.transform.position = search_node->transform.position;
    }
    
    player_set_scene(&level->player, scene);

    search_node = NULL;
    fw64_scene_find_nodes_with_type(scene, NODE_TYPE_NEXTSCENE, &search_node, 1);
    trigger_box_init(&level->next_scene_trigger, search_node, &level->player.collider);
}

void tunnel_level_uninit(TunnelLevel* level) {
    fw64_renderer_set_light_enabled(level->engine->renderer, 1, 0);
}

void tunnel_level_set_game_settings(TunnelLevel* level, GameSettings* settings) {
    level->game_settings = settings;    
    level->player.settings = settings;
}

void tunnel_level_load_next(TunnelLevel* level) {
    SceneRef* current_scene = scene_manager_get_current(&level->scene_manager);
    fw64Node* connector = NULL;
    fw64_scene_find_nodes_with_type(current_scene->scene, NODE_TYPE_CONNECTOR, &connector, 1);

    switch (current_scene->desc.index)
    {
        case FW64_ASSET_scene_hallway: {
            SceneDescription desc;
            tunnel_lavapit_description(&desc);
            scene_manager_load_next_scene(&level->scene_manager, &desc, &connector->transform);
            break;
        }
        
        case FW64_ASSET_scene_lavapit: {
            SceneDescription desc;
            tunnel_firewall_description(&desc);
            scene_manager_load_next_scene(&level->scene_manager, &desc, &connector->transform);
            break;
        }

        case FW64_ASSET_scene_firewall: {
            SceneDescription desc;
            tunnel_atrium_description(&desc);
            scene_manager_load_next_scene(&level->scene_manager, &desc, &connector->transform);
            break;
        }
    
    default:
        break;
    }
}

void on_fade_effect_complete(FadeDirection direction, void* arg) {
    TunnelLevel* level = (TunnelLevel*)arg;

    if (direction == FADE_IN) {
            SceneRef* current = scene_manager_get_current(&level->scene_manager);
            fw64Node* start_node;

            if (fw64_scene_find_nodes_with_type(current->scene, NODE_TYPE_START, &start_node, 1)) {
                player_reset_at_position(&level->player, &start_node->transform.position);
            }

            fade_effect_start(&level->fade_effect, FADE_OUT, 2.0f);
            fw64_audio_play_sound(level->engine->audio, sound_bank_sound_effects_respawn);
    }
    if (direction == FADE_OUT) {
        level->player.process_input = 1;
    }
}

void tunnel_level_kill_player(TunnelLevel* level) {
    if (tunnel_level_player_is_dying(level)) {
        return;
    }

    level->player.process_input = 0;
    fade_effect_start(&level->fade_effect, FADE_IN, 0.35f);
}

int tunnel_level_player_is_dying(TunnelLevel* level) {
    return fade_effect_is_active(&level->fade_effect);
}
