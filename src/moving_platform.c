#include "moving_platform.h"

void moving_platform_init(MovingPlatform* platform, fw64Node* node, Vec3* movement_vec, float dist, float speed, Player* player) {
    platform->node = node;
    platform->movement_vec = *movement_vec;
    platform->max_dist = dist;
    platform->player = player;
    platform->speed = speed;

    platform->traveled_dist = 0.0f;
}

void moving_platform_update(MovingPlatform* platform, float time_delta) {
    platform->traveled_dist += platform->speed * time_delta;
    Vec3 previous_pos = platform->node->transform.position;

    // if platform has moved to its max, need to switch directions
    if (platform->traveled_dist >= platform->max_dist) {
        vec3_add_and_scale(&platform->node->transform.position, &platform->node->transform.position, &platform->movement_vec, platform->traveled_dist - platform->max_dist);

        vec3_negate(&platform->movement_vec);
        platform->traveled_dist = 0.0f;
    }
    else {
        vec3_add_and_scale(&platform->node->transform.position, &platform->node->transform.position, &platform->movement_vec, platform->speed * time_delta);
    }

    fw64_node_update(platform->node);

    if (platform->player->ground_node == platform->node) {
        Vec3 delta;
        vec3_subtract(&delta, &platform->node->transform.position, &previous_pos);

        vec3_add(&platform->player->node.transform.position, &platform->player->node.transform.position, &delta);
        platform->player->previous_position = platform->player->node.transform.position;
    }
}