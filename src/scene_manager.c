#include "scene_manager.h"

#include "typemap.h"

#include "framework64/matrix.h"

#include <string.h>
#include <stdlib.h>

#ifdef PLATFORM_N64
    #include <malloc.h>
#endif

#define CURRENT_SCENE_REF(scene_manager) (&(scene_manager)->scene_refs[(scene_manager)->current_scene])
#define NEXT_SCENE_INDEX(scene_manager) (scene_manager)->current_scene == 1 ? 0 : 1
#define NEXT_SCENE_REF(scene_manager) (&(scene_manager)->scene_refs[NEXT_SCENE_INDEX((scene_manager))])

#define BUMP_ALLOCATOR_SIZE (64 * 1024)


void scene_manager_init(SceneManager* scene_manager, fw64Engine* engine, void* level_arg, SceneFunc swap_func, fw64Transform* target) {
    scene_manager->engine = engine;
    scene_manager->level_arg = level_arg;
    scene_manager->current_scene = 0;
    scene_manager->swap_func = swap_func;
    scene_manager->target = target;
    
    memset(&scene_manager->scene_refs, 0, sizeof(SceneRef) * 2);

    fw64_bump_allocator_init(&scene_manager->scene_refs[0].allocator, BUMP_ALLOCATOR_SIZE);
    fw64_bump_allocator_init(&scene_manager->scene_refs[1].allocator, BUMP_ALLOCATOR_SIZE);
}

void scene_manager_uninit(SceneManager* scene_manager) {
    fw64_bump_allocator_uninit(&scene_manager->scene_refs[0].allocator);
    fw64_bump_allocator_uninit(&scene_manager->scene_refs[1].allocator);
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

    fw64Frustum frustum;
    fw64_camera_extract_frustum_planes(fw64_renderer_get_camera(renderer), &frustum);

    if (current_scene->scene) {
        if (fw64_frustum_intersects_box(&frustum, fw64_scene_get_initial_bounds(current_scene->scene))) {
            fw64_scene_draw_all(current_scene->scene, renderer);

            if (current_scene->desc.draw_func) {
                current_scene->desc.draw_func(scene_manager->level_arg, current_scene->scene, current_scene->data);
            }
        }
    }
        
    if (next_scene->scene) {
        if (fw64_frustum_intersects_box(&frustum, fw64_scene_get_initial_bounds(next_scene->scene))) {
            fw64_scene_draw_all(next_scene->scene, renderer);

            if (next_scene->desc.draw_func) {
                next_scene->desc.draw_func(scene_manager->level_arg, next_scene->scene, next_scene->data);
            }
        }
    }
}

void scene_manager_ui_draw(SceneManager* scene_manager) {
    fw64Renderer* renderer = scene_manager->engine->renderer;
    SceneRef* current_scene = CURRENT_SCENE_REF(scene_manager);

    if (current_scene->desc.ui_draw_func) {
        current_scene->desc.ui_draw_func(scene_manager->level_arg, current_scene->scene, current_scene->data);
    }
}

static void apply_offset_to_scene(fw64Transform* offset, fw64Scene* scene) {
    uint32_t node_count = fw64_scene_get_node_count(scene);

    for (uint32_t i = 0; i < node_count; i++) {
        fw64Node* node = fw64_scene_get_node(scene, i);

        vec3_add(&node->transform.position, &node->transform.position, &offset->position);
        fw64_node_update(node);
    }

    fw64_scene_update_bounding(scene);
}

static void set_scene_ref(SceneManager* scene_manager, int ref_index, SceneDescription* description, fw64Transform* offset) {
    SceneRef* scene_ref = &scene_manager->scene_refs[ref_index];

    if (scene_ref->scene) {
        if (scene_ref->desc.uninit_func) {
            scene_ref->desc.uninit_func(scene_manager->level_arg, scene_ref->scene, scene_ref->data);
        }
        
        fw64_bump_allocator_reset(&scene_ref->allocator);
    }

    // set up the new scene
    memcpy(&scene_ref->desc, description, sizeof(SceneDescription));


    scene_ref->scene = fw64_scene_load(scene_manager->engine->assets, scene_ref->desc.index, &scene_ref->allocator.interface);
    scene_ref->data = scene_ref->allocator.interface.malloc(&scene_ref->allocator.interface, scene_ref->desc.data_size);

    if (offset) {
        apply_offset_to_scene(offset, scene_ref->scene);
    }

    if (scene_ref->desc.init_func) {
        scene_ref->desc.init_func(scene_manager->level_arg, scene_ref->scene, scene_ref->data);
    }
}

void scene_manager_load_current_scene(SceneManager* scene_manager, SceneDescription* description) {
    set_scene_ref(scene_manager, scene_manager->current_scene, description, NULL);
    
    SceneRef* current_scene =  CURRENT_SCENE_REF(scene_manager);
    scene_manager->swap_func(scene_manager->level_arg, current_scene->scene, current_scene->data);
}

void scene_manager_load_next_scene(SceneManager* scene_manager, SceneDescription* description, fw64Transform* offset) {
    SceneRef* next_scene_ref = NEXT_SCENE_REF(scene_manager);
    
    if (next_scene_ref->desc.index == description->index) {
        return;
    }

    set_scene_ref(scene_manager, NEXT_SCENE_INDEX(scene_manager), description, offset);
}

SceneRef* scene_manager_get_current(SceneManager* scene_manager) {
    return CURRENT_SCENE_REF(scene_manager);
}
