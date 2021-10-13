#include "player.h"
#include "assets.h"

#define DEFAULT_PLAYER_MOVE_SPEED 10.0f
#define DEFAULT_PLAYER_ROTATION_SPEED 45.0f
#define STICK_THRESHOLD 0.15f

void player_init(Player* player, fw64Engine* engine) {
    player->engine = engine;
    
    fw64_node_init(&player->node, fw64_mesh_load(player->engine->assets, FW64_ASSET_mesh_penguin));

    player->rotation = 0.0f;
    player->rotation_speed = DEFAULT_PLAYER_ROTATION_SPEED;
    player->move_speed = DEFAULT_PLAYER_MOVE_SPEED;
}

void player_update(Player* player) {
    Vec2 stick;
    fw64_input_stick(player->engine->input, 0, &stick);
    if (stick.x >= STICK_THRESHOLD || stick.x <= -STICK_THRESHOLD  || stick.y >= STICK_THRESHOLD || stick.y <= -STICK_THRESHOLD) {
        float rotation_delta = player->rotation_speed * player->engine->time->time_delta;

        if (stick.x >= STICK_THRESHOLD) {
            player->rotation -= rotation_delta;
        }
        else if (stick.x <= -STICK_THRESHOLD) {
            player->rotation += rotation_delta;
        }

        quat_set_axis_angle(&player->node.transform.rotation, 0, 1, 0, player->rotation * (M_PI / 180.0f));

        Vec3 forward;
        fw64_transform_forward(&player->node.transform, &forward);
        vec3_scale(&forward, &forward, player->move_speed * stick.y * player->engine->time->time_delta);
        vec3_add(&player->node.transform.position, &player->node.transform.position, &forward);
        fw64_node_refresh(&player->node);
    }
}

void player_draw(Player* player) {
    fw64_renderer_draw_static_mesh(player->engine->renderer, &player->node.transform, player->node.mesh);
}