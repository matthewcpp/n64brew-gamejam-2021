#include "fire_obstacle.h"

void fire_rolling_obstacle_init(FireRollingObstacle* obstacle, fw64Node* reference, Player* player, fw64Mesh* flame_mesh) {
    obstacle->player = player;
    
    float flame_width = 3.5f;
    
    Vec3 pos = reference->transform.position;
    float x_pos = pos.x - reference->transform.scale.x + flame_width / 2.0f;

    for (int i = 0; i < ROLL_OBSTACLE_NODE_COUNT; i++) {
        fw64Node* node = &obstacle->nodes[i];
        fw64_node_init(node);
        fw64_node_set_mesh(node, flame_mesh);
        vec3_set(&node->transform.position, x_pos, pos.y, pos.z);
        vec3_set(&node->transform.scale, flame_width, -reference->transform.scale.y * 1.75f, 1.0f);
        x_pos += flame_width * 2;
    }
    
    box_set_center_extents(&obstacle->box, &pos, &reference->transform.scale);
}

int fire_rolling_obstacle_update(FireRollingObstacle* obstacle, float time_delta) {
    if (obstacle->player->is_rolling)
        return 0;
    else
        return fw64_collider_test_box(&obstacle->player->collider, &obstacle->box);
}

void fire_rolling_obstacle_draw(FireRollingObstacle* obstacle, fw64Renderer* renderer) {
    fw64Camera* camera = fw64_renderer_get_camera(renderer);

    for (int i = 0; i < ROLL_OBSTACLE_NODE_COUNT; i++) {
        fw64Node* node = &obstacle->nodes[i];
        fw64_node_billboard(node, camera);
        fw64_renderer_draw_static_mesh(renderer, &node->transform, node->mesh);
    }
}

void fire_obstacle_node_pool_init(FireObstacleNodePool* pool, fw64Mesh* mesh) {
    pool->index = 0;

    for (int i = 0; i < FIRE_OBSTACLE_POOL_SIZE; i++) {
        fw64Node* node = &pool->nodes[i];
        fw64_node_init(node);
        fw64_node_set_mesh(node, mesh);
        pool->stack[i] = node;
    }
}

fw64Node* fire_obstacle_node_pool_get(FireObstacleNodePool* pool) {
    if (pool->index >= FIRE_OBSTACLE_POOL_SIZE) {
        return NULL;
    }
    else
        return pool->stack[pool->index++];
}

void fire_obstacle_node_pool_return(FireObstacleNodePool* pool, fw64Node* node) {
    if (pool->index == 0) {
        return;
    }
    else {
        pool->stack[--pool->index] = node;
    }
}