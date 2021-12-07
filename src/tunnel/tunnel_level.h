#pragma once

#include "framework64/util/bump_allocator.h"

#include "ui.h"
#include "chase_camera.h"
#include "scene_manager.h"
#include "fade_effect.h"
#include "trigger_box.h"
#include "states/gamestates.h"

typedef struct {
    fw64Engine* engine;
    GameStateData* state_data;
    GameSettings* game_settings;
    Player player;
    UI ui;
    ChaseCamera chase_cam;
    SceneManager scene_manager;
    TriggerBox next_scene_trigger;
    TriggerBox prev_scene_trigger;
    Vec3 scene_connectors[4];
    int scene_index;
    FadeEffect fade_effect;
    int debug;
    fw64SoundBank* sound_bank;
    fw64BumpAllocator bump_allocator;
} TunnelLevel;

#ifdef __cplusplus
extern "C" {
#endif

void tunnel_level_init(TunnelLevel* level, fw64Engine* engine, GameStateData* state_data);
void tunnel_level_uninit(TunnelLevel* level);
void tunnel_level_update(TunnelLevel* level);
void tunnel_level_draw(TunnelLevel* level);

void tunnel_level_set_game_settings(TunnelLevel* level, GameSettings* settings);
void tunnel_level_load_next(TunnelLevel* level);
int tunnel_level_player_is_dying(TunnelLevel* level);
void tunnel_level_kill_player(TunnelLevel* level);

// scene chunks
void tunnel_hallway_description(SceneDescription* desc);
void tunnel_lavapit_description(SceneDescription* desc);
void tunnel_firewall_description(SceneDescription* desc);
void tunnel_atrium_description(SceneDescription* desc);

void tunnel_level_set_camera_for_scene(TunnelLevel* level);

#ifdef __cplusplus
}
#endif
