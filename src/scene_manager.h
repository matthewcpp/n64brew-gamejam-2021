#pragma once

#include "framework64/engine.h"
#include "framework64/scene.h"
#include "player.h"
#include "chase_camera.h"
#include "ui.h"

#include <limits.h>

#define INVALID_SCENE_INDEX INT_MAX

typedef void(*SceneFunc)(void* level_arg, fw64Scene* scene, void* data);

typedef struct {
    uint32_t index;
    SceneFunc init_func;
    SceneFunc activated_func;
    SceneFunc update_func;
    SceneFunc draw_func;
    SceneFunc uninit_func;
} SceneDescription;

typedef struct {
    SceneDescription description;
    fw64Scene* scene;
    void* data;
} SceneRef;

typedef struct {
    fw64Engine* engine;
    void* level_arg;
    int current_scene;
    int data_size;
    SceneRef scene_refs[2]; 
} SceneManager;


#ifdef __cplusplus
extern "C" {
#endif

void scene_manager_init(SceneManager* scene_manager, fw64Engine* engine, void* level_arg, int data_size);

void scene_manager_update(SceneManager* scene_manager);
void scene_manager_draw(SceneManager* scene_manager);
void scene_manager_load_current_scene(SceneManager* scene_manager, SceneDescription* description);
void scene_manager_load_next_scene(SceneManager* scene_manager, SceneDescription* description);


#ifdef __cplusplus
}
#endif
