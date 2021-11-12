#pragma once

#include "framework64/node.h"
#include "player.h"

typedef struct {
    fw64Node* node;
    
    /** current direction of travel */
    Vec3 movement_vec;

    /** how far the platform will travel before reversing direction */
    float max_dist;

    /** speed at which the platform will travel */
    float speed;
    Player* player;

    /** how far the platform has traveled since last switching directions*/
    float traveled_dist;
} MovingPlatform;

#ifdef __cplusplus
extern "C" {
#endif

void moving_platform_init(MovingPlatform* platform, fw64Node* node, Vec3* movement_vec, float dist, float speed, Player* player);
void moving_platform_update(MovingPlatform* platform, float time_delta);

#ifdef __cplusplus
}
#endif