#pragma once

#include "framework64/util/bump_allocator.h"
#include "framework64/engine.h"
#include "framework64/scene.h"

typedef void(*SceneFunc)(void* level_arg, fw64Scene* scene, void* data_arg);

typedef struct {
    uint32_t index;
    uint32_t data_size;
    SceneFunc init_func;
    SceneFunc update_func;
    SceneFunc draw_func;
    SceneFunc ui_draw_func;
    SceneFunc uninit_func;
} SceneDescription;

typedef struct {
    SceneDescription desc;
    fw64BumpAllocator allocator;
    fw64Scene* scene;
    void* data;
} SceneRef;

typedef struct {
    fw64Engine* engine;
    fw64Transform* target;
    void* level_arg;
    int current_scene;
    SceneFunc swap_func;
    SceneRef scene_refs[2]; 
} SceneManager;

#ifdef __cplusplus
extern "C" {
#endif

void scene_manager_init(SceneManager* scene_manager, fw64Engine* engine, void* level_arg, SceneFunc swap_func, fw64Transform* target);
void scene_manager_uninit(SceneManager* scene_manager);
void scene_manager_update(SceneManager* scene_manager);
void scene_manager_draw(SceneManager* scene_manager);
void scene_manager_ui_draw(SceneManager* scene_manager);
void scene_manager_load_current_scene(SceneManager* scene_manager, SceneDescription* description);
void scene_manager_load_next_scene(SceneManager* scene_manager, SceneDescription* description, fw64Transform* offset);

SceneRef* scene_manager_get_current(SceneManager* scene_manager);


#ifdef __cplusplus
}
#endif
