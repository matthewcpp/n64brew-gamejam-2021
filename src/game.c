#include "game.h"
#include "assets.h"

float rotation = 0.0f;

void game_init(Game* game, fw64Engine* engine) {
    game->engine = engine;
    fw64_camera_init(&game->camera);
    game->camera.near = 10.0f;
    game->camera.far = 500.0f;
    vec3_set(&game->camera.transform.position, 2.0f, 150.0f, 265.0f);
    Vec3 up = {0.0f, 1.0f, 0.0f};
    Vec3 target = {0.0f, 0.0f, 0.0f};
    fw64_transform_look_at(&game->camera.transform, &target, &up);
    fw64_camera_update_view_matrix(&game->camera);
    fw64_camera_update_projection_matrix(&game->camera);

    fw64_node_init(&game->n64_logo, fw64_mesh_load(engine->assets, FW64_ASSET_mesh_n64_logo));
}

void game_update(Game* game){
    rotation += 45.0f * game->engine->time->time_delta;
    quat_from_euler(&game->n64_logo.transform.rotation, 0.0f, rotation, 0.0f);
    fw64_transform_update_matrix(&game->n64_logo.transform);
    (void)game;
}

void game_draw(Game* game) {
    fw64_renderer_begin(game->engine->renderer, &game->camera, FW64_RENDERER_MODE_TRIANGLES, FW64_RENDERER_FLAG_CLEAR);
    fw64_renderer_draw_static_mesh(game->engine->renderer, &game->n64_logo.transform, game->n64_logo.mesh);
    fw64_renderer_end(game->engine->renderer, FW64_RENDERER_FLAG_SWAP);
}
