#include "scene_manager.h"

#include "typemap.h"

#include "framework64/matrix.h"

#include <string.h>
#include <stdlib.h>

#define CURRENT_SCENE_REF(scene_manager) (&(scene_manager)->scene_refs[(scene_manager)->current_scene])
#define NEXT_SCENE_INDEX(scene_manager) (scene_manager)->current_scene == 1 ? 0 : 1
#define NEXT_SCENE_REF(scene_manager) (&(scene_manager)->scene_refs[NEXT_SCENE_INDEX((scene_manager))])


void scene_manager_init(SceneManager* scene_manager, fw64Engine* engine, void* level_arg, int data_size, SceneFunc swap_func, fw64Transform* target) {
    scene_manager->engine = engine;
    scene_manager->level_arg = level_arg;
    scene_manager->data_size = data_size;
    scene_manager->current_scene = 0;
    scene_manager->swap_func = swap_func;
    scene_manager->target = target;
    
    memset(&scene_manager->scene_refs, 0, sizeof(SceneRef) * 2);
}

void scene_manager_uninit(SceneManager* scene_manager) {

}


void scene_manager_update(SceneManager* scene_manager){
    SceneRef* current_scene = CURRENT_SCENE_REF(scene_manager);
    SceneRef* next_scene = NEXT_SCENE_REF(scene_manager);

    // check to see if the player has moved to the next scene
    if (next_scene->scene) {
        if (box_contains_point(fw64_scene_get_initial_bounds(next_scene->scene), &scene_manager->target->position)) {
            scene_manager->current_scene = NEXT_SCENE_INDEX(scene_manager);
            current_scene = next_scene;
            scene_manager->swap_func(scene_manager->level_arg, current_scene->scene, current_scene->data);
            fw64_renderer_set_clear_color(scene_manager->engine->renderer, 0, 255, 0);
        }
    }

    // we only update the current scene
    if (current_scene->scene && current_scene->desc.update_func) {
        current_scene->desc.update_func(scene_manager->level_arg, current_scene->scene, current_scene->data);
    }
}

void scene_manager_draw(SceneManager* scene_manager) {
    fw64Renderer* renderer = scene_manager->engine->renderer;
    SceneRef* current_scene = CURRENT_SCENE_REF(scene_manager);
    SceneRef* next_scene = NEXT_SCENE_REF(scene_manager);

    if (current_scene->scene) {
        fw64_scene_draw_all(current_scene->scene, renderer);

        if (current_scene->desc.draw_func) {
            current_scene->desc.draw_func(scene_manager->level_arg, current_scene->scene, current_scene->data);
        }
    }
        
    if (next_scene->scene) {
        fw64_scene_draw_all(next_scene->scene, renderer);

        if (next_scene->desc.draw_func) {
            next_scene->desc.draw_func(scene_manager->level_arg, next_scene->scene, next_scene->data);
        }
    }
}

static void set_scene_ref(SceneManager* scene_manager, int ref_index, SceneDescription* description) {
    SceneRef* scene_ref = &scene_manager->scene_refs[ref_index];

    if (scene_ref->scene) {
        if (scene_ref->desc.uninit_func) {
            scene_ref->desc.uninit_func(scene_manager->level_arg, scene_ref->scene, scene_ref->data);
        }
        
        free(scene_ref->data); // note this will be replaced by bump allocator reset
    }

    // set up the new scene
    memcpy(&scene_ref->desc, description, sizeof(SceneDescription));
    scene_ref->scene = fw64_scene_load(scene_manager->engine->assets, scene_ref->desc.index);
    scene_ref->data = malloc(scene_manager->data_size); // note this will be replaced by bump allocator

    if (scene_ref->desc.init_func) {
        scene_ref->desc.init_func(scene_manager->level_arg, scene_ref->scene, scene_ref->data);
    }
}

void level_base_load_current_scene(SceneManager* scene_manager, SceneDescription* description) {
    set_scene_ref(scene_manager, scene_manager->current_scene, description);
}

void level_base_load_next_scene(SceneManager* scene_manager, SceneDescription* description) {
    SceneRef* next_scene_ref = NEXT_SCENE_REF(scene_manager);
    
    if (next_scene_ref->desc.index != description->index) {
        set_scene_ref(scene_manager, NEXT_SCENE_INDEX(scene_manager), description);
    }
}

fw64Scene* scene_manager_get_current_scene(SceneManager* scene_manager) {
    return CURRENT_SCENE_REF(scene_manager)->scene;
}
