#pragma once

#include "game_settings.h"

#include "hallway.h"
#include "atrium.h"
#include "lavapit.h"
#include "firewall.h"

#include "ui.h"
#include "chase_camera.h"
#include "scene_manager.h"
#include "fade_effect.h"

typedef union {
    Hallway hallway;
    LavaPit lava_pit;
    Atrium atrium;
    Firewall firewall;
} SceneData;

typedef struct {
    fw64Engine* engine;
    GameSettings* game_settings;
    Player player;
    UI ui;
    ChaseCamera chase_cam;
    SceneManager scene_manager;
    TriggerBox next_scene_trigger;
    FadeEffect fade_effect;
    int debug;
    fw64SoundBank* sound_bank;
    fw64MusicBank* music_bank;
} TunnelLevel;

#ifdef __cplusplus
extern "C" {
#endif

void tunnel_level_init(TunnelLevel* level, fw64Engine* engine);
void tunnel_level_uninit(TunnelLevel* level);
void tunnel_level_update(TunnelLevel* level);
void tunnel_level_draw(TunnelLevel* level);

void tunnel_level_set_game_settings(TunnelLevel* level, GameSettings* settings);
void tunnel_level_load_next(TunnelLevel* level);
int tunnel_level_player_is_dying(TunnelLevel* level);
void tunnel_level_kill_player(TunnelLevel* level);
#ifdef __cplusplus
}
#endif
