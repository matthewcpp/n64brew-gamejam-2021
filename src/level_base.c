#include "level_base.h"

#include "typemap.h"

#include "framework64/matrix.h"

#include <string.h>

#define CURRENT_SCENE_REF(level) (&(level)->scenes[(level)->current_scene])
#define NEXT_SCENE_INDEX(level) (level)->current_scene == 1 ? 0 : 1
#define NEXT_SCENE_REF(level) (&(level)->scenes[NEXT_SCENE_INDEX((level))])


void level_base_init(LevelBase* level, fw64Engine* engine) {
    level->engine = engine;
    chase_camera_init(&level->chase_cam, engine);
    ui_init(&level->ui, engine, &level->player);

    level->current_scene = 0;
    memset(&level->scenes, 0, sizeof(SceneRef) * 2);
}

void level_base_set_scene_data(LevelBase* level, int index, void* scene_data) {
    level->data_args[index] = scene_data;
}

void level_base_set_level_arg(LevelBase* level, void* level_arg) {
    level->level_arg = level_arg;
}

void level_base_setup_player(LevelBase* level, uint32_t start_node_index) {
    fw64Scene* scene = CURRENT_SCENE_REF(level)->scene;
    fw64Node *node = fw64_scene_get_node(scene, start_node_index);

    player_init(&level->player, level->engine, scene, &node->transform.position);
    vec3_set(&level->player.node.transform.scale, 0.01f, 0.01f, 0.01f);
    level->player.node.transform.position = node->transform.position;
    fw64_node_update(&level->player.node);
    player_calculate_size(&level->player);

    level->chase_cam.target = &level->player.node.transform;
}

static void level_swap_scenes(LevelBase* level) {
    level->current_scene = level->current_scene == 1 ? 0 : 1;
    level->player.scene = CURRENT_SCENE_REF(level)->scene;
}


void level_base_update(LevelBase* level){
    SceneRef* current_scene = CURRENT_SCENE_REF(level);
    SceneRef* next_scene = NEXT_SCENE_REF(level);

    if (current_scene->scene) {
        current_scene->update_func(level->level_arg, level->data_args[level->current_scene]);
    }

    player_update(&level->player);
    chase_camera_update(&level->chase_cam);

    // check to see if the player has moved to the next scene
    if (next_scene->scene) {

        if (box_contains_point(fw64_scene_get_initial_bounds(next_scene->scene), &level->player.node.transform.position)) {
            level_swap_scenes(level);
        }
    }

    if (level->player.node.transform.position.y < -10.0f) { // respawn player
        player_reset(&level->player);
    }

    ui_update(&level->ui);
}

void level_base_draw(LevelBase* level) {
    fw64Renderer* renderer = level->engine->renderer;

    fw64_renderer_begin(renderer, &level->chase_cam.camera, FW64_RENDERER_MODE_TRIANGLES, FW64_RENDERER_FLAG_CLEAR);

    if (level->scenes[0].scene)
        fw64_scene_draw_all(level->scenes[0].scene, renderer);

    if (level->scenes[1].scene)
        fw64_scene_draw_all(level->scenes[1].scene, renderer);

    player_draw(&level->player);
    fw64_renderer_end(renderer, FW64_RENDERER_FLAG_NOSWAP);

    fw64_renderer_begin(renderer, &level->chase_cam.camera, FW64_RENDERER_MODE_ORTHO2D, FW64_RENDERER_FLAG_NOCLEAR);
    ui_draw(&level->ui);
    fw64_renderer_end(renderer, FW64_RENDERER_FLAG_SWAP);
}

static void set_scene_ref(LevelBase* level, int ref_index, uint32_t index, SceneInitFunc init_func, SceneFunc update_func, SceneFunc uninit_func, void* arg) {
    SceneRef* scene_ref = &level->scenes[ref_index];

    if (scene_ref->scene) {
        scene_ref->uninit_func(level->level_arg, level->data_args[ref_index]);
        fw64_scene_delete(level->engine->assets, scene_ref->scene); // todo scene clear / reset?
    }
    
    scene_ref->index = index;
    scene_ref->scene = fw64_scene_load(level->engine->assets, index);
    scene_ref->init_func = init_func;
    scene_ref->update_func = update_func;
    scene_ref->uninit_func = uninit_func;

    scene_ref->init_func(scene_ref->scene, level->level_arg, level->data_args[ref_index]);
}

void level_base_load_current_scene(LevelBase* level, uint32_t index, SceneInitFunc init_func, SceneFunc update_func, SceneFunc uninit_func, void* arg) {
    set_scene_ref(level, level->current_scene, index, init_func, update_func, uninit_func, arg);
}

void level_base_load_next_scene(LevelBase* level, uint32_t index, SceneInitFunc init_func, SceneFunc update_func, SceneFunc uninit_func, void* arg) {
    SceneRef* next_scene_ref = NEXT_SCENE_REF(level);
    
    if (next_scene_ref->index != index) {
        set_scene_ref(level, level->current_scene == 1 ? 0 : 1, index, init_func, update_func, uninit_func, arg);
    }
}