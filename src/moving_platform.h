#pragma once

#include "framework64/node.h"
#include "player.h"

typedef struct {
    fw64Node* platform_node;
    fw64Node* target_node;
    float speed;
    Player* player;
    
    Vec3 start_pos;
    Vec3 target_pos;
    float total_dist;
    float current_dist;
} MovingPlatform;

#ifdef __cplusplus
extern "C" {
#endif

void moving_platform_init(MovingPlatform* platform, fw64Node* platform_node, fw64Node* target_node, float speed, Player* player);
void moving_platform_update(MovingPlatform* platform, float time_delta);

#ifdef __cplusplus
}
#endif