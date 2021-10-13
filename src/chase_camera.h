#pragma once

#include "framework64/camera.h"
#include "framework64/node.h"

typedef struct {
    fw64Camera camera;
    fw64Node* target;

    float back_follow_dist;
    float forward_target_dist;
    float follow_height;
} ChaseCamera;

#ifdef __cplusplus
extern "C" {
#endif

void chase_camera_init(ChaseCamera* camera, fw64Node* target);
void chase_camera_update(ChaseCamera* camera);

#ifdef __cplusplus
}
#endif