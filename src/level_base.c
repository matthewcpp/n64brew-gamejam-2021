#include "level_base.h"

#include "typemap.h"
#include "assets.h"

void level_base_init(LevelBase* level, fw64Engine* engine, uint32_t level_idex) {
    level->engine = engine;
    chase_camera_init(&level->chase_cam, engine);

    level->scene = fw64_scene_load(level->engine->assets, level_idex);

    uint32_t node_count = fw64_scene_get_node_count(level->scene);

    for (uint32_t i = 0; i < node_count; i++) {
        fw64Node *node = fw64_scene_get_node(level->scene, i);

        if (node->type == NODE_TYPE_START) {
            player_init(&level->player, level->engine, level->scene, &node->transform.position);
            vec3_set(&level->player.node.transform.scale, 0.01f, 0.01f, 0.01f);
            level->player.node.transform.position = node->transform.position;
            fw64_node_update(&level->player.node);
            player_calculate_size(&level->player);

            level->chase_cam.target = &level->player.node.transform;
        }
    }

    ui_init(&level->ui, engine, &level->player);
}

void level_base_update(LevelBase* level){
    player_update(&level->player);
    chase_camera_update(&level->chase_cam);

    if (level->player.node.transform.position.y < -10.0f) { // respawn player
        player_reset(&level->player);
    }

    ui_update(&level->ui);
}

void level_base_draw(LevelBase* level) {
    fw64Renderer* renderer = level->engine->renderer;

    fw64_renderer_begin(renderer, &level->chase_cam.camera, FW64_RENDERER_MODE_TRIANGLES, FW64_RENDERER_FLAG_CLEAR);
    fw64_scene_draw_all(level->scene, renderer);
    player_draw(&level->player);
    fw64_renderer_end(renderer, FW64_RENDERER_FLAG_NOSWAP);

    fw64_renderer_begin(renderer, &level->chase_cam.camera, FW64_RENDERER_MODE_ORTHO2D, FW64_RENDERER_FLAG_NOCLEAR);
    ui_draw(&level->ui);
    fw64_renderer_end(renderer, FW64_RENDERER_FLAG_SWAP);
}
