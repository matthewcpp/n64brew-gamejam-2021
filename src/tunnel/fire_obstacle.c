#include "fire_obstacle.h"

void fire_rolling_obstacle_init(StaticFireObstacle* obstacle, fw64Node* reference, Player* player, fw64Mesh* flame_mesh) {
    obstacle->player = player;
    
    float flame_width = 3.5f;
    
    Vec3 pos = reference->transform.position;
    float x_pos = pos.x - reference->transform.scale.x + flame_width / 2.0f;

    for (int i = 0; i < STATIC_FIRE_OBSTACLE_NODE_COUNT; i++) {
        fw64Node* node = &obstacle->nodes[i];
        fw64_node_init(node);
        fw64_node_set_mesh(node, flame_mesh);
        vec3_set(&node->transform.position, x_pos, pos.y, pos.z);
        vec3_set(&node->transform.scale, flame_width, -reference->transform.scale.y * 1.75f, 1.0f);
        x_pos += flame_width * 2;
    }
    
    box_set_center_extents(&obstacle->box, &pos, &reference->transform.scale);
}

int fire_rolling_obstacle_update(StaticFireObstacle* obstacle, float time_delta) {
    if (obstacle->player->is_rolling)
        return 0;
    else
        return fw64_collider_test_box(&obstacle->player->collider, &obstacle->box);
}

void fire_jumping_obstacle_init(StaticFireObstacle* obstacle, fw64Node* reference, Player* player, fw64Mesh* flame_mesh) {
    obstacle->player = player;
    
    float flame_width = 3.5f;
    
    Vec3 pos = reference->transform.position;
    float x_pos = pos.x - reference->transform.scale.x + flame_width / 2.0f;

    for (int i = 0; i < STATIC_FIRE_OBSTACLE_NODE_COUNT; i++) {
        fw64Node* node = &obstacle->nodes[i];
        fw64_node_init(node);
        fw64_node_set_mesh(node, flame_mesh);
        vec3_set(&node->transform.position, x_pos, pos.y, pos.z);
        vec3_set(&node->transform.scale, flame_width, reference->transform.scale.y, 1.0f);
        x_pos += flame_width * 2;
    }
    
    box_set_center_extents(&obstacle->box, &pos, &reference->transform.scale);
}

int fire_jumping_obstacle_update(StaticFireObstacle* obstacle, float time_delta) {
    return fw64_collider_test_box(&obstacle->player->collider, &obstacle->box);
}

void draw_static_fire_obstacles(fw64Renderer* renderer, StaticFireObstacle* obstacles, int count) {
    fw64Camera* camera = fw64_renderer_get_camera(renderer);

        for (int i = 0; i < count; i++) {
        StaticFireObstacle* obstacle = obstacles + i;

        for (int j = 0; j < STATIC_FIRE_OBSTACLE_NODE_COUNT; j++) {
            fw64Node* node = obstacle->nodes + j;
            fw64_node_billboard(node, camera);
            fw64_renderer_draw_static_mesh(renderer, &node->transform, node->mesh);
        }
    }
}