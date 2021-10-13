#include "level1.h"

#include "assets.h"
#include "typemap.h"

static void load_scene(Level1* level1);

void level1_init(Level1* level1, fw64Engine* engine) {
    level1->engine = engine;

    player_init(&level1->player, engine);
    chase_camera_init(&level1->chase_camera, &level1->player.node);

    load_scene(level1);
}

void level1_update(Level1* level1) {
    player_update(&level1->player);
    chase_camera_update(&level1->chase_camera);
    flame_update(&level1->flames[0], level1->engine->time->time_delta);
    flame_update(&level1->flames[1], level1->engine->time->time_delta);
}

void level1_draw(Level1* level1) {
    fw64Renderer* renderer = level1->engine->renderer;

    fw64_renderer_begin(renderer, &level1->chase_camera.camera, FW64_RENDERER_MODE_TRIANGLES, FW64_RENDERER_FLAG_CLEAR);
    player_draw(&level1->player);
    fw64_scene_draw_all(level1->scene, level1->engine->renderer);
    flame_draw(&level1->flames[0], renderer);
    flame_draw(&level1->flames[1], renderer);
    fw64_renderer_end(renderer, FW64_RENDERER_FLAG_SWAP);
}

void load_scene(Level1* level1) {
    level1->scene = fw64_scene_load(level1->engine->assets, FW64_ASSET_scene_simple_scene);

    int flame_index = 0;
    fw64Image* flame_image = fw64_image_load(level1->engine->assets, FW64_ASSET_image_fire_sprite);

    fw64SceneExtra* extras = fw64_scene_get_extras(level1->scene);
    uint32_t extras_count = fw64_scene_get_extra_count(level1->scene);

    for (uint32_t i = 0; i < extras_count; i++) {
        fw64SceneExtra* extra = extras + i;

        if (extra->type == TYPE_START) {
            vec3_set(&level1->player.node.transform.scale, 0.01f, 0.01f, 0.01f);
            level1->player.node.transform.position = extra->position;
            fw64_node_refresh(&level1->player.node);
        }
        else if (extra->type == TYPE_FIRE) {
            Flame* flame = &level1->flames[flame_index++];
            flame_init(flame, level1->engine, flame_image);
            vec3_set(&flame->entity.transform.scale, 3.0f, 5.0f, 3.0f);
            flame->entity.transform.position = extra->position;
            flame->entity.transform.position.y += 3.0f;
        }
    }
}