#include "level_base.h"

#include "typemap.h"

#include "framework64/matrix.h"

#include <string.h>

#define CURRENT_SCENE_REF(level) (&(level)->scenes[(level)->current_scene])
#define NEXT_SCENE_INDEX(level) (level)->current_scene == 1 ? 0 : 1
#define NEXT_SCENE_REF(level) (&(level)->scenes[NEXT_SCENE_INDEX((level))])


void scene_manager_init(SceneManager* scene_manager, fw64Engine* engine, void* level_arg, int data_size, SceneActivatedFunc scene_activated_func) {
    scene_manager->engine = engine;
    scene_manager->level_arg = level_arg;
    scene_manager->data_size = data_size;
    scene_manager->scene_activated_func = scene_activated_func
    scene_manager->current_scene = 0;
    
    memset(&scene_manager->scene_refs, 0, sizeof(SceneRef) * 2);
}


static void level_swap_scenes(LevelBase* level) {
    level->current_scene = level->current_scene == 1 ? 0 : 1;
    level->player.scene = CURRENT_SCENE_REF(level)->scene;
}


void scene_manager_update(SceneManager* level){
    SceneRef* current_scene = CURRENT_SCENE_REF(level);
    SceneRef* next_scene = NEXT_SCENE_REF(level);

    // check to see if the player has moved to the next scene
    if (next_scene->scene) {

        if (box_contains_point(fw64_scene_get_initial_bounds(next_scene->scene), &level->player.node.transform.position)) {
            level_swap_scenes(level);
        }
    }

    if (current_scene->scene) {
        current_scene->update_func(level->level_arg, level->data_args[level->current_scene]);
    }
}

void scene_manager_draw(SceneManager* scene_manager) {
    fw64Renderer* renderer = scene_manager->engine->renderer;
    SceneRef* current_scene = CURRENT_SCENE_REF(scene_manager);
    SceneRef* next_scene = NEXT_SCENE_REF(scene_manager);

    if (current_scene->scene) {
        fw64_scene_draw_all(current_scene->scene, renderer);
        current_scene->draw_func(current_scene->scene, scene_manager->level_arg, current_scene->data);
    }
        

    if (next_scene->scene) {
        fw64_scene_draw_all(next_scene->scene, renderer);
        next_scene->draw_func(next_scene->scene, scene_manager->level_arg, next_scene->data);
    }
    
}

static void set_scene_ref(SceneManager* scene_manager, int ref_index, SceneDescription* description) {
    SceneRef* scene_ref = &level->scenes[ref_index];

    if (scene_ref->scene) {
        scene_ref->uninit_func(level->level_arg, scene_ref->data);
    }

    memcpy(&scene_ref->description, description, sizeof(SceneDescription));

    scene_ref->init_func(scene_ref->scene, level->level_arg, level->data_args[ref_index]);
}

void level_base_load_current_scene(LevelBase* level, uint32_t index, SceneInitFunc init_func, SceneFunc update_func, SceneFunc draw_func,SceneFunc uninit_func, void* arg) {
    set_scene_ref(level, level->current_scene, index, init_func, update_func, draw_func, uninit_func, arg);
}

void level_base_load_next_scene(LevelBase* level, uint32_t index, SceneInitFunc init_func, SceneFunc update_func, SceneFunc draw_func, SceneFunc uninit_func, void* arg) {
    SceneRef* next_scene_ref = NEXT_SCENE_REF(level);
    
    if (next_scene_ref->index != index) {
        set_scene_ref(level, level->current_scene == 1 ? 0 : 1, index, init_func, update_func, draw_func, uninit_func, arg);
    }
}