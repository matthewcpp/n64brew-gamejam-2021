#pragma once

#include "framework64/engine.h"
#include "framework64/scene.h"
#include "player.h"
#include "chase_camera.h"
#include "ui.h"

#include <limits.h>

#define INVALID_SCENE_INDEX INT_MAX

typedef void(*SceneInitFunc)(fw64Scene* scene, void* level, void* data);
typedef void(*SceneFunc)(void* level, void* data);

typedef struct {
    fw64Scene* scene;
    uint32_t index;
    SceneInitFunc init_func;
    SceneFunc update_func;
    SceneFunc uninit_func;
} SceneRef;

typedef struct {
    fw64Engine* engine;

    SceneRef scenes[2];
    void* data_args[2];
    void* level_arg;
    int current_scene;

    UI ui;

    Player player;
    ChaseCamera chase_cam;

} LevelBase;


#ifdef __cplusplus
extern "C" {
#endif

void level_base_init(LevelBase* level, fw64Engine* engine);

// this is unfortunate but I cant think of a better way to do this right now;
void level_base_set_scene_data(LevelBase* level, int index, void* scene_data);
void level_base_set_level_arg(LevelBase* level, void* level_arg);

void level_base_update(LevelBase* level);
void level_base_draw(LevelBase* level);
void level_base_setup_player(LevelBase* level, uint32_t start_node_index);
void level_base_load_current_scene(LevelBase* level, uint32_t index, SceneInitFunc init_func, SceneFunc update_func, SceneFunc uninit_func, void* arg);
void level_base_load_next_scene(LevelBase* level, uint32_t index, SceneInitFunc init_func, SceneFunc update_func, SceneFunc uninit_func, void* arg);


#ifdef __cplusplus
}
#endif
