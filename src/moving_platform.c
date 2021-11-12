#include "moving_platform.h"

void moving_platform_init(MovingPlatform* platform, fw64Node* platform_node, fw64Node* target_node, float speed, Player* player) {
    platform->platform_node = platform_node;
    platform->target_node = target_node;
    platform->speed = speed;
    platform->player = player;

    platform->start_pos = platform_node->transform.position;
    platform->target_pos = target_node->transform.position;
    platform->total_dist = vec3_distance(&platform->start_pos, & platform->target_pos);
    platform->current_dist = 0.0f;
}


void moving_platform_update(MovingPlatform* platform, float time_delta) {
    platform->current_dist += platform->speed * time_delta;
    Vec3 previous_pos = platform->platform_node->transform.position;

    // if platform has moved to its max, need to switch directions
    if (platform->current_dist >= platform->total_dist) {
        platform->platform_node->transform.position = platform->target_pos;
        
        platform->current_dist = 0.0f;
        Vec3 temp = platform->start_pos;
        platform->start_pos = platform->target_pos;
        platform->target_pos = temp;
    }
    else {
        float t = platform->current_dist / platform->total_dist;
        vec3_smoothstep(&platform->platform_node->transform.position, &platform->start_pos, &platform->target_pos, t);
    }

    fw64_node_update(platform->platform_node);

    if (platform->player->ground_node == platform->platform_node) {
        Vec3 delta;
        vec3_subtract(&delta, &platform->platform_node->transform.position, &previous_pos);

        vec3_add(&platform->player->node.transform.position, &platform->player->node.transform.position, &delta);
        platform->player->previous_position = platform->player->node.transform.position;
    }
}