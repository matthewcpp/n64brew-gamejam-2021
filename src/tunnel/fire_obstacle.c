#include "fire_obstacle.h"

void static_fire_obstacle_init(StaticFireObstacle* obstacle, StaticFireObstacleType type, fw64Node* reference, Player* player, fw64Mesh* flame_mesh) {
    obstacle->player = player;
    obstacle->active = 1;
    
    float flame_width = 3.5f;
    
    Vec3 pos = reference->transform.position;
    float x_pos = pos.x - reference->transform.scale.x + flame_width / 2.0f;

    float y_scale = type == STATIC_FIRE_OBSTACLE_ROLL ? -reference->transform.scale.y * 1.75f : 9.0f;

    for (int i = 0; i < STATIC_FIRE_OBSTACLE_NODE_COUNT; i++) {
        fw64Node* node = &obstacle->nodes[i];
        fw64_node_init(node);
        fw64_node_set_mesh(node, flame_mesh);
        vec3_set(&node->transform.position, x_pos, pos.y, pos.z);
        vec3_set(&node->transform.scale, flame_width, y_scale, 1.0f);
        x_pos += flame_width * 2;
    }
    
    box_set_center_extents(&obstacle->box, &pos, &reference->transform.scale);
}

int static_fire_obstacle_update(StaticFireObstacle* obstacle, float time_delta) {
    if (!obstacle->active)
        return 0;

    if (obstacle->type == STATIC_FIRE_OBSTACLE_ROLL && obstacle->player->is_rolling)
        return 0;
    else 
        return fw64_collider_test_box(&obstacle->player->collider, &obstacle->box);
}

void static_fire_obstacle_draw(StaticFireObstacle* obstacle, fw64Renderer* renderer) {
    if (!obstacle->active)
        return;

    fw64Camera* camera = fw64_renderer_get_camera(renderer);

    for (int j = 0; j < STATIC_FIRE_OBSTACLE_NODE_COUNT; j++) {
        fw64Node* node = obstacle->nodes + j;
        fw64_node_billboard(node, camera);
        fw64_renderer_draw_static_mesh(renderer, &node->transform, node->mesh);
    }
}
