#include "enemy.h"

void enemy_init(Enemy* enemy, fw64Engine* engine, Player* player, fw64Mesh* mesh, Vec3* position) {
    enemy->engine = engine;
    enemy->player = player;
    fw64_node_init(&enemy->node);
    fw64_node_set_mesh(&enemy->node, mesh);
    fw64_node_set_collider(&enemy->node, &enemy->collider);
    fw64_node_set_box_collider(&enemy->node, &enemy->collider);

    vec3_set_all(&enemy->node.transform.scale, 0.01);
    enemy->node.transform.position = *position;
    quat_from_euler(&enemy->node.transform.rotation, 0.0f, 180.0f, 0.0f);
    fw64_node_update(&enemy->node);

    enemy->speed = 17.0f;
}

void enemy_update(Enemy* enemy) {
    Vec3 forward;
    fw64_transform_forward(&enemy->node.transform, &forward);
    vec3_add_and_scale(&enemy->node.transform.position, &enemy->node.transform.position, &forward, enemy->speed * enemy->engine->time->time_delta);
    fw64_node_update(&enemy->node);

    Player* player = enemy->player;
    float height_radius = player->height / 2.0f;
    Vec3 query_center = player->node.transform.position;
    query_center.y += height_radius;

    Vec3 out_point;
    if (fw64_collider_test_sphere(enemy->node.collider, &query_center, player->radius, &out_point)) {
        player_reset(player);
    }
}

void enemy_draw(Enemy* enemy) {
    fw64_renderer_draw_static_mesh(enemy->engine->renderer, &enemy->node.transform, enemy->node.mesh);
}