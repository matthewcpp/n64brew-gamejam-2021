#include "tunnel_level.h"

#include "assets.h"
#include "music_bank_music.h"
#include "sound_bank_sound_effects.h"
#include "scene_hallway.h"
#include "typemap.h"

// 11th hour fix for moving backwards
#define SCENE_COUNT 4
int scene_handles[SCENE_COUNT] = {FW64_ASSET_scene_hallway, FW64_ASSET_scene_lavapit, FW64_ASSET_scene_firewall, FW64_ASSET_scene_atrium};

static int find_scene_index(int handle) {
    for (int i = 0; i < SCENE_COUNT; i++) {
        if (scene_handles[i] == handle)
            return i;
    }
}

// TODO: this can probably be tweaked...the current debug font is really big
#define TUNNEL_ALLOCATOR_SIZE 200 * 1024

static void tunnel_level_scene_activated(void* level_arg, fw64Scene* scene, void* data);
static void on_fade_effect_complete(FadeDirection direction, void* arg);
static void tunnel_level_load_prev(TunnelLevel* level);

void tunnel_level_init(TunnelLevel* level, fw64Engine* engine, GameStateData* state_data) {
    level->engine = engine;
    level->state_data = state_data;

    level->game_settings = NULL;
    level->scene_index = 0;

    fw64_bump_allocator_init(&level->bump_allocator, TUNNEL_ALLOCATOR_SIZE);
    fw64Allocator* allocator = &level->bump_allocator.interface;

    player_init(&level->player, level->engine, NULL, allocator);
    vec3_set_all(&level->player.node.transform.scale, 0.02f);
    fw64_node_update(&level->player.node);
    player_calculate_size(&level->player);
    
    chase_camera_init(&level->chase_cam, engine);
    chase_camera_reset(&level->chase_cam, &level->player.node.transform);
    
    ui_init(&level->ui, engine, &level->player, &level->chase_cam, allocator);
    ui_set_mode(&level->ui, UI_MODE_NORMAL_HUD);

    scene_manager_init(&level->scene_manager, engine, level, tunnel_level_scene_activated, &level->player.node.transform);
    SceneDescription desc;
    tunnel_hallway_description(&desc);
    scene_manager_load_current_scene(&level->scene_manager, &desc); // note this will activate the scene
    int index = find_scene_index(desc.index);
    vec3_zero(&level->scene_connectors[index]);

    fw64_renderer_set_light_enabled(engine->renderer, 1, 1);

    fw64ColorRGBA8 fade_color = {255, 255, 255, 255};
    fade_effect_init(&level->fade_effect);
    level->fade_effect.color = fade_color;
    fade_effect_set_callback(&level->fade_effect, on_fade_effect_complete, level);
  
    

    fw64_audio_play_music(engine->audio, music_bank_music_runnyeye);

    level->debug = 1;
}


void tunnel_level_update(TunnelLevel* level){
    if (level->debug) {
        level->debug = 0;
        return;
    }

    if (fw64_audio_get_music_status(level->engine->audio) == FW64_AUDIO_STOPPED) {
        fw64_audio_play_music(level->engine->audio, music_bank_music_runnyeye);
    }

    trigger_box_update(&level->next_scene_trigger);
    trigger_box_update(&level->prev_scene_trigger);

    if (trigger_box_was_triggered(&level->next_scene_trigger)) {
        tunnel_level_load_next(level);
    }

    if (trigger_box_was_triggered(&level->prev_scene_trigger)) {
        tunnel_level_load_prev(level);
    }

    scene_manager_update(&level->scene_manager);

    //this is just a little FOV trick to make dashing look "fast"
    //todo: this is hacky and should go somewhere else for better consistency
    //      but i couldn't figure out how to shove it into player update
    if(level->player.is_dashing && level->chase_cam.mode == CAMERA_MODE_CHASE) {
        level->chase_cam.camera.fovy = ((level->chase_cam.camera.fovy + (45.0f + (2.0f * (level->player.speed / level->player.max_speed)))) / 2.0f );
        fw64_camera_update_projection_matrix(&level->chase_cam.camera);
    }
    else {
        level->chase_cam.camera.fovy = 45.0f;
        fw64_camera_update_projection_matrix(&level->chase_cam.camera);
    }
    player_update(&level->player);

    if (level->player.node.transform.position.y < -30.0f) {
        tunnel_level_kill_player(level);
    }

    chase_camera_update(&level->chase_cam);
    chase_camera_get_forward(&level->chase_cam, &level->player.camera_forward);
    fade_effect_update(&level->fade_effect, level->engine->time->time_delta);
    ui_update(&level->ui);
}

void tunnel_level_draw(TunnelLevel* level) {
    fw64Renderer* renderer = level->engine->renderer;

    fw64_renderer_set_anti_aliasing_enabled(renderer, 1);

    fw64_renderer_begin(renderer, &level->chase_cam.camera, FW64_RENDERER_MODE_TRIANGLES, FW64_RENDERER_FLAG_CLEAR);
    scene_manager_draw(&level->scene_manager);
    player_draw(&level->player); // player is drawn last due to sparkle effect ignoring depth buffer
    fade_effect_draw(&level->fade_effect, level->engine->renderer);
    fw64_renderer_end(renderer, FW64_RENDERER_FLAG_NOSWAP);

    fw64_renderer_set_anti_aliasing_enabled(renderer, 0);

    fw64_renderer_begin(renderer, &level->chase_cam.camera, FW64_RENDERER_MODE_ORTHO2D, FW64_RENDERER_FLAG_NOCLEAR);
    ui_draw(&level->ui);
    scene_manager_ui_draw(&level->scene_manager);
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
    tunnel_level_set_camera_for_scene(level);
    chase_camera_set_scene(&level->chase_cam, scene);

    search_node = NULL;
    fw64_scene_find_nodes_with_type(scene, NODE_TYPE_NEXTSCENE, &search_node, 1);
    trigger_box_init(&level->next_scene_trigger, search_node, &level->player.collider);

    search_node = NULL;
    fw64_scene_find_nodes_with_type(scene, NODE_TYPE_PREVSCENE, &search_node, 1);
    trigger_box_init(&level->prev_scene_trigger, search_node, &level->player.collider);
}

void tunnel_level_uninit(TunnelLevel* level) {
    fw64_renderer_set_light_enabled(level->engine->renderer, 1, 0);
    fw64_audio_stop_music(level->engine->audio);
    
    fw64Allocator* allocator = &level->bump_allocator.interface;
    player_uninit(&level->player, allocator);

    scene_manager_uninit(&level->scene_manager);

    ui_uninit(&level->ui, allocator);

    fw64_bump_allocator_uninit(&level->bump_allocator);
}

void tunnel_level_set_game_settings(TunnelLevel* level, GameSettings* settings) {
    level->game_settings = settings;    
    level->player.settings = settings;
}

void tunnel_level_load_next(TunnelLevel* level) {
    SceneRef* current_scene = scene_manager_get_current(&level->scene_manager);
    fw64Node* connector = NULL;
    fw64_scene_find_nodes_with_type(current_scene->scene, NODE_TYPE_CONNECTOR, &connector, 1);

    SceneDescription desc;
    switch (current_scene->desc.index)
    {
        case FW64_ASSET_scene_hallway:
            tunnel_lavapit_description(&desc);
            break;
        
        case FW64_ASSET_scene_lavapit: 
            tunnel_firewall_description(&desc);
            break;

        case FW64_ASSET_scene_firewall: 
            tunnel_atrium_description(&desc);
            break;
    
    default:
        return;
    }

    
    int index = find_scene_index(desc.index);
    level->scene_connectors[index] = connector->transform.position;
    scene_manager_load_next_scene(&level->scene_manager, &desc, &connector->transform);
}

void tunnel_level_load_prev(TunnelLevel* level) {
    SceneRef* current_scene = scene_manager_get_current(&level->scene_manager);
    
    SceneDescription desc;

    switch (current_scene->desc.index)
    {        
        case FW64_ASSET_scene_lavapit: 
            tunnel_hallway_description(&desc);
            break;
        

        case FW64_ASSET_scene_firewall: 
            tunnel_lavapit_description(&desc);
            break;

        case FW64_ASSET_scene_atrium:
            tunnel_firewall_description(&desc);
            break;
    
    default:
        return;
    }

    int index = find_scene_index(desc.index);
    fw64Transform temp_transform;
    fw64_transform_init(&temp_transform);
    temp_transform.position = level->scene_connectors[index];
    scene_manager_load_next_scene(&level->scene_manager, &desc, &temp_transform);
}

void on_fade_effect_complete(FadeDirection direction, void* arg) {
    TunnelLevel* level = (TunnelLevel*)arg;

    if (direction == FADE_OUT) {
            SceneRef* current = scene_manager_get_current(&level->scene_manager);
            fw64Node* start_node;

            if (fw64_scene_find_nodes_with_type(current->scene, NODE_TYPE_START, &start_node, 1)) {
                player_reset_at_position(&level->player, &start_node->transform.position);
                chase_camera_reset(&level->chase_cam, &level->player.node.transform);
            }

            fade_effect_start(&level->fade_effect, FADE_IN, 2.0f);
            fw64_audio_play_sound(level->engine->audio, sound_bank_sound_effects_respawn);
    }
    if (direction == FADE_IN) {
        level->player.process_input = 1;
        fade_effect_stop(&level->fade_effect);
    }
}

void tunnel_level_kill_player(TunnelLevel* level) {
    if (tunnel_level_player_is_dying(level)) {
        return;
    }

    level->player.process_input = 0;
    level->player.deaths++;
    fade_effect_start(&level->fade_effect, FADE_OUT, 0.35f);
}

int tunnel_level_player_is_dying(TunnelLevel* level) {
    return fade_effect_is_active(&level->fade_effect);
}

void tunnel_level_set_camera_for_scene(TunnelLevel* level) {
    SceneRef* current_scene = scene_manager_get_current(&level->scene_manager);
    switch (current_scene->desc.index)
    {
        case FW64_ASSET_scene_lavapit:
            chase_camera_set_mode(&level->chase_cam, CAMERA_MODE_SIDE, 44.5f, 16.0f, -17.0f, 0.0f);
            break;
        case FW64_ASSET_scene_hallway:  /* fall through */
        case FW64_ASSET_scene_atrium:   /* fall through */   
        case FW64_ASSET_scene_firewall: /* fall through */
        default:
            chase_camera_set_mode(&level->chase_cam, CAMERA_MODE_CHASE, 30.0f, 12.0f, 0.0f, 0.0f);
            break;
    }
}
